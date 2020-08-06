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
//!
//!  @file
//!  Representation of a DVB enhanced_AC-3_descriptor
//!
//----------------------------------------------------------------------------

#pragma once
#include "tsAbstractDescriptor.h"
#include "tsVariable.h"

namespace ts {
    //!
    //! Representation of a DVB enhanced_AC-3_descriptor.
    //! @see ETSI 300 468, D.5.
    //! @ingroup descriptor
    //!
    class TSDUCKDLL DVBEnhancedAC3Descriptor : public AbstractDescriptor
    {
    public:
        // Public members:
        Variable<uint8_t> component_type;  //!< See ETSI 300 468, D.5.
        Variable<uint8_t> bsid;            //!< See ETSI 300 468, D.5.
        Variable<uint8_t> mainid;          //!< See ETSI 300 468, D.5.
        Variable<uint8_t> asvc;            //!< See ETSI 300 468, D.5.
        bool              mixinfoexists;   //!< See ETSI 300 468, D.5.
        Variable<uint8_t> substream1;      //!< See ETSI 300 468, D.5.
        Variable<uint8_t> substream2;      //!< See ETSI 300 468, D.5.
        Variable<uint8_t> substream3;      //!< See ETSI 300 468, D.5.
        ByteBlock         additional_info; //!< See ETSI 300 468, D.5.

        //!
        //! Default constructor.
        //!
        DVBEnhancedAC3Descriptor();

        //!
        //! Constructor from a binary descriptor
        //! @param [in,out] duck TSDuck execution context.
        //! @param [in] bin A binary descriptor to deserialize.
        //!
        DVBEnhancedAC3Descriptor(DuckContext& duck, const Descriptor& bin);

        //!
        //! Merge inside this object missing information which can be found in other object.
        //! @param [in] other Other object to get missing information from.
        //!
        void merge(const DVBEnhancedAC3Descriptor& other);

        // Inherited methods
        virtual void serialize(DuckContext&, Descriptor&) const override;
        virtual void deserialize(DuckContext&, const Descriptor&) override;
        DeclareDisplayDescriptor();

    protected:
        // Inherited methods
        virtual void clearContent() override;
        virtual void buildXML(DuckContext&, xml::Element*) const override;
        virtual bool analyzeXML(DuckContext& duck, const xml::Element* element) override;
    };

    //!
    //! Legacy name for a DVB enhanced_AC-3_descriptor.
    //!
    typedef DVBEnhancedAC3Descriptor EnhancedAC3Descriptor;
}
