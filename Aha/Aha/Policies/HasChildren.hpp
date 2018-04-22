//
//  HasChildren.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 08/01/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <vector>


namespace aha
{
    template <typename T>
    struct HasChildren
    {
        std::vector <T> children;
    };
}
