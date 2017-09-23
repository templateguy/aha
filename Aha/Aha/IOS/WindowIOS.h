//
//  WindowIOS.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <memory>


namespace aha
{
    class WindowIOS
    {
    public:
        WindowIOS();
        ~WindowIOS();
        
    protected:
        struct impl_;
        std::unique_ptr <impl_> pimpl_;
    };
}
