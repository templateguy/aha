//
//  Platform.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 18/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Preprocessor/Platform.h"
#include "../SelectValue.hpp"


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
