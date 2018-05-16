//
//  UIPanel.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 05/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Widget.hpp"


namespace aha
{
    namespace ui
    {
        class UIPanel : public Widget
        {
        public:
            virtual void render(NVGcontext* context) const override
            {
                nvgSave(context);
                nvgStrokeWidth(context, 1.0f);
                nvgBeginPath(context);
                nvgRoundedRect(context, position_.x - 0.5f, position_.y - 0.5f, size_.width + 1.0f, size_.y + 1.0f, 3.0f);
                nvgStrokeColor(context, nvgRGBA(0, 0, 0, 64));
                nvgStroke(context);
                
                nvgBeginPath(context);
                nvgRoundedRect(context, position_.x, position_.y, size_.width, size_.height, 3.0f);
                nvgFillColor(context, nvgRGBA(32, 32, 32, 100));
                nvgFill(context);
                nvgRestore(context);
                Widget::render(context);
            }
        };
    }
}
