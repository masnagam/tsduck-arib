//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
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
//
//  Representation of an ARIB audio component descriptor
//
//----------------------------------------------------------------------------

#if defined(TS_ARIB)

#include "tsARIBAudioComponentDescriptor.h"
#include "tsDescriptor.h"
#include "tsTablesDisplay.h"
#include "tsTablesFactory.h"
#include "tsxmlElement.h"
TSDUCK_SOURCE;

#define MY_XML_NAME u"arib_audio_component_descriptor"
#define MY_DID ts::DID_ARIB_AUDIO_COMPONENT
#define MY_STD ts::STD_ARIB

TS_XML_DESCRIPTOR_FACTORY(ts::ARIBAudioComponentDescriptor, MY_XML_NAME);
TS_ID_DESCRIPTOR_FACTORY(ts::ARIBAudioComponentDescriptor, ts::EDID::Standard(MY_DID));
TS_FACTORY_REGISTER(ts::ARIBAudioComponentDescriptor::DisplayDescriptor, ts::EDID::Standard(MY_DID));


//----------------------------------------------------------------------------
// Default constructor:
//----------------------------------------------------------------------------

ts::ARIBAudioComponentDescriptor::ARIBAudioComponentDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    stream_content_ext(0),
    stream_content(0),
    component_type(0),
    component_tag(0),
    stream_type(0),
    simulcast_group_tag(0),
    es_multi_lingual(false),
    main_component(false),
    quality_indicator(0),
    sampling_rate(0),
    language_code(),
    language_code2(),
    text()
{
    _is_valid = true;
}


//----------------------------------------------------------------------------
// Constructor from a binary descriptor
//----------------------------------------------------------------------------

ts::ARIBAudioComponentDescriptor::ARIBAudioComponentDescriptor(DuckContext& duck, const Descriptor& desc) :
    ARIBAudioComponentDescriptor()
{
    deserialize(duck, desc);
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::ARIBAudioComponentDescriptor::serialize(DuckContext& duck, Descriptor& desc) const
{
    ByteBlockPtr bbp(serializeStart());

    bbp->appendUInt8(uint8_t(stream_content_ext << 4) | (stream_content & 0x0F));
    bbp->appendUInt8(component_type);
    bbp->appendUInt8(component_tag);
    bbp->appendUInt8(stream_type);
    bbp->appendUInt8(simulcast_group_tag);
    bbp->appendUInt8(
        (es_multi_lingual            ? 0x80 : 0) |
        (main_component              ? 0x40 : 0) |
        uint8_t((quality_indicator & 0x03) << 4) |
        uint8_t((sampling_rate     & 0x07) << 1));
    if (!SerializeLanguageCode(*bbp, language_code)) {
        desc.invalidate();
        return;
    }
    if (es_multi_lingual) {
      if (!SerializeLanguageCode(*bbp, language_code2)) {
        desc.invalidate();
        return;
      }
    }
    bbp->append(duck.toDVB(text));

    serializeEnd(desc, bbp);
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::ARIBAudioComponentDescriptor::deserialize(DuckContext& duck, const Descriptor& desc)
{
    const uint8_t* data = desc.payload();
    size_t size = desc.payloadSize();

    _is_valid = desc.isValid() && desc.tag() == _tag && size >= 9;

    if (_is_valid) {
        stream_content_ext = (data[0] >> 4) & 0x0F;
        stream_content = data[0] & 0x0F;
        component_type = data[1];
        component_tag = data[2];
        stream_type = data[3];
        simulcast_group_tag = data[4];
        es_multi_lingual = (data[5] & 0x80) != 0;
        main_component = (data[5] & 0x40) != 0;
        quality_indicator = (data[5] >> 4) & 0x03;
        sampling_rate = (data[5] >> 1) & 0x07;
        language_code.assign(duck.fromDVB(data + 6, 3));
        data += 9;
        size -= 9;
    }
    if (es_multi_lingual) {
        if (size < 3) {
            _is_valid = false;
        } else {
            language_code2.assign(duck.fromDVB(data, 3));
            data += 3;
            size -= 3;
        }
    }
    if (_is_valid) {
        text.assign(duck.fromDVB(data, size));
    }
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::ARIBAudioComponentDescriptor::DisplayDescriptor(TablesDisplay& display, DID did, const uint8_t* data, size_t size, int indent, TID tid, PDS pds)
{
    // TODO
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::ARIBAudioComponentDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    // TODO
}


//----------------------------------------------------------------------------
// XML deserialization
//----------------------------------------------------------------------------

void ts::ARIBAudioComponentDescriptor::fromXML(DuckContext& duck, const xml::Element* element)
{
    // TODO
    _is_valid = false;
}

#endif
