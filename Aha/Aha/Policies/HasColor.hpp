//
//  Color.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 08/01/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class HasColor
    {
    public:
        const T& getColor() const
        {
            return color_;
        }
        
        void setColor(const T& color)
        {
            color_ = color;
        }
        
    protected:
        T color_;
    };
}
