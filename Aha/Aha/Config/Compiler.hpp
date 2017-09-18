//
//  Compiler.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 18/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../SelectValue.hpp"


#define AHA_COMPILER_IS_ARM     0
#define AHA_COMPILER_IS_CLANG   0
#define AHA_COMPILER_IS_GCC     0
#define AHA_COMPILER_IS_INTEL   0
#define AHA_COMPILER_IS_LLVM    0
#define AHA_COMPILER_IS_MINGW   0
#define AHA_COMPILER_IS_MSVC    0
#
#
# // ARM Compiler
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#   define AHA_COMPILER_ARM
#   undef AHA_COMPILER_IS_ARM
#   define AHA_COMPILER_IS_ARM 1
#endif
#
# // Clang
#if defined(__clang__)
#   define AHA_COMPILER_CLANG
#   undef AHA_COMPILER_IS_CLANG
#   define AHA_COMPILER_IS_CLANG 1
#endif
#
# // GCC
#if defined(__GNUC__)
#   define AHA_COMPILER_GCC
#   undef AHA_COMPILER_IS_GCC
#   define AHA_COMPILER_IS_GCC 1
#endif
#
# // Intel Compiler
#if defined(__INTEL_COMPILER)
#   define AHA_COMPILER_INTEL
#   undef AHA_COMPILER_IS_INTEL
#   define AHA_COMPILER_IS_INTEL 1
#endif
#
# // LLVM
#if defined(__llvm__)
#   define AHA_COMPILER_LLVM
#   undef AHA_COMPILER_IS_LLVM
#   define AHA_COMPILER_IS_LLVM 1
#endif
#
# // MINGW
#if defined(__MINGW32__) || defined(__MINGW64__)
#   define AHA_COMPILER_MINGW
#   undef AHA_COMPILER_IS_MINGW
#   define AHA_COMPILER_IS_MINGW 1
#endif
#
# // Microsoft Visual C++
#if defined(_MSC_VER)
#   define AHA_COMPILER_MSVC
#   undef AHA_COMPILER_IS_MSVC
#   define AHA_COMPILER_IS_MSVC 1
#endif


namespace aha
{
    struct Compiler
    {
        enum class Name
        {
            Arm,
            Clang,
            GCC,
            Intel,
            LLVM,
            MSVC,
            MINGW,
            Unknown
        };
        
        static constexpr Name NAME =
        SelectValue <Name, AHA_COMPILER_IS_ARM, Name::Arm,
        SelectValue <Name, AHA_COMPILER_IS_CLANG, Name::Clang,
        SelectValue <Name, AHA_COMPILER_IS_GCC, Name::GCC,
        SelectValue <Name, AHA_COMPILER_IS_INTEL, Name::Intel,
        SelectValue <Name, AHA_COMPILER_IS_LLVM, Name::LLVM,
        SelectValue <Name, AHA_COMPILER_IS_MINGW, Name::MINGW,
        SelectValue <Name, AHA_COMPILER_IS_MSVC, Name::MSVC,
        Name::Unknown>::Value>::Value>::Value>::Value>::Value>::Value>::Value;
    };
}
