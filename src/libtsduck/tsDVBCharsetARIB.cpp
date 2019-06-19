//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2019, Thierry Lelegard
// Copyright (c) 2019 Masayuki Nagamachi <masayuki.nagamachi@gmail.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------

#if defined(TS_ARIB)

#if defined(__llvm__)
#pragma clang diagnostic ignored "-Wpadded"
#endif

#include <memory>

extern "C" {
#include <aribb24/aribb24.h>
#include <aribb24/decoder.h>
}

#include "tsDVBCharsetARIB.h"
#include "tsUString.h"
TSDUCK_SOURCE;

namespace {

// For RAII
class ARIBStringDecoder
{
public:
    ARIBStringDecoder() = default;

    ~ARIBStringDecoder() {
        if (_decoder != nullptr) {
            arib_finalize_decoder(_decoder);
        }
        if (_instance != nullptr) {
            arib_instance_destroy(_instance);
        }
    }

    bool Init() {
        _instance = arib_instance_new(nullptr);
        if (_instance == nullptr) {
            return false;
        }

        _decoder = arib_get_decoder(_instance);
        if (_decoder == nullptr) {
            return false;
        }

        arib_initialize_decoder(_decoder);
        return true;
    }

    ts::UString decode(const uint8_t* dvb, size_t dvbSize) {
        auto utf8Size = dvbSize * 4;
        auto utf8 = std::make_unique<char[]>(utf8Size + 1);
        utf8Size = arib_decode_buffer(_decoder, reinterpret_cast<const unsigned char*>(dvb), dvbSize, utf8.get(), utf8Size);
        return ts::UString::FromUTF8(utf8.get(), utf8Size);
    }

    ARIBStringDecoder(const ARIBStringDecoder&) = delete;
    ARIBStringDecoder& operator=(const ARIBStringDecoder&) = delete;

private:
    arib_instance_t* _instance = nullptr;
    arib_decoder_t* _decoder = nullptr;
};

}

// ARIB character set singleton
const ts::DVBCharsetARIB ts::DVBCharsetARIB::ARIB_STD_B24;


//----------------------------------------------------------------------------
// Decode a DVB string from the specified byte buffer.
//----------------------------------------------------------------------------

bool ts::DVBCharsetARIB::decode(UString& str, const uint8_t* dvb, size_t dvbSize) const
{
    ARIBStringDecoder decoder;
    if (!decoder.Init()) {
        return false;
    }
    str = decoder.decode(dvb, dvbSize);
    return true;
}


//----------------------------------------------------------------------------
// Check if a string can be encoded using the charset.
//----------------------------------------------------------------------------

bool ts::DVBCharsetARIB::canEncode(const UString& str, size_t start, size_t count) const
{
    return false;
}


//----------------------------------------------------------------------------
// Encode a C++ Unicode string into a DVB string.
//----------------------------------------------------------------------------

size_t ts::DVBCharsetARIB::encode(uint8_t*& buffer, size_t& size, const UString& str, size_t start, size_t count) const
{
    return 0;
}

#endif // TS_ARIB
