//
//  UISlider.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 04/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../Math/Color.hpp"
#include "Widget.hpp"
#include "UIRespondsToMouseEvents.hpp"


namespace aha
{
    namespace ui
    {
        class UISlider : public Widget, public UIRespondsToMouseEvents <UISlider>
        {
        public:
            float getValue() const
            {
                return value_;
            }
            
            void setValue(float value)
            {
                value_ = value;
            }
            
            const Color& getHighlightColor() const
            {
                return highlightedColor_;
            }
            
            void setHighlightColor(const Color& highlightColor)
            {
                highlightedColor_ = highlightColor;
            }
            
            const std::pair<float, float>& getRange() const
            {
                return range_;
            }
            
            void setRange(const std::pair<float, float>& range)
            {
                range_ = range;
            }
            
            const std::pair<float, float>& getHighlightedRange() const
            {
                return highlightedRange_;
            }
            
            void setHighlightedRange(const std::pair<float, float>& highlightedRange)
            {
                highlightedRange_ = highlightedRange;
            }
            
            const std::function <void (float)>& getCallback() const
            {
                return callback_;
            }
            
            void setCallback(const std::function <void (float)>& callback)
            {
                callback_ = callback;
            }
            
            const std::function <void (float)>& getFinalCallback() const
            {
                return finalCallback_;
            }
            
            void setFinalCallback(const std::function <void (float)>& callback)
            {
                finalCallback_ = callback;
            }
            
            void onMouseLeftButtonDown(float x, float y)
            {
                value_ = (x - position_.x) / size_.width;
                value_ = value_ > 1.0f? 1.0f : value_;
                value_ = value_ < 0.0f? 0.0f : value_;
                if(callback_)
                {
                    callback_(value_);
                }
            }
            
            void onMouseLeftButtonDragged(float x, float y)
            {
                value_ = (x - position_.x) / size_.width;
                value_ = value_ > 1.0f? 1.0f : value_;
                value_ = value_ < 0.0f? 0.0f : value_;
                if(callback_)
                {
                    callback_(value_);
                }
            }
            
            void onMouseLeftButtonUp(float x, float y)
            {
                value_ = (x - position_.x) / size_.width;
                value_ = value_ > 1.0f? 1.0f : value_;
                value_ = value_ < 0.0f? 0.0f : value_;
                if(callback_)
                {
                    callback_(value_);
                }
                if(finalCallback_)
                {
                    finalCallback_(value_);
                }
            }
            
            virtual Vec2f getPreferredSize(NVGcontext* context) const override
            {
                return Vec2f(70.0f, 16.0f);
            }
            
            virtual void render(NVGcontext* context) const override
            {
                NVGpaint bg, knob;
                float cy = position_.y + (int) (size_.y * 0.5f);
                float kr = (int) (size_.width * 0.10f);
                
                nvgSave(context);
                
                // Slot
                bg = nvgBoxGradient(context, position_.x, cy - 2.0f + 1.0f, size_.x, 4.0f, 2.0f, 2.0f, Color(0, 0, 0, isEnabled_? 32 : 10), Color(0, 0, 0, isEnabled_? 128 : 210));
                nvgBeginPath(context);
                nvgRoundedRect(context, position_.x, cy - 2.0f, size_.width, 4.0f, 2.0f);
                nvgFillPaint(context, bg);
                nvgFill(context);
                
                NVGpaint knobShadow = nvgRadialGradient(context, position_.x + (int) (value_ * size_.x), cy + 1.0f, kr - 3.0f, kr + 3.0f, Color(0, 0, 0, 64), theme_->transparent);
                
                nvgBeginPath(context);
                nvgRect(context, position_.x + (int) (value_ * size_.x) - kr - 5.0f, cy - kr - 5.0f, kr * 2.0f + 5.0f + 5.0f, kr * 2.0f + 5.0f + 5.0f + 3.0f);
                nvgCircle(context, position_.x + (int) (value_ * size_.x), cy, kr);
                nvgPathWinding(context, NVG_HOLE);
                nvgFillPaint(context, knobShadow);
                nvgFill(context);
                
                knob = nvgLinearGradient(context, position_.x, cy - kr, position_.x, cy + kr, theme_->borderLight, theme_->borderMedium);
                NVGpaint knobReverse = nvgLinearGradient(context, position_.x, cy - kr, position_.x, cy + kr, theme_->borderMedium, theme_->borderLight);
                
                nvgBeginPath(context);
                nvgCircle(context, position_.x + (int) (value_ * size_.x), cy, kr - 1.0f);
                nvgStrokeColor(context, theme_->borderDark);
                nvgFillPaint(context, knob);
                nvgStroke(context);
                nvgFill(context);
                nvgBeginPath(context);
                nvgCircle(context, position_.x + (int) (value_ * size_.x), cy, (kr - 1.0f) / 2);
                nvgFillColor(context, Color(150, isEnabled_ ? 255 : 100));
                nvgStrokePaint(context, knobReverse);
                nvgStroke(context);
                nvgFill(context);
                
                nvgRestore(context);
                
                Widget::render(context);
            }
            
        protected:
            float value_{0.0f};
            std::pair<float, float> range_{0.0f, 1.0f};
            std::pair<float, float> highlightedRange_{0.0f, 0.0f};
            Color highlightedColor_{Color(255, 80, 80, 70)};
            std::function<void(float)> callback_;
            std::function<void(float)> finalCallback_;
        };
    }
}
