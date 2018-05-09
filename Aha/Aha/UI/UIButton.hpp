//
//  UIButton.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 05/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Widget.hpp"
#include "UIRespondsToMouseEvents.hpp"


namespace aha
{
    namespace ui
    {
        class UIButton : public Widget, public UIRespondsToMouseEvents <UIButton>
        {
        public:
            enum class IconPosition
            {
                Left,
                LeftCentered,
                RightCentered,
                Right
            };
            
            UIButton(const std::string& caption = "Button", int icon = 0) : caption_(caption), icon_(icon), iconPosition_(IconPosition::LeftCentered), isPushed_(false), backgroundColor_(Color(0.0f, 0.0f, 0.0f, 0.0f)), textColor_(Color(0.0f, 0.0f, 0.0f, 0.0f))
            {
                ;
            }
            
            const std::string& getCaption() const
            {
                return caption_;
            }
            
            void setCaption(const std::string& caption)
            {
                caption_ = caption;
            }
            
            const Color& getBackgroundColor() const
            {
                return backgroundColor_;
            }
            
            void setBackgroundColor(const Color& backgroundColor)
            {
                backgroundColor_ = backgroundColor;
            }
            
            const Color& getTextColor() const
            {
                return textColor_;
            }
            
            void setTextColor(const Color& textColor)
            {
                textColor_ = textColor;
            }
            
            int getIcon() const
            {
                return icon_;
            }
            
            void setIcon(int icon)
            {
                icon_ = icon;
            }
            
            IconPosition getIconPosition() const
            {
                return iconPosition_;
            }
            
            void setIconPosition(IconPosition iconPosition)
            {
                iconPosition_ = iconPosition;
            }
            
            bool isPushed() const
            {
                return isPushed_;
            }
            
            void setPushed(bool isPushed)
            {
                isPushed_ = isPushed;
            }
            
            const std::function <void ()> getCallback() const
            {
                return callback_;
            }
            
            void setCallback(const std::function <void ()>& callback)
            {
                callback_ = callback;
            }
            
            void onMouseLeftButtonDown(float x, float y)
            {
                isPushed_ = true;
            }
            
            void onMouseLeftButtonDragged(float x, float y)
            {
                if(!(x >= position_.x && x <= position_.x + size_.width && y >= position_.y && y <= position_.y + size_.height))
                {
                    isPushed_ = false;
                }
            }
            
            void onMouseLeftButtonUp(float x, float y)
            {
                if(isPushed_)
                {
                    isPushed_ = false;
                    if(callback_)
                    {
                        callback_();
                    }
                }
            }
            
            virtual Size2f getPreferredSize(NVGcontext* context) const override
            {
                int fontSize = fontSize_ <= 0? theme_->buttonFontSize : fontSize_;
                nvgFontSize(context, fontSize);
                nvgFontFace(context, "sans-bold");
                float tw = nvgTextBounds(context, 0,0, caption_.c_str(), nullptr, nullptr);
                float iw = 0.0f, ih = fontSize;
                
                if(icon_)
                {
                    if(icon_ > 1024)
                    {
                        ih *= theme_->iconScale;
                        nvgFontFace(context, "icons");
                        nvgFontSize(context, ih);
                        iw = nvgTextBounds(context, 0, 0, utf8(icon_).data(), nullptr, nullptr) + size_.height * 0.15f;
                    }
                    else
                    {
                        int w, h;
                        ih *= 0.9f;
                        nvgImageSize(context, icon_, &w, &h);
                        iw = w * ih / h;
                    }
                }
                return Vec2f((tw + iw) + 20.0f, fontSize + 10.0f);
            }
            
