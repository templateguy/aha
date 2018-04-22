//
//  Scene.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 08/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#define AHA_CREATE_FUNC(class)                                      \
static std::shared_ptr <class> create()                             \
{                                                                   \
    std::shared_ptr <class> object(std::make_shared <class> ());    \
    if(object && object->init())                                    \
    {                                                               \
        return object;                                              \
    }                                                               \
    return std::shared_ptr <class> ();                              \
}

namespace aha
{
    class Scene
    {
    public:
        virtual void render() = 0;
        
    protected:
        virtual bool init() = 0;
    };
}
