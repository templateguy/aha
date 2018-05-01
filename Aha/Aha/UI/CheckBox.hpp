//
//  CheckBox.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 30/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Label.hpp"


namespace aha
{
    namespace ui
    {
        class CheckBox : public UI
        {
        public:
            CheckBox(bool isChecked = false) : isChecked_(isChecked)
            {
                setSize(glm::vec2(28.0f, 28.0f));
            }
            
            bool isChecked() const
            {
                return isChecked_;
            }
            
            void setChecked(bool isChecked)
            {
                isChecked_ = isChecked;
            }
            
            void render(NVGcontext* vg) const override
            {
                const auto position(getPositionInWorldSpace());
                NVGpaint bg;
                char icon[8];
                
                bg = nvgBoxGradient(vg, position.x + 1.0f, position.y + (int) (size_.y * 0.5f) - 9.0f + 1.0f, 18.0f, 18.0f, 3.0f, 3.0f, nvgRGBA(0, 0, 0, 32), nvgRGBA(0, 0, 0, 92));
                nvgBeginPath(vg);
                nvgRoundedRect(vg, position.x + 1.0f, position.y + (int) (size_.y * 0.5f) - 9.0f, 18.0f, 18.0f, 3.0f);
                nvgFillPaint(vg, bg);
                nvgFill(vg);
                
                if(isChecked_)
                {
                    nvgFontSize(vg, 40.0f);
                    nvgFontFace(vg, "icons");
                    nvgFillColor(vg, nvgRGBA(255, 255, 255, 128));
                    nvgTextAlign(vg, NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
                    nvgText(vg, position.x + 9.0f + 2.0f, position.y + size_.y * 0.5f, cpToUTF8(ICON_CHECK, icon), NULL);
                }
            }
            
        protected:
            bool isChecked_{false};
        };
    }
}
