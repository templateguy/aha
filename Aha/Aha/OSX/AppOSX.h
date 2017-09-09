//
//  AppOSX.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <memory>


namespace aha
{
    class ApplicationState_;
    
    
    class AppOSX
    {
    public:
        AppOSX();
        ~AppOSX();
        void run();
        
    private:
        std::shared_ptr <ApplicationState_> state_;
    };
}
