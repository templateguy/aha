//
//  Registry.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 25/02/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    template <typename T>
    class Registry
    {
    protected:
        bool add(std::shared_ptr <T> object)
        {
            if(object)
            {
                auto i(std::find_if(std::begin(registry_), std::end(registry_), [&] (const std::shared_ptr <T>& obj) {
                    return object->getID() == obj->getID();
                }));
                if(i != std::end(registry_))
                {
                    registry_.emplace_back(object);
                    return true;
                }
            }
            return false;
        }
        
        bool remove(std::shared_ptr <T> object)
        {
            if(object)
            {
                auto i(std::find_if(std::begin(registry_), std::end(registry_), [&] (const std::shared_ptr <T>& obj) {
                    return object->getID() == obj->getID();
                }));
                if(i != std::end(registry_))
                {
                    registry_.erase(i);
                    return true;
                }
            }
            return false;
        }
        
        std::vector <std::shared_ptr <T>> registry_;
    };
}
