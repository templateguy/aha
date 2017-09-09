//
//  App.h
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
    
    
    using AppPlatformPolicy = SelectType <Platform::Os == Platform::OS::ANDROID, AppAndroid,
    SelectType <Platform::Os == Platform::OS::IOS, AppIOS,
    SelectType <Platform::Os == Platform::OS::OSX, AppOSX,
    SelectType <Platform::Os == Platform::OS::WIN, AppWin,
    AppOSX>::Type>::Type>::Type>::Type;
    
    
    class App : public AppPlatformPolicy
    {
    public:
        App();
        ~App();
    };
}
