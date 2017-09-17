//
//  App.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <type_traits>
#include "PlatformConfig.h"
#include "Platform.hpp"

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
    
    
    using AppPlatformPolicy =
    std::conditional <Platform::Os == Platform::OS::ANDROID, AppAndroid,
    std::conditional <Platform::Os == Platform::OS::IOS, AppIOS,
    std::conditional <Platform::Os == Platform::OS::OSX, AppOSX,
    std::conditional <Platform::Os == Platform::OS::WIN, AppWin,
    AppOSX>::type>::type>::type>::type;
    
    
    class App : public AppPlatformPolicy
    {
    public:
        App();
        ~App();
    };
}
