//
//  Vec4.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 01/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class Vec4
    {
    public:
        Vec4() = default;
        
        Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
        {
            ;
        }
        
        union
        {
            T x;
            T r;
        };
        union
        {
            T y;
            T g;
        };
        union
        {
            T z;
            T b;
        };
        union
        {
            T w;
            T a;
        };
    };
    
    using Vec4i = Vec4 <int>;
    using Vec4f = Vec4 <float>;
    
    using Color4i = Vec4 <int>;
    using Color4f = Vec4 <float>;
}
