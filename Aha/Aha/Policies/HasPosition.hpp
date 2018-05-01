//
//  HasPosition.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 08/01/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class HasPosition
    {
    public:
        const T& getPosition() const
        {
            return position_;
        }
        
        void setPosition(const T& position)
        {
            position_ = position;
        }
        
    protected:
        T position_{};
    };
}
