//
//  Platform.h
//  Aha
//
//  Created by Priyanshi Thakur on 26/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once
#
#
#include "Common.h"
#
#
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
#
#
#define AHA_ANDROID_SPECIFIC(file, ext) AHA_STRINGIFY(AHA_EXPAND(Android/)AHA_EXPAND(file)AHA_EXPAND(Android)AHA_EXPAND(ext))
#define AHA_IOS_SPECIFIC(file, ext) AHA_STRINGIFY(AHA_EXPAND(IOS/)AHA_EXPAND(file)AHA_EXPAND(IOS)AHA_EXPAND(ext))
#define AHA_OSX_SPECIFIC(file, ext) AHA_STRINGIFY(AHA_EXPAND(OSX/)AHA_EXPAND(file)AHA_EXPAND(OSX)AHA_EXPAND(ext))
#define AHA_WIN_SPECIFIC(file, ext) AHA_STRINGIFY(AHA_EXPAND(Win/)AHA_EXPAND(file)AHA_EXPAND(Win)AHA_EXPAND(ext))
#
#define AHA_PLATFORM_SPECIFIC(file, ext) \
AHA_IF(AHA_PLATFORM_IS_ANDROID, AHA_ANDROID_SPECIFIC(file, ext), \
AHA_IF(AHA_PLATFORM_IS_IOS, AHA_IOS_SPECIFIC(file, ext), \
AHA_IF(AHA_PLATFORM_IS_OSX, AHA_OSX_SPECIFIC(file, ext), \
AHA_IF(AHA_PLATFORM_IS_WIN, AHA_WIN_SPECIFIC(file, ext), \
AHA_OSX_SPECIFIC(file, ext)))))
#
#
#define AHA_PLATFORM_SPECIFIC_FORWARD_DECLARATIONS(type) \
\
namespace aha \
{ \
    class AHA_CONCAT(type, Android); \
    class AHA_CONCAT(type, IOS); \
    class AHA_CONCAT(type, OSX); \
    class AHA_CONCAT(type, Win); \
    class AHA_CONCAT(type, Unknown); \
}


#define AHA_PLATFORM_POLICY(class) \
std::conditional <Platform::OS == Platform::Os::Android, AHA_CONCAT(class, Android), \
std::conditional <Platform::OS == Platform::Os::IOS, AHA_CONCAT(class, IOS), \
std::conditional <Platform::OS == Platform::Os::OSX, AHA_CONCAT(class, OSX), \
std::conditional <Platform::OS == Platform::Os::Win, AHA_CONCAT(class, Win), \
AHA_CONCAT(class, Unknown)>::type>::type>::type>::type;
