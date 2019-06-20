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
//!
//!  @file
//!  Representation of an ARIB series descriptor
//!
//----------------------------------------------------------------------------

#if defined(TS_ARIB)

#pragma once
#include "tsAbstractDescriptor.h"
#include "tsTime.h"
#include "tsVariable.h"

namespace ts {
    //!
    //! Representation of an ARIB series descriptor.
    //! @see ARIB STD-B10 v4.6-E2, 6.2.33.
    //! @ingroup descriptor
    //!
    class TSDUCKDLL ARIBSeriesDescriptor : public AbstractDescriptor
    {
    public:
        // Public members:
        uint16_t series_id;            //!< 16 bits, see ARIB STD-B10 v4.6-E2, 6.2.33.
        uint8_t repeat_label;          //!< 4 bits, see ARIB STD-B10 v4.6-E2, 6.2.33.
        uint8_t program_pattern;       //!< 3 bits, see ARIB STD-B10 v4.6-E2, 6.2.33.
        bool expire_date_valid;        //!< 1 bits, see ARIB STD-B10 v4.6-E2, 6.2.33.
        Time expire_date;              //!< 16 bits, see ARIB STD-B10 v4.6-E2, 6.2.33.
        uint16_t episode_number;       //!< 12 bits, see ARIB STD-B10 v4.6-E2, 6.2.33.
        uint16_t last_episode_number;  //!< 12 bit, see ARIB STD-B10 v4.6-E2, 6.2.33.
        UString series_name;           //!< A string, see ARIB STD-B10 v4.6-E2, 6.2.33.

        //!
        //! Default constructor.
        //!
        ARIBSeriesDescriptor();

        //!
        //! Constructor from a binary descriptor
        //! @param [in,out] duck TSDuck execution context.
        //! @param [in] bin A binary descriptor to deserialize.
        //!
        ARIBSeriesDescriptor(DuckContext& duck, const Descriptor& bin);

        // Inherited methods
        virtual void serialize(DuckContext&, Descriptor&) const override;
        virtual void deserialize(DuckContext&, const Descriptor&) override;
        virtual void buildXML(DuckContext&, xml::Element*) const override;
        virtual void fromXML(DuckContext&, const xml::Element*) override;
        DeclareDisplayDescriptor();
    };
}

#endif
