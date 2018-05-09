//
//  Widget.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 01/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <memory>
#include <vector>
#include <array>
#include "../nanovg/nanovg.h"
#include "../nanovg/nanovg_gl.h"
#include "../Math/Vec2.hpp"
#include "Layout.hpp"
#include "Theme.hpp"


namespace aha
{
    namespace ui
    {
        class Widget : public std::enable_shared_from_this <Widget>
        {
        public:
            Widget() : theme_(std::make_shared <Theme> ())
            {
                
            }
            
            virtual ~Widget()
            {
                
            }
            
            const std::string& getType() const
            {
                return type_;
            }
            
            bool isVisible() const
            {
                return isVisible_;
            }
            
            void setVisible(bool isVisible)
            {
                isVisible_ = isVisible;
            }
            
            bool isEnabled() const
            {
                return isEnabled_;
            }
            
            void setEnabled(bool isEnabled)
            {
                isEnabled_ = isEnabled;
            }
            
            bool hasFocus() const
            {
                return hasFocus_;
            }
            
            void setFocus(bool hasFocus)
            {
                hasFocus_ = hasFocus;
            }
            
            void requestFocus()
            {
                
            }
            
            bool hasMouseFocus() const
            {
                return hasMouseFocus_;
            }
            
            void setMouseFocus(bool hasMouseFocus)
            {
                hasMouseFocus_ = hasMouseFocus;
            }
            
            const Point2f& getPosition() const
            {
                return position_;
            }
            
            void setPosition(const Point2f& position)
            {
                position_ = position;
                setDirty_();
            }
            
            float getWidth() const
            {
                return size_.width;
            }
            
            void setWidth(float width)
            {
                size_.width = width;
                setDirty_();
            }
            
            float getHeight() const
            {
                return size_.height;
            }
            
            void setHeight(float height)
            {
                size_.height = height;
                setDirty_();
            }
            
            const Size2f& getSize() const
            {
                return size_;
            }
            
            virtual Size2f getPreferredSize(NVGcontext* context) const
            {
                return layout_? layout_->getPreferredSize(context, this) : size_;
            }
            
            void setSize(const Size2f& size)
            {
                size_ = size;
                setDirty_();
            }
            
            float getFixedWidth() const
            {
                return fixedSize_.width;
            }
            
            void setFixedWidth(float width)
            {
                fixedSize_.width = width;
                setDirty_();
            }
            
            float getFixedHeight() const
            {
                return fixedSize_.height;
            }
            
            void setFixedHeight(float height)
            {
                fixedSize_.height = height;
                setDirty_();
            }
            
            const Size2f& getFixedSize() const
            {
                return fixedSize_;
            }
            
            void setFixedSize(const Size2f& size)
            {
                fixedSize_ = size;
                setDirty_();
            }
            
            int getFontSize() const
            {
                return fontSize_;
            }
            
            void setFontSize(int fontSize)
            {
                fontSize_ = fontSize;
                setDirty_();
            }
            
            bool hasFontSize() const
            {
                return fontSize_ > 0;
            }
            
            Widget* getParent()
            {
                return parent_;
            }
            
            const Widget* getParent() const
            {
                return parent_;
            }
            
            void setParent(Widget* parent)
            {
                parent_ = parent;
                setDirty_();
            }
            
            const std::shared_ptr <Widget>& getChildAt(size_t index)
            {
                return children_[index];
            }
            
            void addChild(const std::shared_ptr <Widget>& child)
            {
                if(child)
                {
                    children_.emplace_back(child);
                    child->setParent(this);
                    child->setTheme(theme_);
                    setDirty_();
                }
            }
            
            void removeChild(const std::shared_ptr <Widget>& child)
            {
                children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
                setDirty_();
            }
            
            size_t getChildrenCount() const
            {
                return children_.size();
            }
            
            const std::vector <std::shared_ptr <Widget>>& getAllChildren() const
            {
                return children_;
            }
            
            void removeAllChildren()
            {
                children_.erase(children_.begin(), children_.end());
                children_.clear();
                setDirty_();
            }
            
            Vec2f convertToWindowSpace(const Vec2f& location)
            {
                auto locationInWindowSpace(location);
                locationInWindowSpace += getPosition();
                for(auto parent(parent_); parent; parent = parent->getParent())
                {
                    locationInWindowSpace += parent->getPosition();
                }
                return locationInWindowSpace;
            }
            
            Vec2f convertToParentSpace(const Vec2f& location)
            {
                auto locationInParentSpace(location);
                for(auto parent(parent_); parent; parent = parent->getParent())
                {
                    locationInParentSpace -= parent->getPosition();
                }
                return locationInParentSpace;
            }
            
