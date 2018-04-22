//
//  RendererGLOSX.h
//  Aha
//
//  Created by Priyanshi Thakur on 07/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <memory>
#include "../Renderer.hpp"


namespace aha
{
    class WindowOSX;
    
    class RendererGLOSX : public Renderer
    {
    public:
        virtual ~RendererGLOSX();
        virtual void* context() const override;
        virtual void* pixelFormat() const override;
        virtual std::shared_ptr <Scene> scene() const override;
        virtual void scene(std::shared_ptr <Scene> s) override;
        virtual void preRender() override;
        virtual void render() override;
        virtual void postRender() override;
        virtual void setClearColor(float r, float g, float b, float a) override;
        virtual void clear() override;
        virtual void setViewport(int x, int y, int width, int height) override;
        
        friend class Application_;
        
    protected:
        RendererGLOSX(std::shared_ptr <WindowOSX> window);
        void prepareGL_(std::shared_ptr <WindowOSX> window);
        
        std::shared_ptr <Scene> scene_;
        struct impl_;
        std::unique_ptr <impl_> pimpl_;
    };
}
