//
//  HasParent.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 08/01/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

template <typename T>
class HasParent
{
public:
    T* getParent() const
    {
        return parent_;
    }
    
    void setParent(T* parent)
    {
        parent_ = parent;
    }
    
protected:
    T* parent_{nullptr};
};
