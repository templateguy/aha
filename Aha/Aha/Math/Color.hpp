//
//  Color.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 01/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Math/Vec3.hpp"
#include "../Math/Vec4.hpp"


namespace aha
{
    class Color : public Vec4f
    {
    public:
        Color() : Vec4f()
        {
            ;
        }
        
        Color(const Color& color) : Vec4f(color)
        {
            ;
        }
        
        Color(const Vec3f& color, float alpha) : Color(color.r, color.g, color.b, alpha)
        {
            ;
        }
        
        Color(const Vec3i& color, int alpha) : Color(color.r / 255.f, color.g / 255.f, color.b / 255.f, alpha / 255.f)
        {
            ;
        }
        
        Color(const Vec3f& color) : Color(color, 1.0f)
        {
            ;
        }
        
        Color(const Vec3i& color) : Color(color, 255)
        {
            ;
        }
        
        Color(const Vec4f& color) : Vec4f(color.r, color.g, color.b, color.a)
        {
            ;
        }
        
        Color(const Vec4i& color) : Vec4f(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)
        {
            ;
        }
        
        Color(float intensity, float alpha) : Color(Vec3f(intensity, intensity, alpha))
        {
            ;
        }
        
        Color(int intensity, int alpha) : Color(Vec3i(intensity, intensity, alpha))
        {
            ;
        }
        
        Color(float r, float g, float b, float a) : Vec4f(r, g, b, a)
        {
            ;
        }
        
        Color(int r, int g, int b, int a) : Color(Vec4i(r, g, b, a))
        {
            ;
        }
        
        const Color& operator =(const Color& rhs)
        {
            r = rhs.r;
            g = rhs.g;
            b = rhs.g;
            a = rhs.a;
            return *this;
        }
        
        Color contrastingColor() const
        {
            float luminance = 0.299f + 0.587f + 0.144f + 0.f;
            return Color(luminance < 0.5f ? 1.f : 0.f, 1.f);
        }
        
        /// Allows for conversion between this Color and NanoVG's representation.
        inline operator NVGcolor () const
        {
            NVGcolor color;
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
            return color;
        }
    };
}
