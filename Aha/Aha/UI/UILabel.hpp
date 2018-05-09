//
//  UILabel.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 03/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Widget.hpp"


namespace aha
{
    namespace ui
    {
        class UILabel : public Widget
        {
        public:
            UILabel(const std::string& caption, const std::string& font = "sans", int fontSize = -1) : caption_(caption), font_(font)
            {
                type_ = "UILabel";
                
                if(theme_)
                {
                    fontSize_ = theme_->standardFontSize;
                    color_ = theme_->textColor;
                }
                if(fontSize >= 0)
                {
                    fontSize_ = fontSize;
                }
            }
            
            const std::string& getCaption() const
            {
                return caption_;
            }
            
            void setCaption(const std::string& caption)
            {
                caption_ = caption;
            }
            
            const std::string& getFont() const
            {
                return font_;
            }
            
            void setFont(const std::string& font)
            {
                font_ = font;
            }
            
            Color getColor() const
            {
                return color_;
            }
            
            void setColor(const Color& color)
            {
                color_ = color;
                hasCustomColor_ = true;
            }
            
            void resetColor()
            {
                if(theme_)
                {
                    color_ = theme_->textColor;
                }
                hasCustomColor_ = false;
            }
            
            virtual void setTheme(const std::shared_ptr <Theme>& theme) override
            {
                Widget::setTheme(theme);
                if(theme_)
                {
                    if(!fontSize_ || fontSize_ < -1)
                    {
                        fontSize_ = theme_->standardFontSize;
                    }
                    if(!hasCustomColor_)
                    {
                        color_ = theme_->textColor;
                    }
                }
            }
            
            virtual Size2f getPreferredSize(NVGcontext* context) const override
            {
                if(caption_ == "")
                {
                    return Size2f();
                }
                nvgFontFace(context, font_.c_str());
                nvgFontSize(context, fontSize_);
                if(fixedSize_.width > 0.0f)
                {
                    float bounds[4];
                    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
                    nvgTextBoxBounds(context, position_.x, position_.y, fixedSize_.width, caption_.c_str(), nullptr, bounds);
                    return Size2f(fixedSize_.width, bounds[3] - bounds[1]);
                }
                else
                {
                    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    return Size2f(nvgTextBounds(context, 0, 0, caption_.c_str(), nullptr, nullptr) + 2, fontSize_);
                }
            }
            
            virtual void render(NVGcontext* context) const override
            {
                nvgFontFace(context, font_.c_str());
                nvgFontSize(context, fontSize_);
                nvgFillColor(context, color_);
                if(fixedSize_.width > 0)
                {
                    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
                    nvgTextBox(context, position_.x, position_.y, fixedSize_.width, caption_.c_str(), nullptr);
                }
                else
                {
                    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    nvgText(context, position_.x, position_.y + size_.height * 0.5f, caption_.c_str(), nullptr);
                }
                Widget::render(context);
            }
            
        protected:
            std::string caption_;
            std::string font_;
            Color color_;
            bool hasCustomColor_{false};
        };
    }
}
