//
//  UIWindow.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 02/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Widget.hpp"
#include "UIRespondsToMouseEvents.hpp"


namespace aha
{
    namespace ui
    {
        class UIWindow : public Widget, public UIRespondsToMouseEvents <UIWindow>
        {
        public:
            UIWindow()
            {
                type_ = "UIWindow";
            }
            
            const std::string& getTitle() const
            {
                return title_;
            }
            
            void setTitle(const std::string& title)
            {
                title_ = title;
            }
            
            bool isModal() const
            {
                return isModal_;
            }
            
            void setModal(bool isModal)
            {
                isModal_ = isModal;
            }
            
            std::shared_ptr <Widget>& getButtonPanel()
            {
                if(!buttonPanel_)
                {
                    buttonPanel_ = std::make_shared <Widget> ();
                    buttonPanel_->setLayout(std::make_shared <BoxLayout> (Orientation::Horizontal, Alignment::Middle, 0, 4));
                    addChild(buttonPanel_);
                }
                return buttonPanel_;
            }
            
            void dispose()
            {
                
            }
            
            void center()
            {
                
            }
            
            void onMouseLeftButtonDown(float x, float y)
            {
                if(y >= position_.y && y <= position_.y + theme_->windowHeaderHeight)
                {
                    mouseX_ = x;
                    mouseY_ = y;
                    selfX_ = position_.x;
                    selfY_ = position_.y;
                    drag_ = true;
                }
            }
             
            void onMouseLeftButtonDragged(float x, float y)
            {
                if(drag_)
                {
                    float deltaX(x - mouseX_);
                    float deltaY(y - mouseY_);
                    position_.x = deltaX + selfX_;
                    position_.y = deltaY + selfY_;
                }
            }
            
            void onMouseLeftButtonUp(float x, float y)
            {
                drag_ = false;
            }
             
            virtual Vec2f getPreferredSize(NVGcontext* context) const override
            {
                if(buttonPanel_)
                {
                    buttonPanel_->setVisible(false);
                }
                Size2f ps(Widget::getPreferredSize(context));
                if(buttonPanel_)
                {
                    buttonPanel_->setVisible(true);
                }
                nvgFontSize(context, 18.0f);
                nvgFontFace(context, "sans-bold");
                float bounds[4];
                nvgTextBounds(context, 0, 0, title_.c_str(), nullptr, bounds);
                float width(bounds[2] - bounds[0] + 20);
                float height(bounds[3] - bounds[1]);
                return Vec2f(ps.width > width ? ps.width : width, ps.height > height ? ps.height : height);
            }
            
            virtual void performLayout(NVGcontext* context) override
            {
                if(!buttonPanel_)
                {
                    Widget::performLayout(context);
                }
                else
                {
                    buttonPanel_->setVisible(false);
                    Widget::performLayout(context);
                    auto children(buttonPanel_->getAllChildren());
                    for(auto child : children)
                    {
                        child->setFixedSize(Vec2f(22.0f, 22.0f));
                        child->setFontSize(15);
                    }
                    buttonPanel_->setVisible(true);
                    buttonPanel_->setSize(Vec2f(getWidth(), 22.0f));
                    buttonPanel_->setPosition(Vec2f(getWidth() - (buttonPanel_->getPreferredSize(context).width + 5.0f), 3.0f));
                    buttonPanel_->performLayout(context);
                }
            }
                
            void render(NVGcontext* context) const override
            {
                int ds(theme_->windowDropShadowSize);
                int cr(theme_->windowCornerRadius);
                int hh(theme_->windowHeaderHeight);
                
                /* Draw window */
                nvgSave(context);
                nvgBeginPath(context);
                nvgRoundedRect(context, position_.x, position_.y, size_.width, size_.height, cr);
                
                nvgFillColor(context, hasMouseFocus_? theme_->windowFillFocused : theme_->windowFillUnfocused);
                nvgFill(context);
                
                /* Draw a drop shadow */
                NVGpaint shadowPaint(nvgBoxGradient(context, position_.x, position_.y, size_.width, size_.height, cr * 2, ds * 2, theme_->dropShadow, theme_->transparent));
                
                nvgSave(context);
                nvgResetScissor(context);
                nvgBeginPath(context);
                nvgRect(context, position_.x - ds, position_.y - ds, size_.width + 2 * ds, size_.height + 2 * ds);
                nvgRoundedRect(context, position_.x, position_.y, size_.width, size_.height, cr);
                nvgPathWinding(context, NVG_HOLE);
                nvgFillPaint(context, shadowPaint);
                nvgFill(context);
                nvgRestore(context);
                
                if(!title_.empty())
                {
                    /* Draw header */
                    NVGpaint headerPaint(nvgLinearGradient(context, position_.x, position_.y, position_.x, position_.y + hh, theme_->windowHeaderGradientTop, theme_->windowHeaderGradientBot));
                    
                    nvgBeginPath(context);
                    nvgRoundedRect(context, position_.x, position_.y, size_.width, hh, cr);
                    
                    nvgFillPaint(context, headerPaint);
                    nvgFill(context);
                    
                    nvgBeginPath(context);
                    nvgRoundedRect(context, position_.x, position_.y, size_.width, hh, cr);
                    nvgStrokeColor(context, theme_->windowHeaderSepTop);
                    
                    nvgSave(context);
                    nvgIntersectScissor(context, position_.x, position_.y, size_.width, 0.5f);
                    nvgStroke(context);
                    nvgRestore(context);
                    
                    nvgBeginPath(context);
                    nvgMoveTo(context, position_.x + 0.5f, position_.y + hh - 1.5f);
                    nvgLineTo(context, position_.x + size_.width - 0.5f, position_.y + hh - 1.5f);
                    nvgStrokeColor(context, theme_->windowHeaderSepBot);
                    nvgStroke(context);
                    
                    nvgFontSize(context, 18.0f);
                    nvgFontFace(context, "sans-bold");
                    nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
                    
                    nvgFontBlur(context, 2);
                    nvgFillColor(context, theme_->dropShadow);
                    nvgText(context, position_.x + size_.width / 2.0f, position_.y + hh / 2.0f, title_.c_str(), nullptr);
                    
                    nvgFontBlur(context, 0);
                    nvgFillColor(context, hasFocus_ ? theme_->windowTitleFocused : theme_->windowTitleUnfocused);
                    nvgText(context, position_.x + size_.width / 2.0f, position_.y + hh / 2.0f - 1.0f, title_.c_str(), nullptr);
                }
                
                nvgRestore(context);
                Widget::render(context);
            }
                
        protected:
            std::string title_{"Untitled"};
            std::shared_ptr <Widget> buttonPanel_{nullptr};
            bool isModal_{false};
            bool drag_{false};
            float mouseX_{};
            float mouseY_{};
            float selfX_{};
            float selfY_{};
        };
    }
}
