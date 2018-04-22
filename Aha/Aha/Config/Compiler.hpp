//
//  Compiler.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 18/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Preprocessor/Compiler.h"
#include "../SelectValue.hpp"


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
