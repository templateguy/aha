//
//  HasID.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 02/10/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class HasID
    {
    public:
        HasID() : id_(generateID_())
        {
            ;
        }
        
        inline auto& getID() const
        {
            return id_;
        }
        
    protected:
        T generateID_() const
        {
            static T count;
            return count++;
        }
        
        T id_;
    };
}
