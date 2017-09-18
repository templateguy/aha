//
//  Window.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <type_traits>
#include "Config/Config.hpp"

// Platform specific inclusion.
#if defined AHA_PLATFORM_ANDROID
#   include "Android/WindowAndroid.h"
#elif defined AHA_PLATFORM_IOS
#   include "IOS/WindowIOS.h"
#elif defined AHA_PLATFORM_OSX
#   include "OSX/WindowOSX.h"
#elif defined AHA_PLATFORM_WIN
#   include "Win/WindowWin.h"
#endif


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
