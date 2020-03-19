//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2020, Thierry Lelegard
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

#include "tsSSUSubgroupAssociationDescriptor.h"
#include "tsDescriptor.h"
#include "tsTablesDisplay.h"
#include "tsTablesFactory.h"
#include "tsxmlElement.h"
TSDUCK_SOURCE;

#define MY_XML_NAME u"SSU_subgroup_association_descriptor"
#define MY_DID ts::DID_UNT_SUBGROUP_ASSOC
#define MY_TID ts::TID_UNT
#define MY_STD ts::STD_DVB

TS_XML_TABSPEC_DESCRIPTOR_FACTORY(ts::SSUSubgroupAssociationDescriptor, MY_XML_NAME, MY_TID);
TS_ID_DESCRIPTOR_FACTORY(ts::SSUSubgroupAssociationDescriptor, ts::EDID::TableSpecific(MY_DID, MY_TID));
TS_FACTORY_REGISTER(ts::SSUSubgroupAssociationDescriptor::DisplayDescriptor, ts::EDID::TableSpecific(MY_DID, MY_TID));


//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

ts::SSUSubgroupAssociationDescriptor::SSUSubgroupAssociationDescriptor() :
    AbstractDescriptor(MY_DID, MY_XML_NAME, MY_STD, 0),
    subgroup_tag(0)
{
    _is_valid = true;
}

ts::SSUSubgroupAssociationDescriptor::SSUSubgroupAssociationDescriptor(DuckContext& duck, const Descriptor& desc) :
    SSUSubgroupAssociationDescriptor()
{
    deserialize(duck, desc);
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::SSUSubgroupAssociationDescriptor::serialize(DuckContext& duck, Descriptor& desc) const
{
    ByteBlockPtr bbp(serializeStart());
    bbp->appendUInt40(subgroup_tag);
    serializeEnd(desc, bbp);
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::SSUSubgroupAssociationDescriptor::deserialize(DuckContext& duck, const Descriptor& desc)
{
    const uint8_t* data = desc.payload();
    size_t size = desc.payloadSize();

    _is_valid = desc.isValid() && desc.tag() == _tag && size == 5;

    if (_is_valid) {
        subgroup_tag = GetUInt40(data);
    }
}


//----------------------------------------------------------------------------
// Static method to display a descriptor.
//----------------------------------------------------------------------------

void ts::SSUSubgroupAssociationDescriptor::DisplayDescriptor(TablesDisplay& display, DID did, const uint8_t* data, size_t size, int indent, TID tid, PDS pds)
{
    std::ostream& strm(display.duck().out());
    const std::string margin(indent, ' ');

    if (size >= 5) {
        const uint64_t tag = GetUInt40(data);
        strm << margin << UString::Format(u"Subgroup tag: 0x%010X (%d)", {tag, tag}) << std::endl;
        data += 5; size -= 5;
    }

    display.displayExtraData(data, size, indent);
}


//----------------------------------------------------------------------------
// XML serialization
//----------------------------------------------------------------------------

void ts::SSUSubgroupAssociationDescriptor::buildXML(DuckContext& duck, xml::Element* root) const
{
    root->setIntAttribute(u"subgroup_tag", subgroup_tag, true);
}


//----------------------------------------------------------------------------
// XML deserialization
//----------------------------------------------------------------------------

void ts::SSUSubgroupAssociationDescriptor::fromXML(DuckContext& duck, const xml::Element* element)
{
    xml::ElementVector children;
    _is_valid =
        checkXMLName(element) &&
        element->getIntAttribute<uint64_t>(subgroup_tag, u"subgroup_tag", true, 0, 0, TS_UCONST64(0x000000FFFFFFFFFF));
}
