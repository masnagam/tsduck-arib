//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
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
//
//  Representation of an ARIB series descriptor
//
//----------------------------------------------------------------------------

#if defined(TS_ARIB)

#include "tsARIBSeriesDescriptor.h"
#include "tsDescriptor.h"
#include "tsMJD.h"
#include "tsTablesDisplay.h"
#include "tsTablesFactory.h"
#include "tsxmlElement.h"
TSDUCK_SOURCE;

#define MY_XML_NAME u"arib_series_descriptor"
#define MY_DID ts::DID_ARIB_SERIES
#define MY_STD ts::STD_ARIB

TS_XML_DESCRIPTOR_FACTORY(ts::ARIBSeriesDescriptor, MY_XML_NAME);
TS_ID_DESCRIPTOR_FACTORY(ts::ARIBSeriesDescriptor, ts::EDID::Standard(MY_DID));
TS_ID_DESCRIPTOR_DISPLAY(ts::ARIBSeriesDescriptor::DisplayDescriptor, ts::EDID::Standard(MY_DID));


//----------------------------------------------------------------------------
// Default constructor:
//----------------------------------------------------------------------------

ts::ARIBSeriesDescriptor::ARIBSeriesDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    series_id(0),
    repeat_label(0),
    program_pattern(0),
    expire_date_valid(false),
    episode_number(0),
    last_episode_number(0),
    series_name()
{
    _is_valid = true;
}


//----------------------------------------------------------------------------
// Constructor from a binary descriptor
//----------------------------------------------------------------------------

ts::ARIBSeriesDescriptor::ARIBSeriesDescriptor(DuckContext& duck, const Descriptor& desc) :
    ARIBSeriesDescriptor()
{
    deserialize(duck, desc);
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::ARIBSeriesDescriptor::serialize(DuckContext& duck, Descriptor& desc) const
{
    ByteBlockPtr bbp(serializeStart());

    bbp->appendUInt16(series_id);
    bbp->appendUInt8(
        uint8_t((repeat_label    & 0x0F) << 4) |
        uint8_t((program_pattern & 0x07) << 1) |
        (expire_date_valid         ? 0x01 : 0));
    bbp->appendUInt8(uint8_t((episode_number >> 4) & 0xFF));
    bbp->appendUInt8(
        uint8_t((episode_number << 4) & 0xF0) |
        uint8_t((last_episode_number >> 8) & 0x0F));
    bbp->appendUInt8(uint8_t(last_episode_number & 0x000F));
    bbp->append(duck.toDVB(series_name));

    serializeEnd(desc, bbp);
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::ARIBSeriesDescriptor::deserialize(DuckContext& duck, const Descriptor& desc)
{
    const uint8_t* data = desc.payload();
    size_t size = desc.payloadSize();

    _is_valid = desc.isValid() && desc.tag() == _tag && size >= 8;

    if (_is_valid) {
        series_id = GetUInt16(data);
        repeat_label = (data[2] >> 4) & 0x0F;
        program_pattern = (data[2] >> 1) & 0x07;
        expire_date_valid = (data[2] & 0x01) != 0;
        DecodeMJD(data + 3, 2, expire_date);
        episode_number = uint16_t((uint16_t(data[5]) << 4) | (data[6] >> 4));
        last_episode_number = uint16_t((uint16_t(data[6]) << 8) | data[7]);
        series_name.assign(duck.fromDVB(data + 8, size - 8));
    }
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::ARIBSeriesDescriptor::DisplayDescriptor(TablesDisplay& display, DID did, const uint8_t* data, size_t size, int indent, TID tid, PDS pds)
{
    // TODO
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::ARIBSeriesDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    // TODO
}


//----------------------------------------------------------------------------
// XML deserialization
//----------------------------------------------------------------------------

void ts::ARIBSeriesDescriptor::fromXML(DuckContext& duck, const xml::Element* element)
{
    // TODO
    _is_valid = false;
}

#endif
