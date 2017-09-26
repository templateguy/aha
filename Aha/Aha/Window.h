//
//  Window.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <type_traits>
#include "Preprocessor/Platform.h"
#include "Config/Platform.hpp"

// Platform specific inclusion. See Preprocessor/Platform.h
#include AHA_PLATFORM_SPECIFIC(Window, .h)


namespace aha
{
    class WindowAndroid;
    class WindowIOS;
    class WindowOSX;
    class WindowWin;
    class WindowUnknownPlatform;
    
    
    using WindowPlatformPolicy =
    std::conditional <Platform::OS == Platform::Os::Android, WindowAndroid,
    std::conditional <Platform::OS == Platform::Os::IOS, WindowIOS,
    std::conditional <Platform::OS == Platform::Os::OSX, WindowOSX,
    std::conditional <Platform::OS == Platform::Os::Win, WindowWin,
    WindowUnknownPlatform>::type>::type>::type>::type;
    
    
    class Window : public WindowPlatformPolicy
    {
        ;
    };
}
