//
//  Platform.hpp
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "PlatformConfig.h"
#include "SelectValue.hpp"


namespace aha
{
    class Platform
    {
    public:
        enum class OS
        {
            WIN,
            OSX,
            LINUX,
            UNIX,
            POSIX,
            IOS,
            ANDROID
        };
        
        enum class ARCH
        {
            X86,
            X64
        };
        
        enum class TARGET
        {
            DEVICE,
            SIMULATOR
        };
        
        static constexpr OS Os = SelectValue <OS, AHA_PLATFORM_IS_WIN, OS::WIN,
        SelectValue <OS, AHA_PLATFORM_IS_OSX, OS::OSX,
        SelectValue <OS, AHA_PLATFORM_IS_LINUX, OS::LINUX,
        SelectValue <OS, AHA_PLATFORM_IS_UNIX, OS::UNIX,
        SelectValue <OS, AHA_PLATFORM_IS_POSIX, OS::POSIX,
        SelectValue <OS, AHA_PLATFORM_IS_IOS, OS::IOS,
        SelectValue <OS, AHA_PLATFORM_IS_ANDROID, OS::ANDROID,
        OS::OSX>::Value>::Value>::Value>::Value>::Value>::Value>::Value;
        
        static constexpr ARCH Arch = SelectValue <ARCH, AHA_ARCH_IS_X86, ARCH::X86, ARCH::X64>::Value;
        
        static constexpr TARGET Target = SelectValue <TARGET, AHA_TARGET_IS_DEVICE, TARGET::DEVICE, TARGET::SIMULATOR>::Value;
    };
}
