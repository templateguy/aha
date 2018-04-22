//
//  Compiler.h
//  Aha
//
//  Created by Priyanshi Thakur on 26/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once
#
#
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
