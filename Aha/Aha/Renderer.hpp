//
//  Renderer.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 02/10/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    class Scene;
    
    class Renderer
    {
    public:
        virtual ~Renderer()
        {
            ;
        }
        virtual void* context() const = 0;
        virtual void* pixelFormat() const = 0;
        virtual std::shared_ptr <Scene> scene() const = 0;
        virtual void scene(std::shared_ptr <Scene> s) = 0;
        virtual void preRender() = 0;
        virtual void render() = 0;
        virtual void postRender() = 0;
        virtual void setClearColor(float r, float g, float b, float a) = 0;
        virtual void clear() = 0;
        virtual void setViewport(int x, int y, int width, int height) = 0;
    };
}
