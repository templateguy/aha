//
//  App.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Window.hpp"
#include AHA_PLATFORM_SPECIFIC(RendererGL,.h)
#include "Scene.hpp"


namespace aha
{
    class Application_ final
    {
    public:
        void init(const std::string& title = "Aha", float width = -1.0f, float height = -1.0f, bool isFullScreen = false)
        {
            struct Window_ : public Window
            {
                Window_(const std::string& title, float width, float height, bool isFullScreen) : Window(title, width, height, isFullScreen) {}
            };
            window_ = std::make_shared <Window_> (title, width, height, isFullScreen);
            renderer_ = std::shared_ptr <Renderer> (new RendererGLOSX(window_));
        }
        
        void run(std::shared_ptr <Scene> scene = std::shared_ptr <Scene> (nullptr))
        {
            if(scene)
            {
                setScene(scene);
            }
            window_->startRenderLoop();
        }
        
        void setScene(std::shared_ptr <Scene> scene)
        {
            renderer_->scene(scene);
        }
        
        size_t getWindowWidth() const
        {
            if(window_)
            {
                return window_->width();
            }
            return 0u;
        }
        
        size_t getWindowHeight() const
        {
            if(window_)
            {
                return window_->height();
            }
            return 0u;
        }
        
        std::vector <std::string> openFileDialog(const std::vector<std::string>& fileTypes, bool multiple)
        {
            if(window_)
            {
                return window_->openFileDialog(fileTypes, multiple);
            }
            return std::vector <std::string> ();
        }
        
    private:
        std::shared_ptr <Window> window_{};
        std::shared_ptr <Renderer> renderer_{};
        AHA_MAKE_SINGLETON(Application_);
    };
}

#define Application Application_::getInstance()
