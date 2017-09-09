//
//  WindowOSX.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <memory>


namespace aha
{
    class WindowOSX
    {
    public:
        WindowOSX();
        ~WindowOSX();
        
    private:
        struct impl_;
        std::unique_ptr <impl_> pImpl_;
    };
}
