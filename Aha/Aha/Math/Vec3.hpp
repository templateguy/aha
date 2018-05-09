//
//  Vec3.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 02/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class Vec3
    {
    public:
        Vec3() = default;
        
        Vec3(T x, T y, T z) : x(x), y(y), z(z)
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
    };
    
    using Vec3i = Vec3 <int>;
    using Vec3f = Vec3 <float>;
    
    using Point3i = Vec3 <int>;
    using Point3f = Vec3 <float>;
    
    using Color3i = Vec3 <int>;
    using Color3f = Vec3 <float>;
}
