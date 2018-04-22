//
//  Window.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Preprocessor/Platform.h"
#include "Config/Platform.hpp"

// Platform specific inclusion. See Preprocessor/Platform.h
#include AHA_PLATFORM_SPECIFIC(Window, .h)


// Forward declaration of platform specific classes. See Preprocesor/Platform.h
AHA_PLATFORM_SPECIFIC_FORWARD_DECLARATIONS(Window);


namespace aha
{
    // AHA_PLATFORM_POLICY(class) expands to the platform specific version of the class. See Preprocessor/Platform.h
    using WindowPlatformPolicy = AHA_PLATFORM_POLICY(Window);
    
    class Window : public WindowPlatformPolicy
    {
    public:
        friend class Application_;
        
    protected:
        Window(const std::string& title, float width, float height, bool isFullScreen) : WindowPlatformPolicy(title, width, height, isFullScreen)
        {
            ;
        }
    };
}
