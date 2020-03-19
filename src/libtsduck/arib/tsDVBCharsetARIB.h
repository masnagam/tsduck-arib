//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2019, Thierry Lelegard
// Copyright (c) 2019 Masayuki Nagamachi <masnagam@gmail.com>
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
//!
//!  @file
//!  Declaration of class DVBCharsetARIB.
//!
//----------------------------------------------------------------------------

#if defined(TS_ARIB)

#pragma once
#include "tsDVBCharset.h"

namespace ts {
    //!
    //! Definition of Japanese character set defined in ARIB STD-B24.
    //! @see ARIB STD-B24, Chapter 7 "Character coding" in Part 2.
    //! @ingroup mpeg
    //!
    class TSDUCKDLL DVBCharsetARIB: public DVBCharset
    {
    public:
        //!
        //! ARIB character set singleton
        //!
        static const DVBCharsetARIB ARIB_STD_B24;

        // Inherited methods.
        virtual bool decode(UString& str, const uint8_t* dvb, size_t dvbSize) const override;
        virtual bool canEncode(const UString& str, size_t start = 0, size_t count = NPOS) const override;
        virtual size_t encode(uint8_t*& buffer, size_t& size, const UString& str, size_t start = 0, size_t count = NPOS) const override;

    protected:
        //!
        //! Constructor.
        //! There is only one definition for such DVB character sets.
        //!
        DVBCharsetARIB() : DVBCharset(u"ARIB-STD-B24", 0x081B24) {}

    private:
        // Inaccessible operations.
        DVBCharsetARIB(const DVBCharsetARIB&) = delete;
        DVBCharsetARIB& operator=(const DVBCharsetARIB&) = delete;
    };
}

#endif // TS_ARIB
