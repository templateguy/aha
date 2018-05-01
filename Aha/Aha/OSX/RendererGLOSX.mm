//
//  RendererGLOSX.m
//  AhaOSX
//
//  Created by Priyanshi Thakur on 07/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <cstdlib>
#include <OpenGL/gl.h>
#include "RendererGLOSX.h"
#include "WindowOSX.h"
#include "../Scene.hpp"


namespace aha
{
    struct RendererGLOSX::impl_
    {
        NSOpenGLContext* context_;
        NSOpenGLPixelFormat* pixelFormat_;
    };
    
    RendererGLOSX::RendererGLOSX(std::shared_ptr <WindowOSX> window) : pimpl_(std::make_unique <impl_> ())
    {
        if(window)
        {
            prepareGL_(window);
            window->renderer(this);
        }
    }
    
    RendererGLOSX::~RendererGLOSX()
    {
        [pimpl_->context_ release];
        pimpl_->context_ = nil;
        [pimpl_->pixelFormat_ release];
        pimpl_->pixelFormat_ = nil;
    }
    
    void* RendererGLOSX::context() const
    {
        return pimpl_->context_;
    }
    
    void* RendererGLOSX::pixelFormat() const
    {
        return pimpl_->pixelFormat_;
    }
    
    std::shared_ptr <Scene> RendererGLOSX::scene() const
    {
        return scene_;
    }
    
    void RendererGLOSX::scene(std::shared_ptr <Scene> s)
    {
        scene_ = s;
    }
    
    void RendererGLOSX::preRender()
    {
        mutex_.lock();
        [pimpl_->context_ makeCurrentContext];
        
        // We draw on a secondary thread through the display link
        // Add a mutex around to avoid the threads from accessing the context simultaneously
        //CGLLockContext([pimpl_->context_ CGLContextObj]);
    }
    
    void RendererGLOSX::render()
    {
        if(scene_)
        {
            scene_->render();
        }
    }
    
    void RendererGLOSX::postRender()
    {
        if(scene_)
        {
            [pimpl_->context_ flushBuffer];
        }
        mutex_.unlock();
    }
    
    void RendererGLOSX::setClearColor(float r, float g, float b, float a)
    {
        glClearColor(0, 0, 0, 0);
    }
    
    void RendererGLOSX::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    
    void RendererGLOSX::setViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
    
    void RendererGLOSX::prepareGL_(std::shared_ptr <WindowOSX> window)
    {
        NSOpenGLPixelFormatAttribute attrs[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAStencilSize, 8,
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAAccelerated,
            0
        };
        pimpl_->pixelFormat_ = [[NSOpenGLPixelFormat alloc] initWithAttributes : attrs];
        [pimpl_->pixelFormat_ retain];
        pimpl_->context_ = [[NSOpenGLContext alloc] initWithFormat : pimpl_->pixelFormat_ shareContext : nil];
        [pimpl_->context_ retain];
        [pimpl_->context_ makeCurrentContext];
        setViewport(0, 0, static_cast <int> (window->width()), static_cast <int> (window->height()));
    }
}
