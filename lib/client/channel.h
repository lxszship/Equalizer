/* Copyright (c) 2005-2006, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQ_CHANNEL_H
#define EQ_CHANNEL_H

#include "commands.h"
#include "frustum.h"
#include "pixelViewport.h"
#include "window.h"

#include <eq/net/base.h>
#include <eq/net/object.h>

namespace eq
{
    class Channel;
    class Node;
    class Range;
    class RenderContext;
    class SceneObject;

    class Channel : public eqNet::Object
    {
    public:
        /** 
         * Constructs a new channel.
         */
        Channel();

        /**
         * @name Data Access
         */
        //*{
        Window* getWindow()   const
            { return _window; }
        Pipe*   getPipe()   const
            { return ( _window ? _window->getPipe() : NULL );}
        Node* getNode() const 
            { return ( _window ? _window->getNode() : NULL );}
        Config* getConfig() const 
            { return ( _window ? _window->getConfig() : NULL );}

        const std::string& getName() const { return _name; }

        /** 
         * Set the near and far planes for this channel.
         * 
         * The near and far planes are set during initialisation and are
         * inherited by source channels contributing to the rendering of this
         * channel. Dynamic near and far planes can be applied using
         * applyNearFar.
         *
         * @param near the near plane.
         * @param far the far plane.
         */
        void setNearFar( const float Nnear, const float Ffar )
            { _near = Nnear; _far = Ffar; }

        /** 
         * Returns the current near and far planes for this channel.
         *
         * The current near and far plane depends on the context from which this
         * function is called.
         * 
         * @param near a pointer to store the near plane.
         * @param far a pointer to store the far plane.
         */
        void getNearFar( float *near, float *far );
        //*}

        /**
         * @name Callbacks
         *
         * The callbacks are called by Equalizer during rendering to execute
         * various actions.
         */
        //@{

        /** 
         * Initialises this channel.
         * 
         * @param initID the init identifier.
         */
        virtual bool init( const uint32_t initID ){ return true; }

        /** 
         * Exit this channel.
         */
        virtual bool exit(){ return true; }

        /** 
         * Clear the frame buffer.
         * 
         * @param frameID the per-frame identifier.
         */
        virtual void clear( const uint32_t frameID );

        /** 
         * Draw the scene.
         * 
         * @param frameID the per-frame identifier.
         */
        virtual void draw( const uint32_t frameID );
        //@}

        /**
         * @name Operations
         *
         * Operations are only available from within certain callbacks.
         */
        //*{

        /** 
         * Apply the current rendering buffer.
         */
        void applyBuffer();

        /** 
         * Apply the OpenGL viewport for the current rendering task.
         */
        void applyViewport();

        /** 
         * @return the channel's pixel viewport
         */
        const PixelViewport& getPixelViewport() const;

        /** 
         * Set the channel's fractional viewport wrt its parent pipe.
         *
         * Updates the pixel viewport accordingly.
         * 
         * @param vp the fractional viewport.
         */
        void setViewport( const Viewport& vp );

        /** @return the current fractional viewport of this channel. */
        const Viewport& getViewport() const;

        /** @return the view frustum for the current rendering task. */
        const Frustum& getFrustum() const;

        /** @return the database range for the current rendering task. */
        const Range& getRange() const;

        /**
         * Apply the frustum matrix for the current rendering task.
         */
        void applyFrustum() const;

        /**
         * @return the modelling transformation to position and orient the view
         *         frustum.
         */
        const float* getHeadTransform() const;

        /** 
         * Apply the modelling transformation to position and orient the view
         * frustum.
         */
        void applyHeadTransform() const;
        //*}

        /** @name Scene Object Access. */
        //*{
        SceneObject* getNextSceneObject();
        SceneObject* checkNextSceneObject();
        //void putSceneObject( SceneObject* object );
        void passSceneObject( SceneObject* object );
        void flushSceneObjects();
        //*}

    protected:
        /**
         * Destructs the channel.
         */
        virtual ~Channel();

    private:
        /** The parent node. */
        friend class   Window;
        Window*        _window;

        /** The name. */
        std::string    _name;

        /** Static near plane. */
        float          _near;
        /** Static far plane. */
        float          _far;

        /** server-supplied rendering data. */
        RenderContext *_context;

        /** The native pixel viewport wrt the window. */
        eq::PixelViewport _pvp;

        /** The native viewport. */
        Viewport       _vp;

        /** The native ('identity') frustum. */
        Frustum        _frustum;

        /** 
         * Set the channel's pixel viewport wrt its parent pipe.
         *
         * Updates the fractional viewport accordingly.
         * 
         * @param pvp the viewport in pixels.
         */
        void setPixelViewport( const PixelViewport& pvp );
        
        /* The command handler functions. */
        eqNet::CommandResult _pushCommand( eqNet::Node* node,
                                           const eqNet::Packet* packet );
        eqNet::CommandResult _reqInit( eqNet::Node* node,
                                       const eqNet::Packet* packet );
        eqNet::CommandResult _reqExit( eqNet::Node* node,
                                       const eqNet::Packet* packet );
        eqNet::CommandResult _reqClear( eqNet::Node* node,
                                        const eqNet::Packet* packet );
        eqNet::CommandResult _reqDraw( eqNet::Node* node,
                                       const eqNet::Packet* packet );
    };
}

#endif // EQ_CHANNEL_H

