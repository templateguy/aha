//
//  Event.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 22/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <string>
#include "Signal.hpp"
#include "Preprocessor/Preprocessor.h"


namespace aha
{
    template <typename IdentifierType>
    class Event_
    {
    public:
        template <typename FunctionSignature>
        auto addListener(const IdentifierType& identifier, const std::function <FunctionSignature>& listener)
        {
            auto& signal(Registry_::template fetch <FunctionSignature> (identifier));
            return signal.connect(listener);
        }
        
        template <typename FunctionSignature>
        void removeListener(const IdentifierType& identifier, const typename Signal <FunctionSignature>::Handle& handle)
        {
            auto& signal(Registry_::template fetch <FunctionSignature> (identifier));
            signal.disconnect(handle);
        }
        
        template <typename FunctionSignature, typename... Args>
        void fire(const IdentifierType& identifier, Args... args)
        {
            auto& signal(Registry_::template fetch <FunctionSignature> (identifier));
            signal(args...);
        }
        
    protected:
        class Registry_
        {
        public:
            template <typename FunctionSignature>
            static auto& fetch(const IdentifierType& identifier)
            {
                static std::map <IdentifierType, aha::Signal <FunctionSignature>> registry;
                auto i(registry.begin());
                i = registry.find(identifier);
                if(i != registry.end())
                {
                    return i->second;
                }
                registry[identifier] = aha::Signal <FunctionSignature> ();
                return registry[identifier];
            }
        };
        
        AHA_MAKE_SINGLETON(Event_<std::string>);
    };
#define Event Event_ <std::string>::getInstance()
}
