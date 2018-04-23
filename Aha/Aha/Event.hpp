//
//  Event.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 22/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <map>
#include <string>
#include <functional>
#include "Signal.hpp"
#include "Preprocessor/Preprocessor.h"


namespace aha
{
    template <typename IdentifierType>
    class Event_
    {
    public:
        template <typename FunctionSignature, typename HandleType = unsigned int>
        HandleType addListener(const IdentifierType& identifier, std::function <FunctionSignature> listener)
        {
            if(listener)
            {
                auto signal(Registry_::template fetch <FunctionSignature> (identifier));
                return signal.connect(listener);
            }
            return HandleType{};
        }
        
        template <typename FunctionSignature, typename HandleType = unsigned int>
        void removeListener(const IdentifierType& identifier, HandleType handle)
        {
            auto signal(Registry_::template fetch <FunctionSignature> (identifier));
            signal.disconnect(handle);
        }
        
        template <typename FunctionSignature, typename... Args>
        void fire(const IdentifierType& identifier, Args... args)
        {
            auto signal(Registry_::template fetch <FunctionSignature> (identifier));
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
