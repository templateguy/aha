//
//  Timer.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 24/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    class Timer
    {
    public:
        Timer() : start_(clock_::now())
        {
            ;
        }
        
        void reset()
        {
            start_ = clock_::now();
        }
        
        double elapsed() const
        {
            return std::chrono::duration_cast<second_> (clock_::now() - start_).count();
        }
        
    private:
        using clock_ = std::chrono::high_resolution_clock;
        using second_ = std::chrono::duration<double, std::ratio<1> >;
        std::chrono::time_point<clock_> start_;
    };
}
