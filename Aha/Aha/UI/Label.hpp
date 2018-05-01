//
//  Label.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 29/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <string>
#include "../nanovg/nanovg.h"
#include "../Policies/HasColor.hpp"
#include "UI.hpp"


namespace aha
{
    namespace ui
    {
        class Label : public UI, public HasColor <glm::vec4>
        {
        public:
            Label(const std::string& text = "Label") : text_(text)
            {
                ;
            }
            
            const std::string& getText() const
            {
                return text_;
            }
            
            void setText(const std::string& text)
            {
                text_ = text;
            }
            
            float getFontSize() const
            {
                return fontSize_;
            }
            
            void setFontSize(float fontSize)
            {
                fontSize_ = fontSize;
            }
            
            float getFontBlur() const
            {
                return blur_;
            }
            
            void setFontBlur(float blur)
            {
                blur_ = blur;
            }
            
            void render(NVGcontext* vg) const override
            {
                const auto position(getPositionInWorldSpace());
                nvgFontSize(vg, fontSize_);
                nvgFontFace(vg, "sans");
                
                // Render shadow
                nvgFontBlur(vg, 2.0f);
                nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
                nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
                nvgText(vg, position.x + 1.0f + size_.x / 2.0f, position.y + 1.0f + size_.y * 0.5f, text_.c_str(), NULL);
                
                nvgFontBlur(vg, blur_);
                nvgFillColor(vg, nvgRGBA(static_cast <unsigned char> (color_.x * 255.0f), static_cast <unsigned char> (color_.y * 255.0f), static_cast <unsigned char> (color_.z * 255.0f), static_cast <unsigned char> (color_.w * 255.0f)));
                nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
                nvgText(vg, position.x + size_.x / 2.0f, position.y + size_.y * 0.5f, text_.c_str(), NULL);
            }
            
        protected:
            std::string text_{"Label"};
            float fontSize_{18.0f};
            float blur_{0.0f};
        };
    }
}
