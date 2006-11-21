
/* Copyright (c) 2006, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQ_FRAME_H
#define EQ_FRAME_H

#include <eq/net/object.h>
#include <eq/vmmlib/Vector2.h>
#include <eq/client/pixelViewport.h>

namespace eqs
{
    class Frame;
}

namespace eq
{
    class FrameBuffer;

    /**
     * A holder for a FrameBuffer and frame parameters.
     */
    class Frame : public eqNet::Object
    {
    public:
        /** 
         * The frame format defines which components of the framebuffer are to
         * be used during recomposition.
         */
        enum Format
        {
            FORMAT_NONE      = 0,
            FORMAT_UNDEFINED = 0x1,      //!< Inherit, only if no others are set
            FORMAT_COLOR     = 0x10,     //!< Use color images
            FORMAT_DEPTH     = 0x10000,  //!< Use depth images
            FORMAT_ALL       = 0xfffffff
        };

        /** 
         * Constructs a new Frame.
         */
        Frame( const void* data, uint64_t dataSize );

        /**
         * @name Data Access
         */
        //*{
        const vmml::Vector2i& getOffset() const { return _data.offset; }
        //*}

        /**
         * @name Operations
         */
        //*{

        /** 
         * Read back a set of images according to the current frame data.
         * 
         * The images are added to the frame, existing images are retained.
         */
        void startReadback();
        
        /** Synchronize the image readback. */
        void syncReadback();

        /** 
         * Transmit the frame data to the specified node.
         *
         * Used internally after readback to push the image data to the input
         * frame nodes. Do not use directly.
         * 
         * @param toNode the receiving node.
         */
        void transmit( eqBase::RefPtr<eqNet::Node> toNode );

        /** 
         * Test the readiness of the frame.
         * 
         * The readiness of the frame is automatically managed by the frame
         * buffer readback and transmit implementation.
         * 
         * @return true if the frame is ready, false if not. 
         */
        bool isReady();

        /** Wait for the frame to become available. */
        void waitReady();
        //*}

    protected:
        /** @sa eqNet::Object::unpack */
        virtual void unpack( const void* data, const uint64_t size ) 
            { _clear(); eqNet::Object::unpack( data, size ); }

    private:
        std::string _name;

        /** All distributed Data shared between eq::Frame and eqs::Frame. */
        struct Data
        {
            vmml::Vector2i       offset;
            Format               format;
            eqNet::ObjectVersion buffer;
        }
            _data;
        friend class eqs::Frame;

        FrameBuffer* _buffer;

        FrameBuffer* _getBuffer();

        /** 
         * Clear the frame by deleting the attached images and unsetting the
         * frame buffer.
         */
        void _clear();
    };
};
#endif // EQ_FRAME_H
