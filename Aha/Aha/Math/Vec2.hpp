//
//  Vector2.h
//  Aha
//
//  Created by Priyanshi Thakur on 02/10/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class Vec2
    {
    public:
        Vec2() = default;
        
        Vec2(T x, T y) : x(x), y(y)
        {
            ;
        }
        
        Vec2 operator +(const Vec2& rhs) const
        {
            return Vec2(x + rhs.x, y + rhs.y);
        }
        
        Vec2 operator -(const Vec2& rhs) const
        {
            return Vec2(x - rhs.x, y - rhs.y);
        }
        
        const Vec2& operator +=(const Vec2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        
        const Vec2& operator -=(const Vec2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }
        
        union
        {
            T x;
            T width;
            T u;
        };
        union
        {
            T y;
            T height;
            T v;
        };
    };
    
    using Vec2i = Vec2 <int>;
    using Vec2f = Vec2 <float>;
    
    using Point2i = Vec2 <int>;
    using Point2f = Vec2 <float>;
    
    using Size2i = Vec2 <int>;
    using Size2f = Vec2 <float>;
}
