//
//  Window.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "PlatformConfig.h"
#include "Platform.hpp"
#include "SelectType.hpp"

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
    
    
    using WindowPlatformPolicy = SelectType <Platform::Os == Platform::OS::ANDROID, WindowAndroid,
    SelectType <Platform::Os == Platform::OS::IOS, WindowIOS,
    SelectType <Platform::Os == Platform::OS::OSX, WindowOSX,
    SelectType <Platform::Os == Platform::OS::WIN, WindowWin,
    WindowOSX>::Type>::Type>::Type>::Type;
    
    
    class Window : public WindowPlatformPolicy
    {
        
    };
}
