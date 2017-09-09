//
//  SelectValue.hpp
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T, bool Condition, T ValueA, T ValueB>
    struct SelectValue
    {
        static constexpr T Value = ValueA;
    };
    
    template <typename T, T ValueA, T ValueB>
    struct SelectValue <T, false, ValueA, ValueB>
    {
        static constexpr T Value = ValueB;
    };
}