            virtual void render(NVGcontext* context) const override
            {
                Widget::render(context);
                
                NVGcolor gradTop = theme_->buttonGradientTopUnfocused;
                NVGcolor gradBot = theme_->buttonGradientBotUnfocused;
                
                if(isPushed_)
                {
                    gradTop = theme_->buttonGradientTopPushed;
                    gradBot = theme_->buttonGradientBotPushed;
                }
                else if(hasMouseFocus_ && isEnabled_)
                {
                    gradTop = theme_->buttonGradientTopFocused;
                    gradBot = theme_->buttonGradientBotFocused;
                }
                
                nvgBeginPath(context);
                
                nvgRoundedRect(context, position_.x + 1.0f, position_.y + 1.0f, size_.width - 2.0f, size_.height - 2.0f, theme_->buttonCornerRadius - 1.0f);
                
                if(backgroundColor_.a != 0.0f)
                {
                    nvgFillColor(context, Color(backgroundColor_.r, backgroundColor_.g, backgroundColor_.b, 1.f));
                    nvgFill(context);
                    if(isPushed_)
                    {
                        gradTop.a = gradBot.a = 0.8f;
                    }
                    else
                    {
                        double v = 1 - backgroundColor_.a;
                        gradTop.a = gradBot.a = isEnabled_? v : v * .5f + .5f;
                    }
                }
                
                NVGpaint bg = nvgLinearGradient(context, position_.x, position_.y, position_.x, position_.y + size_.height, gradTop, gradBot);
                
                nvgFillPaint(context, bg);
                nvgFill(context);
                
                nvgBeginPath(context);
                nvgStrokeWidth(context, 1.0f);
                nvgRoundedRect(context, position_.x + 0.5f, position_.y + (isPushed_? 0.5f : 1.5f), size_.width - 1.0f, size_.height - 1.0f - (isPushed_? 0.0f : 1.0f), theme_->buttonCornerRadius);
                nvgStrokeColor(context, theme_->borderLight);
                nvgStroke(context);
                
                nvgBeginPath(context);
                nvgRoundedRect(context, position_.x + 0.5f, position_.y + 0.5f, size_.width - 1.0f, size_.height - 2.0f, theme_->buttonCornerRadius);
                nvgStrokeColor(context, theme_->borderDark);
                nvgStroke(context);
                
                int fontSize = fontSize_ <= 0? theme_->buttonFontSize : fontSize_;
                nvgFontSize(context, fontSize);
                nvgFontFace(context, "sans-bold");
                float tw = nvgTextBounds(context, 0, 0, caption_.c_str(), nullptr, nullptr);
                
                Vec2f center(position_.x + (size_.width * 0.5f), position_.y + (size_.height * 0.5f));
                Vec2f textPos(center.x - tw * 0.5f, center.y - 1.0f);
                NVGcolor textColor = textColor_.a == 0 ? theme_->textColor : textColor_;
                if(!isEnabled_)
                {
                    textColor = theme_->disabledTextColor;
                }
                
                if(icon_)
                {
                    auto icon = utf8(icon_);
                    
                    float iw, ih = fontSize;
                    if(icon_ > 1024)
                    {
                        ih *= theme_->iconScale;
                        nvgFontSize(context, ih);
                        nvgFontFace(context, "icons");
                        iw = nvgTextBounds(context, 0, 0, icon.data(), nullptr, nullptr);
                    }
                    else
                    {
                        int w, h;
                        ih *= 0.9f;
                        nvgImageSize(context, icon_, &w, &h);
                        iw = w * ih / h;
                    }
                    if(caption_ != "")
                    {
                        iw += size_.height * 0.15f;
                    }
                    nvgFillColor(context, textColor);
                    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    Vec2f iconPos = center;
                    iconPos.y -= 1.0f;
                    
                    if(iconPosition_ == IconPosition::LeftCentered)
                    {
                        iconPos.x -= (tw + iw) * 0.5f;
                        textPos.x += iw * 0.5f;
                    }
                    else if(iconPosition_ == IconPosition::RightCentered)
                    {
                        textPos.x -= iw * 0.5f;
                        iconPos.x += tw * 0.5f;
                    }
                    else if(iconPosition_ == IconPosition::Left)
                    {
                        iconPos.x = position_.x + 8.0f;
                    }
                    else if(iconPosition_ == IconPosition::Right)
                    {
                        iconPos.x = position_.x + size_.width - iw - 8.0f;
                    }
                    
                    if(icon_ > 1024)
                    {
                        nvgText(context, iconPos.x, iconPos.y + 1.0f, icon.data(), nullptr);
                    }
                    else
                    {
                        NVGpaint imgPaint = nvgImagePattern(context, iconPos.x, iconPos.y - ih / 2.0f, iw, ih, 0, icon_, isEnabled_? 0.5f : 0.25f);
                        nvgFillPaint(context, imgPaint);
                        nvgFill(context);
                    }
                }
                
                nvgFontSize(context, fontSize);
                nvgFontFace(context, "sans-bold");
                nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                nvgFillColor(context, theme_->textColorShadow);
                nvgText(context, textPos.x, textPos.y, caption_.c_str(), nullptr);
                nvgFillColor(context, textColor);
                nvgText(context, textPos.x, textPos.y + 1.0f, caption_.c_str(), nullptr);
            }
            
        protected:
            std::string caption_;
            int icon_;
            IconPosition iconPosition_;
            bool isPushed_;
            Color backgroundColor_;
            Color textColor_;
            std::function <void ()> callback_;
        };
    }
}
