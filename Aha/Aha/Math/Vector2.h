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
    class Vector2
    {
    public:
        Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y)
        {
            ;
        }
        
        float x;
        float y;
    };
    
    using Point2 = Vector2;
    using Size2 = Vector2;
}
