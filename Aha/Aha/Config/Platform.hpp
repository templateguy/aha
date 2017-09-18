//
//  Platform.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 18/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../SelectValue.hpp"


#define AHA_PLATFORM_IS_ANDROID 0
#define AHA_PLATFORM_IS_IOS 0
#define AHA_PLATFORM_IS_OSX 0
#define AHA_PLATFORM_IS_WIN 0
#
#define AHA_ARCH_IS_X86 0
#define AHA_ARCH_IS_X64 0
#
#define AHA_TARGET_IS_DEVICE 0
#define AHA_TARGET_IS_SIMULATOR 0
#
#
#ifdef _WIN32
#   define AHA_PLATFORM_WIN
#   undef AHA_PLATFORM_IS_WIN
#   define AHA_PLATFORM_IS_WIN 1
#
#   ifdef _WIN64
#       define AHA_ARCH_X64
#       undef AHA_ARCH_IS_X64
#       define AHA_IS_ARCH_IS_X64 1
#
#   else
#       define AHA_ARCH_X86
#       undef AHA_ARCH_IS_X86
#       define AHA_ARCH_IS_X86 1
#   endif
#
#elif __APPLE__
#   include "TargetConditionals.h"
#
#   if TARGET_IPHONE_SIMULATOR
#       define AHA_PLATFORM_IOS
#       undef AHA_PLATFORM_IS_IOS
#       define AHA_PLATFORM_IS_IOS 1
#       define AHA_TARGET_SIMULATOR
#
#   elif TARGET_OS_IPHONE
#       define AHA_PLATFORM_IOS
#       undef AHA_PLATFORM_IS_IOS
#       define AHA_PLATFORM_IS_IOS 1
#       define AHA_TARGET_SIMULATOR
#
#   elif TARGET_OS_MAC
#       define AHA_PLATFORM_OSX
#       undef AHA_PLATFORM_IS_OSX
#       define AHA_PLATFORM_IS_OSX 1
#   endif
#
#elif __ANDROID__
#   define AHA_PLATFORM_ANDROID
#   undef AHA_PLATFORM_IS_ANDROID
#   define AHA_PLATFORM_IS_ANDROID 1
#endif


namespace aha
{
    class Platform
    {
    public:
        enum class Os
        {
            Android,
            IOS,
            OSX,
            Win,
            Unknown
        };
        
        enum class Arch
        {
            X86,
            X64
        };
        
        enum class Target
        {
            Device,
            Simulator
        };
        
        static constexpr Os OS =
        SelectValue <Os, AHA_PLATFORM_IS_ANDROID, Os::Android,
        SelectValue <Os, AHA_PLATFORM_IS_IOS, Os::IOS,
        SelectValue <Os, AHA_PLATFORM_IS_OSX, Os::OSX,
        SelectValue <Os, AHA_PLATFORM_IS_WIN, Os::Win,
        Os::Unknown>::Value>::Value>::Value>::Value;
        
        static constexpr Arch ARCH = SelectValue <Arch, AHA_ARCH_IS_X86, Arch::X86, Arch::X64>::Value;
        
        static constexpr Target TARGET = SelectValue <Target, AHA_TARGET_IS_DEVICE, Target::Device, Target::Simulator>::Value;
    };
}
