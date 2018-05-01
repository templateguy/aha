//
//  TitleBar.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 29/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <string>
#include "../nanovg/nanovg.h"
#include "Label.hpp"
#include "RespondsToUIMouseEvents.hpp"


namespace aha
{
    namespace ui
    {
        template <typename Parent>
        class TitleBar : public UI, public RespondsToUIMouseEvents <TitleBar <Parent>>
        {
        public:
            TitleBar(const std::string& title = "", float cornerRadius = 3.0f) : title_(std::make_shared <Label> (title)), cornerRadius_(cornerRadius)
            {
                size_.y = 30.0f;
                addChild(title_);
                title_->setParent(this);
                title_->setColor(glm::vec4(0.9f, 0.9f, 0.9f, 0.8f));
            }
            
            const std::string& getTitleText() const
            {
                return title_;
            }
            
            void setTitleText(const std::string& title)
            {
                title_ = title;
            }
            
            void setSize(const glm::vec2& size)
            {
                size_ = size;
                title_->setSize(size);
            }
            
            void render(NVGcontext* vg) const override
            {
                const auto position(getPositionInWorldSpace());
                // Header
                NVGpaint headerPaint(nvgLinearGradient(vg, position.x, position.y, position.x, position.y + 15.0f, nvgRGBA(255, 255, 255, 8), nvgRGBA(0, 0, 0, 16)));
                nvgBeginPath(vg);
                nvgRoundedRect(vg, position.x + 1.0f, position.y + 1.0f, size_.x - 2.0f, 30.0f, cornerRadius_ - 1.0f);
                nvgFillPaint(vg, headerPaint);
                nvgFill(vg);
                nvgBeginPath(vg);
                nvgMoveTo(vg, position.x + 0.5f, position.y + 0.5f + 30.0f);
                nvgLineTo(vg, position.x + 0.5f + size_.x - 1.0f, position.y + 0.5f + 30.0f);
                nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
                nvgStroke(vg);
                
                title_->render(vg);
            }
            
            void onMouseLeftButtonDown(float x, float y)
            {
                mouseClickStartPosition_ = glm::vec2(x, y);
                auto self(static_cast <Parent*> (parent_));
                selfStartPosition_ = self->getPosition();
            }
            
            void onMouseLeftButtonDragged(float x, float y)
            {
                glm::vec2 delta(x - mouseClickStartPosition_.x, y - mouseClickStartPosition_.y);
                auto self(static_cast <Parent*> (parent_));
                self->setPosition(selfStartPosition_ + delta);
            }
            
        protected:
            std::shared_ptr <Label> title_;
            float cornerRadius_{3.0f};
            glm::vec2 mouseClickStartPosition_;
            glm::vec2 selfStartPosition_;
        };
    }
}
