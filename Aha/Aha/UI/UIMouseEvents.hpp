//
//  UIMouseEvents.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 30/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Event.hpp"
#include "../Application.hpp"


namespace aha
{
    namespace ui
    {
        class UIMouseEvents_ final
        {
        public:
            using Handle = unsigned int;
            
            Handle addListener(const std::string& event, const std::function <bool (float, float)>& slot)
            {
                Handle handle(newHandle_());
                if(auto i(slots_.find(event)); i != slots_.end())
                {
                    i->second[handle] = slot;
                    return handle;
                }
                std::map <Handle, std::function <bool (float, float)>> slots;
                slots[handle] = slot;
                slots_[event] = slots;
                return handle;
            }
            
            void removeListener(const std::string& event, Handle handle)
            {
                if(auto i(slots_.find(event)); i != slots_.end())
                {
                    if(auto j(i->second.find(handle)); j != i->second.end())
                    {
                        i->second.erase(j);
                    }
                }
            }
            
            static UIMouseEvents_& getInstance()
            {
                static UIMouseEvents_ instance;
                return instance;
            }
            
            UIMouseEvents_(const UIMouseEvents_&) = delete;
            const UIMouseEvents_& operator =(const UIMouseEvents_&) = delete;
            
        private:
            UIMouseEvents_()
            {
                onMouseEnteredHandle_ = Event.addListener <void (float, float)> ("MouseEntered", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseEntered", std::placeholders::_1, std::placeholders::_2));
                onMouseExitedHandle_ = Event.addListener <void (float, float)> ("MouseExited", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseExited",  std::placeholders::_1, std::placeholders::_2));
                onMouseMovedHandle_ = Event.addListener <void (float, float)> ("MouseMoved", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseMoved",  std::placeholders::_1, std::placeholders::_2));
                onMouseLeftButtonDownHandle_ = Event.addListener <void (float, float)> ("MouseLeftButtonDown", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseLeftButtonDown", std::placeholders::_1, std::placeholders::_2));
                onMouseLeftButtonDraggedHandle_ = Event.addListener <void (float, float)> ("MouseLeftButtonDragged", std::bind(&UIMouseEvents_::onMouseEvent_, this,  "MouseLeftButtonDragged", std::placeholders::_1, std::placeholders::_2));
                onMouseLeftButtonUpHandle_ = Event.addListener <void (float, float)> ("MouseLeftButtonUp", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseLeftButtonUp", std::placeholders::_1, std::placeholders::_2));
                onMouseRightButtonDownHandle_ = Event.addListener <void (float, float)> ("MouseRightButtonDown", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseRightButtonDown", std::placeholders::_1, std::placeholders::_2));
                onMouseRightButtonDraggedHandle_ = Event.addListener <void (float, float)> ("MouseRightButtonDragged", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseRightButtonDragged", std::placeholders::_1, std::placeholders::_2));
                onMouseRightButtonUpHandle_ = Event.addListener <void (float, float)> ("MouseRightButtonUp", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseRightButtonUp", std::placeholders::_1, std::placeholders::_2));
                onMouseMiddleButtonDownHandle_ = Event.addListener <void (float, float)> ("MouseMiddleButtonDown", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseMiddleButtonDown", std::placeholders::_1, std::placeholders::_2));
                onMouseMiddleButtonDraggedHandle_ = Event.addListener <void (float, float)> ("MouseMiddleButtonDragged", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseMiddleButtonDragged", std::placeholders::_1, std::placeholders::_2));
                onMouseMiddleButtonUpHandle_ = Event.addListener <void (float, float)> ("MouseMiddleButtonUp", std::bind(&UIMouseEvents_::onMouseEvent_, this, "MouseMiddleButtonUp", std::placeholders::_1, std::placeholders::_2));
                onMouseWheelScrolledHandle_ = Event.addListener <void (float, float)> ("MouseWheelScrolled", std::bind(&UIMouseEvents_::fire_, this, "MouseWheelScrolled", std::placeholders::_1, std::placeholders::_2));
            }
            
            ~UIMouseEvents_()
            {
                Event.removeListener <void (float, float)> ("MouseEntered", onMouseEnteredHandle_);
                Event.removeListener <void (float, float)> ("MouseExited", onMouseExitedHandle_);
                Event.removeListener <void (float, float)> ("MouseMoved", onMouseMovedHandle_);
                Event.removeListener <void (float, float)> ("MouseLeftButtonDown", onMouseLeftButtonDownHandle_);
                Event.removeListener <void (float, float)> ("MouseLeftButtonDragged", onMouseLeftButtonDraggedHandle_);
                Event.removeListener <void (float, float)> ("MouseLeftButtonUp", onMouseLeftButtonUpHandle_);
                Event.removeListener <void (float, float)> ("MouseRightButtonDown", onMouseRightButtonDownHandle_);
                Event.removeListener <void (float, float)> ("MouseRightButtonDragged", onMouseRightButtonDraggedHandle_);
                Event.removeListener <void (float, float)> ("MouseRightButtonUp", onMouseRightButtonUpHandle_);
                Event.removeListener <void (float, float)> ("MouseMiddleButtonDown", onMouseMiddleButtonDownHandle_);
                Event.removeListener <void (float, float)> ("MouseMiddleButtonDragged", onMouseMiddleButtonDraggedHandle_);
                Event.removeListener <void (float, float)> ("MouseMiddleButtonUp", onMouseMiddleButtonUpHandle_);
                Event.removeListener <void (float, float)> ("MouseWheelScrolled", onMouseWheelScrolledHandle_);
            }
            
            void onMouseEvent_(const std::string& event, float x, float y)
            {
                const float height(Application.getWindowHeight());
                fire_(event, x, height - y);
            }
            
            void fire_(const std::string& event, float x, float y)
            {
                if(auto i(slots_.find(event)); i != slots_.end())
                    if(i->second.size())
                        for(auto ri(i->second.rbegin()); ri != i->second.rend(); ++ri)
                            if(ri->second)
                                if(ri->second(x, y))
                                    return;
            }
            
            static Handle newHandle_()
            {
                static Handle handle;
                return ++handle;
            }
            
            Handle onMouseEnteredHandle_;
            Handle onMouseExitedHandle_;
            Handle onMouseMovedHandle_;
            Handle onMouseLeftButtonDownHandle_;
            Handle onMouseLeftButtonDraggedHandle_;
            Handle onMouseLeftButtonUpHandle_;
            Handle onMouseRightButtonDownHandle_;
            Handle onMouseRightButtonDraggedHandle_;
            Handle onMouseRightButtonUpHandle_;
            Handle onMouseMiddleButtonDownHandle_;
            Handle onMouseMiddleButtonDraggedHandle_;
            Handle onMouseMiddleButtonUpHandle_;
            Handle onMouseWheelScrolledHandle_;
            
            std::map <std::string, std::map <Handle, std::function <bool (float, float)>>> slots_;
        };
    }
}
#define UIMouseEvents UIMouseEvents_::getInstance()
