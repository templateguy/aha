//
//  PlatformConfig.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#define AHA_PLATFORM_IS_WIN 0
#define AHA_PLATFORM_IS_OSX 0
#define AHA_PLATFORM_IS_LINUX 0
#define AHA_PLATFORM_IS_UNIX 0
#define AHA_PLATFORM_IS_POSIX 0
#define AHA_PLATFORM_IS_IOS 0
#define AHA_PLATFORM_IS_ANDROID 0
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
#
#   else
#       error "Unknown Apple Platform"
#   endif
#
#elif __ANDROID__
#   define AHA_PLATFORM_ANDROID
#   undef AHA_PLATFORM_IS_ANDROID
#   define AHA_PLATFORM_IS_ANDROID 1
#
#elif __linux__
#   define AHA_PLATFORM_LINUX
#   undef AHA_PLATFORM_IS_LINUX
#   define AHA_PLATFORM_IS_LINUX 1
#
#elif __unix__
#   define AHA_PLATFORM_UNIX
#   undef AHA_PLATFORM_IS_UNIX
#   define AHA_PLATFORM_IS_UNIX 1
#
#elif defined(_POSIX_VERSION)
#   define AHA_PLATFORM_POSIX
#   undef AHA_PLATFORM_IS_POSIX
#   define AHA_PLATFORM_IS_POSIX 1
#
#else
#   error "Unknow Platform"
#endif
