//
//  UIRespondsToMouseEvents.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 30/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Math/Vec2.hpp"
#include "UIMouseEvents.hpp"


namespace aha
{
    namespace ui
    {
        // This class assumes that Parent would have position, size and getPositionInWorldSpace() functionalities
        template <typename Parent>
        class UIRespondsToMouseEvents
        {
        protected:
            UIRespondsToMouseEvents()
            {
                onMouseEnteredHandle_ = UIMouseEvents.addListener("MouseEntered", std::bind(&UIRespondsToMouseEvents::onMouseEntered_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseExitedHandle_ = UIMouseEvents.addListener("MouseExited", std::bind(&UIRespondsToMouseEvents::onMouseExited_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseMovedHandle_ = UIMouseEvents.addListener("MouseMoved", std::bind(&UIRespondsToMouseEvents::onMouseMoved_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseLeftButtonDownHandle_ = UIMouseEvents.addListener("MouseLeftButtonDown", std::bind(&UIRespondsToMouseEvents::onMouseLeftButtonDown_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseLeftButtonDraggedHandle_ = UIMouseEvents.addListener("MouseLeftButtonDragged", std::bind(&UIRespondsToMouseEvents::onMouseLeftButtonDragged_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseLeftButtonUpHandle_ = UIMouseEvents.addListener("MouseLeftButtonUp", std::bind(&UIRespondsToMouseEvents::onMouseLeftButtonUp_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseRightButtonDownHandle_ = UIMouseEvents.addListener("MouseRightButtonDown", std::bind(&UIRespondsToMouseEvents::onMouseRightButtonDown_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseRightButtonDraggedHandle_ = UIMouseEvents.addListener("MouseRightButtonDragged", std::bind(&UIRespondsToMouseEvents::onMouseRightButtonDragged_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseRightButtonUpHandle_ = UIMouseEvents.addListener("MouseRightButtonUp", std::bind(&UIRespondsToMouseEvents::onMouseRightButtonUp_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseMiddleButtonDownHandle_ = UIMouseEvents.addListener("MouseMiddleButtonDown", std::bind(&UIRespondsToMouseEvents::onMouseMiddleButtonDown_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseMiddleButtonDraggedHandle_ = UIMouseEvents.addListener("MouseMiddleButtonDragged", std::bind(&UIRespondsToMouseEvents::onMouseMiddleButtonDragged_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseMiddleButtonUpHandle_ = UIMouseEvents.addListener("MouseMiddleButtonUp", std::bind(&UIRespondsToMouseEvents::onMouseMiddleButtonUp_, this, std::placeholders::_1, std::placeholders::_2));
                onMouseWheelScrolledHandle_ = UIMouseEvents.addListener("MouseWheelScrolled", std::bind(&UIRespondsToMouseEvents::onMouseWheelScrolled_, this, std::placeholders::_1, std::placeholders::_2));
            }
            
            virtual ~UIRespondsToMouseEvents()
            {
                UIMouseEvents.removeListener("MouseEntered", onMouseEnteredHandle_);
                UIMouseEvents.removeListener("MouseExited", onMouseExitedHandle_);
                UIMouseEvents.removeListener("MouseMoved", onMouseMovedHandle_);
                UIMouseEvents.removeListener("MouseLeftButtonDown", onMouseLeftButtonDownHandle_);
                UIMouseEvents.removeListener("MouseLeftButtonDragged", onMouseLeftButtonDraggedHandle_);
                UIMouseEvents.removeListener("MouseLeftButtonUp", onMouseLeftButtonUpHandle_);
                UIMouseEvents.removeListener("MouseRightButtonDown", onMouseRightButtonDownHandle_);
                UIMouseEvents.removeListener("MouseRightButtonDragged", onMouseRightButtonDraggedHandle_);
                UIMouseEvents.removeListener("MouseRightButtonUp", onMouseRightButtonUpHandle_);
                UIMouseEvents.removeListener("MouseMiddleButtonDown", onMouseMiddleButtonDownHandle_);
                UIMouseEvents.removeListener("MouseMiddleButtonDragged", onMouseMiddleButtonDraggedHandle_);
                UIMouseEvents.removeListener("MouseMiddleButtonUp", onMouseMiddleButtonUpHandle_);
                UIMouseEvents.removeListener("MouseWheelScrolled", onMouseWheelScrolledHandle_);
            }
            
            void onMouseEntered(float x, float y)
            {
                
            }
            
            void onMouseExited(float x, float y)
            {
                
            }
            
            void onMouseMoved(float x, float y)
            {
                
            }
            
            void onMouseLeftButtonDown(float x, float y)
            {
                
            }
            
            void onMouseLeftButtonDragged(float x, float y)
            {
                
            }
            
            void onMouseLeftButtonUp(float x, float y)
            {
                
            }
            
            void onMouseRightButtonDown(float x, float y)
            {
                
            }
            
            void onMouseRightButtonDragged(float x, float y)
            {
                
            }
            
            void onMouseRightButtonUp(float x, float y)
            {
                
            }
            
            void onMouseMiddleButtonDown(float x, float y)
            {
                
            }
            
            void onMouseMiddleButtonDragged(float x, float y)
            {
                
            }
            
            void onMouseMiddleButtonUp(float x, float y)
            {
                
            }
            
            void onMouseWheelScrolled(float deltaX, float deltaY)
            {
                
            }
            
        protected:
            using Handle_ = unsigned int;
            
            bool onMouseEntered_(float x, float y)
            {
                auto self(static_cast <Parent*> (this));
                Vec2f position(self->convertToViewSpace(Vec2f(x, y)));
                if(self->isViewSpaceLocationInside(position))
                {
                    self->onMouseEntered(position.x, position.y);
                    return true;
                }
                return false;
            }
            
            bool onMouseExited_(float x, float y)
            {
                auto self(static_cast <Parent*> (this));
                Vec2f position(self->convertToViewSpace(Vec2f(x, y)));
                if(self->isViewSpaceLocationInside(position))
                {
                    self->onMouseExited(position.x, position.y);
                    return true;
                }
                return false;
            }
            
            bool onMouseMoved_(float x, float y)
            {
                auto self(static_cast <Parent*> (this));
                Vec2f position(self->convertToParentSpace(Vec2f(x, y)));
                if(self->isParentSpaceLocationInside(position))
                {
                    self->onMouseMoved(position.x, position.y);
                    return true;
                }
                return false;
            }
            
            bool onMouseLeftButtonDown_(float x, float y)
            {
                auto self(static_cast <Parent*> (this));
                Vec2f position(self->convertToParentSpace(Vec2f(x, y)));
                if(self->isParentSpaceLocationInside(position))
                {
                    mouseLeftButtonDownFlag_ = true;
                    self->onMouseLeftButtonDown(position.x, position.y);
                    return true;
                }
                return false;
            }
            
            bool onMouseLeftButtonDragged_(float x, float y)
            {
                auto self(static_cast <Parent*> (this));
                if(mouseLeftButtonDownFlag_)
                {
                    Vec2f position(self->convertToParentSpace(Vec2f(x, y)));
                    self->onMouseLeftButtonDragged(position.x, position.y);
                    return true;
                }
                return false;
            }
            
            bool onMouseLeftButtonUp_(float x, float y)
            {
                auto self(static_cast <Parent*> (this));
                if(mouseLeftButtonDownFlag_)
                {
                    Vec2f position(self->convertToParentSpace(Vec2f(x, y)));
                    self->onMouseLeftButtonUp(position.x, position.y);
                    mouseLeftButtonDownFlag_ = false;
                    return true;
                }
                return false;
            }
            
            bool onMouseRightButtonDown_(float x, float y)
            {
                return false;
            }
            
            bool onMouseRightButtonDragged_(float x, float y)
            {
                return false;
            }
            
            bool onMouseRightButtonUp_(float x, float y)
            {
                return false;
            }
            
            bool onMouseMiddleButtonDown_(float x, float y)
            {
                return false;
            }
            
            bool onMouseMiddleButtonDragged_(float x, float y)
            {
                return false;
            }
            
            bool onMouseMiddleButtonUp_(float x, float y)
            {
                return false;
            }
            
            bool onMouseWheelScrolled_(float deltaX, float deltaY)
            {
                return false;
            }
            
            Handle_ onMouseEnteredHandle_;
            Handle_ onMouseExitedHandle_;
            Handle_ onMouseMovedHandle_;
            Handle_ onMouseLeftButtonDownHandle_;
            Handle_ onMouseLeftButtonDraggedHandle_;
            Handle_ onMouseLeftButtonUpHandle_;
            Handle_ onMouseRightButtonDownHandle_;
            Handle_ onMouseRightButtonDraggedHandle_;
            Handle_ onMouseRightButtonUpHandle_;
            Handle_ onMouseMiddleButtonDownHandle_;
            Handle_ onMouseMiddleButtonDraggedHandle_;
            Handle_ onMouseMiddleButtonUpHandle_;
            Handle_ onMouseWheelScrolledHandle_;
            
            bool mouseLeftButtonDownFlag_{false};
        };
    }
}
