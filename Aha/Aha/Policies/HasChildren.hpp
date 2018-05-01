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
    class HasChildren
    {
    public:
        void addChild(const T& child)
        {
            children_.emplace_back(child);
        }
        
        const T& getChildAt(size_t index) const
        {
            return children_[index];
        }
        
        const std::vector <T>& getAllChildren() const
        {
            return children_;
        }
        
    protected:
        std::vector <T> children_;
    };
}
