//
//  Singleton.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 18/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class Singleton
    {
    public:
        static T& getInstance()
        {
            // Since C++11, static local variables are thread safe.
            static T instance;
            return instance;
        }
        
        Singleton(const Singleton&) = delete;
        const Singleton& operator =(const Singleton&) = delete;
    protected:
        Singleton() = default;
    };
}
