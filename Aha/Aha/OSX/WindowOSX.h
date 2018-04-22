//
//  WindowOSX.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <string>
#include <memory>


namespace aha
{
    class Renderer;
    
    class WindowOSX
    {
    public:
        WindowOSX(const std::string& title, float width, float height, bool isFullScreen);
        ~WindowOSX();
        
        size_t width() const;
        void width(size_t w);
        size_t height() const;
        void height(size_t h);
        void size(size_t w, size_t h);
        aha::Renderer* renderer() const;
        void renderer(aha::Renderer* renderer);
        
        void startRenderLoop();
        void stopRenderLoop();
        
    private:
        struct impl_;
        std::unique_ptr <impl_> pimpl_;
    };
}
