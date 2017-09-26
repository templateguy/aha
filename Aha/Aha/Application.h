//
//  App.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <type_traits>
#include <memory>
#include <vector>
#include "Preprocessor/Platform.h"
#include "Config/Platform.hpp"
#include "Window.h"

// Platform specific inclusion. See Preprocessor/Platform.h
#include AHA_PLATFORM_SPECIFIC(Application, .h)


namespace aha
{
    class ApplicationAndroid;
    class ApplicationIOS;
    class ApplicationOSX;
    class ApplicationWin;
    class ApplicationUnknown;
    
    
    using AppPlatformPolicy =
    std::conditional <Platform::OS == Platform::Os::Android, ApplicationAndroid,
    std::conditional <Platform::OS == Platform::Os::IOS, ApplicationIOS,
    std::conditional <Platform::OS == Platform::Os::OSX, ApplicationOSX,
    std::conditional <Platform::OS == Platform::Os::Win, ApplicationWin,
    ApplicationUnknown>::type>::type>::type>::type;
    
    
    class Application_ : public AppPlatformPolicy
    {
    public:
        inline void init()
        {
            
        }
        
        inline void run()
        {
            AppPlatformPolicy::run();
        }
        
        inline auto createWindow()
        {
            std::shared_ptr <Window> window(std::make_shared <Window> ());
            windows_.push_back(window);
            return window;
        }
        
        static auto& getInstance()
        {
            static Application_ application;
            return application;
        }
        
        Application_(const Application_&) = delete;
        const Application_& operator =(const Application_&) = delete;
        
    protected:
        Application_()
        {
            
        }
        
        std::vector <std::shared_ptr <Window>> windows_;
    };
}

#define Application aha::Application_::getInstance()
