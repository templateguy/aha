//
//  Slider.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 30/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "../nanovg/nanovg.h"
#include "UI.hpp"
#include "RespondsToUIMouseEvents.hpp"


namespace aha
{
    namespace ui
    {
        class Slider : public UI, public RespondsToUIMouseEvents <Slider>
        {
        public:
            Slider(float value = 0.0f, const glm::vec2& size = glm::vec2(150.0f, 30.0f)) : value_(value)
            {
                setSize(size);
            }
            
            float getValue() const
            {
                return value_;
            }
            
            void setValue(float value)
            {
                value_ = value;
                value_ = value_ < 0.0f? 0.0f : value_;
                value_ = value_ > 1.0f? 1.0f : value_;
            }
            
            void render(NVGcontext* vg) const override
            {
                const auto position(getPositionInWorldSpace());
                NVGpaint bg, knob;
                float cy = position.y + (int) (size_.y * 0.5f);
                float kr = (int) (size_.y * 0.25f);
                
                nvgSave(vg);
                
                // Slot
                bg = nvgBoxGradient(vg, position.x, cy - 2.0f + 1.0f, size_.x, 4.0f, 2.0f, 2.0f, nvgRGBA(0, 0, 0, 32), nvgRGBA(0, 0, 0, 128));
                nvgBeginPath(vg);
                nvgRoundedRect(vg, position.x, cy - 2.0f, size_.x, 4.0f, 2.0f);
                nvgFillPaint(vg, bg);
                nvgFill(vg);
                
                // Knob Shadow
                bg = nvgRadialGradient(vg, position.x + (int) (value_ * size_.x), cy + 1.0f, kr - 3.0f, kr + 3.0f, nvgRGBA(0, 0, 0, 64), nvgRGBA(0, 0, 0, 0));
                nvgBeginPath(vg);
                nvgRect(vg, position.x + (int) (value_ * size_.x) - kr - 5.0f, cy - kr - 5.0f, kr * 2.0f + 5.0f + 5.0f, kr * 2.0f + 5.0f + 5.0f + 3.0f);
                nvgCircle(vg, position.x + (int) (value_ * size_.x), cy, kr);
                nvgPathWinding(vg, NVG_HOLE);
                nvgFillPaint(vg, bg);
                nvgFill(vg);
                
                // Knob
                knob = nvgLinearGradient(vg, position.x, cy - kr, position.x, cy + kr, nvgRGBA(255, 255, 255, 16), nvgRGBA(0, 0, 0, 16));
                nvgBeginPath(vg);
                nvgCircle(vg, position.x + (int) (value_ * size_.x), cy, kr - 1.0f);
                nvgFillColor(vg, nvgRGBA(40, 43, 48, 255));
                nvgFill(vg);
                nvgFillPaint(vg, knob);
                nvgFill(vg);
                
                nvgBeginPath(vg);
                nvgCircle(vg, position.x + (int) (value_ * size_.x), cy, kr - 0.5f);
                nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 92));
                nvgStroke(vg);
                
                nvgRestore(vg);
            }
            
            void onMouseLeftButtonDown(float x, float y)
            {
                mouseClickStartPosition_ = glm::vec2(x, y);
                selfStartPosition_ = position_;
            }
            
            void onMouseLeftButtonDragged(float x, float y)
            {
                glm::vec2 delta(x - mouseClickStartPosition_.x, y - mouseClickStartPosition_.y);
                setPosition(selfStartPosition_ + delta);
            }
            
        protected:
            float value_{0.0f};
            glm::vec2 mouseClickStartPosition_;
            glm::vec2 selfStartPosition_;
        };
    }
}
