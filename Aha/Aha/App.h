//
//  App.h
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
#   include "Android/AppAndroid.h"
#elif defined AHA_PLATFORM_IOS
#   include "IOS/AppIOS.h"
#elif defined AHA_PLATFORM_OSX
#   include "OSX/AppOSX.h"
#elif defined AHA_PLATFORM_WIN
#   include "WIN/AppWin.h"
#endif


namespace aha
{
    class AppAndroid;
    class AppIOS;
    class AppOSX;
    class AppWin;
    class AppUnknown;
    
    
    using AppPlatformPolicy =
    std::conditional <Platform::OS == Platform::Os::Android, AppAndroid,
    std::conditional <Platform::OS == Platform::Os::IOS, AppIOS,
    std::conditional <Platform::OS == Platform::Os::OSX, AppOSX,
    std::conditional <Platform::OS == Platform::Os::Win, AppWin,
    AppUnknown>::type>::type>::type>::type;
    
    
    class App : public AppPlatformPolicy
    {
        ;
    };
}
