//
//  SelectType.hpp
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <bool Condition, typename TypeA, typename TypeB>
    struct SelectType
    {
        using Type = TypeA;
    };
    
    template <typename TypeA, typename TypeB>
    struct SelectType <false, TypeA, TypeB>
    {
        using Type = TypeB;
    };
}
