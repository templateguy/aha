//
//  HasSize.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 08/01/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class HasSize
    {
    public:
        const T& getSize() const
        {
            return size_;
        }
        
        void setSize(const T& size)
        {
            size_ = size;
        }
        
    protected:
        T size_{};
    };
}
