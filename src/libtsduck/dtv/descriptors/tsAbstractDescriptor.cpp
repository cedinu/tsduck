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

#include "tsAbstractDescriptor.h"
#include "tsDescriptor.h"
#include "tsDescriptorList.h"
#include "tsPSIBuffer.h"
TSDUCK_SOURCE;


//----------------------------------------------------------------------------
// Constructors and destructors.
//----------------------------------------------------------------------------

ts::AbstractDescriptor::AbstractDescriptor(DID tag, const UChar* xml_name, Standards standards, PDS pds, const UChar* xml_legacy_name) :
    AbstractSignalization(xml_name, standards, xml_legacy_name),
    _tag(tag),
    _required_pds(pds)
{
}

ts::AbstractDescriptor::~AbstractDescriptor()
{
}


//----------------------------------------------------------------------------
// Get the extended descriptor tag (first byte in payload).
//----------------------------------------------------------------------------

ts::DID ts::AbstractDescriptor::extendedTag() const
{
    // By default, there no extended descriptor tag.
    // MPEG-defined and DVB-defined extension descriptors must override this virtual method.
    return EDID_NULL;
}


//----------------------------------------------------------------------------
// Descriptor serialization.
//----------------------------------------------------------------------------

void ts::AbstractDescriptor::serialize(DuckContext& duck, Descriptor& bin) const
{
    if (!isValid()) {
        // The descriptor is already invalid.
        bin.invalidate();
    }
    else {
        // Allocate a byte block of the maximum descriptor size.
        ByteBlockPtr bbp(new ByteBlock(MAX_DESCRIPTOR_SIZE));
        CheckNonNull(bbp.pointer());

        // Map a serialization buffer over the payload part.
        PSIBuffer buf(duck, bbp->data() + 2, bbp->size() - 2, false);

        // If this is an extension descriptor, add extended tag.
        const DID etag = extendedTag();
        if (etag != EDID_NULL) {
            buf.putUInt8(etag);
        }

        // Let the subclass serialize the payload in the buffer.
        serializePayload(buf);

        if (buf.error()) {
            // Serialization error, not a valid descriptor.
            bin.invalidate();
        }
        else {
            // Update the actual descriptor size.
            const size_t size = buf.currentWriteByteOffset();
            (*bbp)[0] = _tag;
            (*bbp)[1] = uint8_t(size);

            // Resize the byte block and store it into the descriptor.
            bbp->resize(2 + size);
            bin = Descriptor(bbp, ShareMode::SHARE);
        }
    }
}


//----------------------------------------------------------------------------
// Descriptor deserialization.
//----------------------------------------------------------------------------

void ts::AbstractDescriptor::deserialize(DuckContext& duck, const Descriptor& bin)
{
    // Make sure the object is cleared before analyzing the binary descriptor.
    clear();

    if (!bin.isValid() || bin.tag() != _tag) {
        // If the binary descriptor is already invalid or has the wrong descriptor tag, this object is invalid too.
        invalidate();
    }
    else {
        // Map a deserialization read-only buffer over the payload part.
        PSIBuffer buf(duck, bin.payload(), bin.payloadSize());

        // If this is an extension descriptor, check that the expected extended tag is present in the payload.
        const DID etag = extendedTag();
        if (etag != EDID_NULL && (buf.getUInt8() != etag || buf.error())) {
            invalidate();
            return;
        }

        // Let the subclass deserialize the payload in the buffer.
        deserializePayload(buf);

        if (buf.error() || !buf.endOfRead()) {
            // Deserialization error or extraneous data, not a valid descriptor.
            clear();
            invalidate();
        }
    }
}


//----------------------------------------------------------------------------
// Deserialize from a descriptor list.
//----------------------------------------------------------------------------

void ts::AbstractDescriptor::deserialize(DuckContext& duck, const DescriptorList& dlist, size_t index)
{
    if (index > dlist.count()) {
        invalidate();
    }
    else {
        deserialize(duck, *dlist[index]);
    }
}
