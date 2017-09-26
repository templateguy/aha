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


// Forward declares platform specific classes. See Preprocesor/Platform.h
AHA_PLATFORM_SPECIFIC_FORWARD_DECLARATIONS(Application);


namespace aha
{
    
    // AHA_PLATFORM_POLICY(class) expands to the platform specific version of the class. See Preprocessor/Platform.h
    using ApplicationPlatformPolicy = AHA_PLATFORM_POLICY(Application);
    
    
    class Application_ : public ApplicationPlatformPolicy
    {
    public:
        inline void init()
        {
            
        }
        
        inline void run()
        {
            ApplicationPlatformPolicy::run();
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
