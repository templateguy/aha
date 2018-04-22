//
//  Rect.h
//  Aha
//
//  Created by Priyanshi Thakur on 02/10/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    class Rect
    {
    public:
        Rect(float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f) : x(x), y(y), w(w), h(h)
        {
            ;
        }
        
        int x;
        int y;
        int w;
        int h;
    };
}
