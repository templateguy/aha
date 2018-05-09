//
//  UICheckBox.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 30/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Widget.hpp"
#include "UIRespondsToMouseEvents.hpp"


namespace aha
{
    namespace ui
    {
        class UICheckBox : public Widget, public UIRespondsToMouseEvents <UICheckBox>
        {
        public:
            UICheckBox(const std::shared_ptr <Widget>& object = {}, bool isChecked = false) : object_(object), isChecked_(isChecked)
            {
                
            }
            
            bool isChecked() const
            {
                return isChecked_;
            }
            
            void setChecked(bool isChecked)
            {
                isChecked_ = isChecked;
            }
            
            void onMouseLeftButtonUp(float x, float y)
            {
                isChecked_ = !isChecked_;
            }
            
            virtual Vec2f getPreferredSize(NVGcontext* context) const override
            {
                Vec2f size(checkboxWidth_, checkboxHeight_);
                if(object_)
                {
                    const auto objectSize(object_->getPreferredSize(context));
                    size.width = checkboxWidth_ + spacing_ + objectSize.width;
                    if(objectSize.height > checkboxHeight_)
                    {
                        size.height = objectSize.height;
                    }
                }
                return size;
            }
            
            virtual void performLayout(NVGcontext* context) override
            {
                if(object_)
                {
                    auto objectSize(object_->getPreferredSize(context));
                    objectSize.height = objectSize.height < checkboxHeight_? checkboxHeight_ : objectSize.height;
                    object_->setSize(objectSize);
                    object_->setPosition(Vec2f(checkboxWidth_ + spacing_, 0.0f));
                }
                Widget::performLayout(context);
            }
            
            virtual void render(NVGcontext* context) const override
            {
                auto position(position_);
                if(object_)
                {
                    nvgSave(context);
                    nvgTranslate(context, position_.x, position_.y);
                    object_->render(context);
                    nvgRestore(context);
                }
                
                NVGpaint bg;
                char icon[8];
                
                bg = nvgBoxGradient(context, position.x + 1.0f, position.y + (int) (size_.y * 0.5f) - 9.0f + 1.0f, 18.0f, 18.0f, 3.0f, 3.0f, nvgRGBA(0, 0, 0, 32), nvgRGBA(0, 0, 0, 92));
                nvgBeginPath(context);
                nvgRoundedRect(context, position.x + 1.0f, position.y + (int) (size_.y * 0.5f) - 9.0f, 18.0f, 18.0f, 3.0f);
                nvgFillPaint(context, bg);
                nvgFill(context);
                
                if(isChecked_)
                {
                    nvgFontSize(context, 40.0f);
                    nvgFontFace(context, "icons");
                    nvgFillColor(context, nvgRGBA(255, 255, 255, 128));
                    nvgTextAlign(context, NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
                    nvgText(context, position.x + 9.0f + 2.0f, position.y + size_.y * 0.5f, cpToUTF8(ICON_CHECK, icon), NULL);
                }
                Widget::render(context);
            }
            
        protected:
            bool isChecked_{false};
            std::shared_ptr <Widget> object_{};
            float spacing_{6.0f};
            float checkboxWidth_{28.0f};
            float checkboxHeight_{28.0f};
        };
    }
}

