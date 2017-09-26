//
//  Window.h
//  Aha
//
//  Created by Saurabh Sinha on 09/09/17.
//  Copyright © 2017 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <type_traits>
#include "Preprocessor/Platform.h"
#include "Config/Platform.hpp"

// Platform specific inclusion. See Preprocessor/Platform.h
#include AHA_PLATFORM_SPECIFIC(Window, .h)


// Forward declares platform specific classes. See Preprocesor/Platform.h
AHA_PLATFORM_SPECIFIC_FORWARD_DECLARATIONS(Window);


namespace aha
{
    // AHA_PLATFORM_POLICY(class) expands to the platform specific version of the class. See Preprocessor/Platform.h
    using WindowPlatformPolicy = AHA_PLATFORM_POLICY(Window);
    
    
    class Window : public WindowPlatformPolicy
    {
        ;
    };
}