            Vec2f convertToViewSpace(const Vec2f& location)
            {
                auto locationInViewSpace(location);
                locationInViewSpace -= getPosition();
                return convertToParentSpace(locationInViewSpace);
            }
            
            bool isWindowSpaceLocationInside(const Vec2f& location)
            {
                auto locationInViewSpace(convertToViewSpace(location));
                return isViewSpaceLocationInside(locationInViewSpace);
            }
            
            bool isParentSpaceLocationInside(const Vec2f& location)
            {
                return location.x >= position_.x && location.x <= position_.x + size_.width && location.y > position_.y && location.y <= position_.y + size_.height;
            }
            
            bool isViewSpaceLocationInside(const Vec2f& location)
            {
                return location.x >= 0.0f && location.x <= size_.width && location.y > 0.0f && location.y <= size_.height;
            }
            
            const std::shared_ptr <Layout>& getLayout() const
            {
                return layout_;
            }
            
            void setLayout(const std::shared_ptr <Layout>& layout)
            {
                layout_ = layout;
                setDirty_();
            }
            
            virtual void performLayout(NVGcontext* context)
            {
                if(layout_)
                {
                    layout_->performLayout(context, this);
                }
                else
                {
                    for(auto child : children_)
                    {
                        Size2f pref(child->getPreferredSize(context));
                        Size2f fix(child->getFixedSize());
                        child->setSize(Vec2f(fix.width ? fix.width : pref.width, fix.height ? fix.height : pref.height));
                        child->performLayout(context);
                    }
                }
            }
            
            const std::shared_ptr <Theme>& getTheme() const
            {
                return theme_;
            }
            
            virtual void setTheme(const std::shared_ptr <Theme>& theme)
            {
                theme_ = theme;
            }
            
            void visit(NVGcontext* context)
            {
                if(isDirty_)
                {
                    Widget* p{parent_};
                    Widget* parent{parent_};
                    while(parent)
                    {
                        p = parent;
                        parent = parent->getParent();
                    }
                    if(p)
                    {
                        p->performLayout(context);
                    }
                    else
                    {
                        performLayout(context);
                    }
                    isDirty_ = false;
                }
            }
            
            virtual void render(NVGcontext* context) const
            {
                if(!children_.empty())
                {
                    nvgSave(context);
                    //nvgIntersectScissor(context, position_.x, position_.y, size_.width, size_.height);
                    nvgTranslate(context, position_.x, position_.y);
                    for(auto child : children_)
                    {
                        if(child->isVisible())
                        {
                            nvgSave(context);
                            //nvgIntersectScissor(context, child->position_.x, child->position_.y, child->size_.width, child->size_.height);
                            child->render(context);
                            nvgRestore(context);
                        }
                    }
                    nvgRestore(context);
                }
            }
            
        protected:
            void setDirty_()
            {
                isDirty_ = true;
            }
            
            static char* cpToUTF8(int cp, char* str)
            {
                int n = 0;
                if (cp < 0x80) n = 1;
                else if (cp < 0x800) n = 2;
                else if (cp < 0x10000) n = 3;
                else if (cp < 0x200000) n = 4;
                else if (cp < 0x4000000) n = 5;
                else if (cp <= 0x7fffffff) n = 6;
                str[n] = '\0';
                switch (n)
                {
                    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
                    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
                    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
                    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
                    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
                    case 1: str[0] = cp;
                }
                return str;
            }
            
            static std::array <char, 8> utf8(int c)
            {
                std::array <char, 8> seq;
                int n = 0;
                if (c < 0x80) n = 1;
                else if (c < 0x800) n = 2;
                else if (c < 0x10000) n = 3;
                else if (c < 0x200000) n = 4;
                else if (c < 0x4000000) n = 5;
                else if (c <= 0x7fffffff) n = 6;
                seq[n] = '\0';
                switch(n)
                {
                    case 6: seq[5] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x4000000;
                    case 5: seq[4] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x200000;
                    case 4: seq[3] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x10000;
                    case 3: seq[2] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x800;
                    case 2: seq[1] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0xc0;
                    case 1: seq[0] = c;
                }
                return seq;
            }
            
            std::string type_{"Widget"};
            bool isVisible_{true};
            bool isEnabled_{true};
            bool hasFocus_{false};
            bool hasMouseFocus_{false};
            bool isDirty_{true};
            Point2f position_{0.0f, 0.0f};
            Size2f size_{0.0f, 0.0f};
            Size2f fixedSize_{0.0f, 0.0f};
            int fontSize_{0};
            Widget* parent_{};
            std::vector <std::shared_ptr <Widget>> children_;
            std::shared_ptr <Layout> layout_;
            std::shared_ptr <Theme> theme_;
        };
    }
}
