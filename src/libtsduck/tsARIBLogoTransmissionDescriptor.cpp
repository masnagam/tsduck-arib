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
//  Representation of an ARIB logo transmission descriptor
//
//----------------------------------------------------------------------------

#if defined(TS_ARIB)

#include "tsARIBLogoTransmissionDescriptor.h"
#include "tsDescriptor.h"
#include "tsTablesDisplay.h"
#include "tsTablesFactory.h"
#include "tsxmlElement.h"
TSDUCK_SOURCE;

#define MY_XML_NAME u"arib_logo_transmission_descriptor"
#define MY_DID ts::DID_ARIB_LOGO_TRANSMISSION
#define MY_STD ts::STD_ARIB

TS_XML_DESCRIPTOR_FACTORY(ts::ARIBLogoTransmissionDescriptor, MY_XML_NAME);
TS_ID_DESCRIPTOR_FACTORY(ts::ARIBLogoTransmissionDescriptor, ts::EDID::Standard(MY_DID));
TS_FACTORY_REGISTER(ts::ARIBLogoTransmissionDescriptor::DisplayDescriptor, ts::EDID::Standard(MY_DID));


//----------------------------------------------------------------------------
// Default constructor:
//----------------------------------------------------------------------------

ts::ARIBLogoTransmissionDescriptor::ARIBLogoTransmissionDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    logo_transmission_type(0),
    logo_id(0),
    logo_version(0),
    download_data_id(0),
    simple_logo()
{
    _is_valid = true;
}


//----------------------------------------------------------------------------
// Constructor from a binary descriptor
//----------------------------------------------------------------------------

ts::ARIBLogoTransmissionDescriptor::ARIBLogoTransmissionDescriptor(DuckContext& duck, const Descriptor& desc) :
    ARIBLogoTransmissionDescriptor()
{
    deserialize(duck, desc);
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::ARIBLogoTransmissionDescriptor::serialize(DuckContext& duck, Descriptor& desc) const
{
    ByteBlockPtr bbp(serializeStart());

    // TODO

    serializeEnd(desc, bbp);
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::ARIBLogoTransmissionDescriptor::deserialize(DuckContext& duck, const Descriptor& desc)
{
    const uint8_t* data = desc.payload();
    size_t size = desc.payloadSize();

    _is_valid = desc.isValid() && desc.tag() == _tag && size >= 1;

    if (!_is_valid) {
        return;
    }

    logo_transmission_type = data[0];
    data += 1; size -= 1;

    switch (logo_transmission_type) {
        case 1:
            if (size < 6) {
                _is_valid = false;
                return;
            }
            logo_id = GetUInt16(data) & 0x01FF;
            logo_version = GetUInt16(data + 2) & 0x0FFF;
            download_data_id = GetUInt16(data + 4);
            break;
        case 2:
            if (size < 2) {
                _is_valid = false;
                return;
            }
            logo_id = GetUInt16(data) & 0x01FF;
            break;
        case 3:
            simple_logo = duck.fromDVBWithByteLength(data, size);
            break;
        default:
            break;
    }
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::ARIBLogoTransmissionDescriptor::DisplayDescriptor(TablesDisplay& display, DID did, const uint8_t* data, size_t size, int indent, TID tid, PDS pds)
{
    // TODO
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::ARIBLogoTransmissionDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    // TODO
}


//----------------------------------------------------------------------------
// XML deserialization
//----------------------------------------------------------------------------

void ts::ARIBLogoTransmissionDescriptor::fromXML(DuckContext& duck, const xml::Element* element)
{
    // TODO
    _is_valid = false;
}

#endif
