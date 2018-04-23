//
//  Signal.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 22/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <functional>
#include <map>


namespace aha
{
    template <typename FunctionSignature, typename DelegateType = std::function <FunctionSignature>, typename HandleType = unsigned int>
    class Signal;
    
    template <typename DelegateType, typename HandleType, typename ReturnType, typename... Args>
    class Signal <ReturnType (Args...), DelegateType, HandleType>
    {
    public:
        using Handle = HandleType;
        
        auto connect(const DelegateType& slot)
        {
            if(slot)
            {
                auto handle(newHandle_());
                slots_[handle] = slot;
                return handle;
            }
            return HandleType{};
        }
        
        void disconnect(const HandleType& handle)
        {
            auto i(slots_.begin());
            i = slots_.find(handle);
            if(i != slots_.end())
            {
                slots_.erase(i);
            }
        }
        
        void operator ()(Args... args)
        {
            for(auto slot : slots_)
            {
                slot.second(args...);
            }
        }
        
    protected:
        static auto newHandle_()
        {
            static HandleType handle;
            return ++handle;
        }
        
        std::map <HandleType, DelegateType> slots_{};
    };
}
