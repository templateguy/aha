//
//  Layout.cpp
//  Aha
//
//  Created by Priyanshi Thakur on 02/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#include "../Aha.h"


namespace aha
{
    namespace ui
    {
        Vec2f BoxLayout::getPreferredSize(NVGcontext* context, const Widget* widget) const
        {
            Vec2f size(Vec2f(2.0f * margin_, 2.0f * margin_));
            
            int yOffset = 0;
            const UIWindow* window = dynamic_cast <const UIWindow*> (widget);
            if(window && !window->getTitle().empty())
            {
                if(orientation_ == Orientation::Vertical)
                {
                    size.height += widget->getTheme()->windowHeaderHeight - margin_ / 2;
                }
                else
                {
                    yOffset = widget->getTheme()->windowHeaderHeight;
                }
            }
            
            bool first = true;
            auto children(widget->getAllChildren());
            for(auto child : children)
            {
                if (!child->isVisible())
                {
                    continue;
                }
                if (first)
                {
                    first = false;
                }
                else
                {
                    size.width += spacing_;
                }
                
                Vec2f ps = child->getPreferredSize(context);
                Vec2f fs = child->getFixedSize();
                Vec2f targetSize(fs.width ? fs.width : ps.width, fs.height ? fs.height : ps.height);
                
                size.height += targetSize.width;
                size.height = std::max(size.height, targetSize.height + 2.0f * margin_);
                first = false;
            }
            return Vec2f(size.width, size.height + yOffset);
        }
        
        void BoxLayout::performLayout(NVGcontext* context, Widget* widget) const
        {
            Size2f fs_w = widget->getFixedSize();
            Size2f containerSize(fs_w.width ? fs_w.width : widget->getWidth(), fs_w.height ? fs_w.height : widget->getHeight());
            
            float position = margin_;
            float yOffset = 0.0f;
            
            const UIWindow* window(dynamic_cast <const UIWindow*> (widget));
            if(window && !window->getTitle().empty())
            {
                if(orientation_ == Orientation::Vertical)
                {
                    position += widget->getTheme()->windowHeaderHeight - margin_ / 2.0f;
                }
                else
                {
                    yOffset = widget->getTheme()->windowHeaderHeight;
                    containerSize.height -= yOffset;
                }
            }
            
            bool first = true;
            auto children(widget->getAllChildren());
            for(auto child : children)
            {
                if (!child->isVisible())
                {
                    continue;
                }
                if(first)
                {
                    first = false;
                }
                else
                {
                    position += spacing_;
                }
                
                Size2f ps(child->getPreferredSize(context));
                Size2f fs(child->getFixedSize());
                Size2f targetSize(fs.width ? fs.width : ps.width, fs.height ? fs.height : ps.height);
                Vec2f pos(0.0f, yOffset);
                
                pos.x = position;
                
                switch(alignment_)
                {
                    case Alignment::Minimum:
                        pos.y += margin_;
                        break;
                    case Alignment::Middle:
                        pos.y += (containerSize.height - targetSize.height) / 2.0f;
                        break;
                    case Alignment::Maximum:
                        pos.y += containerSize.height - targetSize.height - margin_ * 2.0f;
                        break;
                    case Alignment::Fill:
                        pos.y += margin_;
                        targetSize.y = fs.height ? fs.height : (containerSize.height - margin_ * 2.0f);
                        break;
                }
                
                child->setPosition(pos);
                child->setSize(targetSize);
                child->performLayout(context);
                position += targetSize.width;
            }
        }
        
        
        Vec2f GroupLayout::getPreferredSize(NVGcontext* context, const Widget* widget) const
        {
            float height(margin_);
            float width(2.0f * margin_);
            
            const UIWindow* window(dynamic_cast<const UIWindow*> (widget));
            if(window && !window->getTitle().empty())
            {
                height += widget->getTheme()->windowHeaderHeight - margin_ / 2.0f;
            }
            
            bool first(true);
            bool indent(false);
            auto children(widget->getAllChildren());
            for(auto child : children)
            {
                if(!child->isVisible())
                {
                    continue;
                }
                std::shared_ptr <UILabel> label(std::dynamic_pointer_cast <UILabel> (child));
                if (!first)
                {
                    height += (label == nullptr) ? spacing_ : groupSpacing_;
                }
                first = false;
                
                Size2f ps(child->getPreferredSize(context));
                Size2f fs(child->getFixedSize());
                Size2f targetSize(fs.width ? fs.width : ps.width, fs.height ? fs.height : ps.height);
                
                bool indentCur(indent && label == nullptr);
                height += targetSize.height;
                width = std::max(width, targetSize.width + 2.0f * margin_ + (indentCur ? groupIndent_ : 0.0f));
                
                if(label)
                {
                    indent = !label->getCaption().empty();
                }
            }
            height += margin_;
            return Size2f(width, height);
        }
        
        void GroupLayout::performLayout(NVGcontext* context, Widget* widget) const
        {
            float height(margin_);
            float availableWidth((widget->getFixedWidth() ? widget->getFixedWidth() : widget->getWidth()) - 2.0f * margin_);
            
            const UIWindow* window(dynamic_cast <const UIWindow*> (widget));
            if(window && !window->getTitle().empty())
            {
                height += widget->getTheme()->windowHeaderHeight - margin_ / 2.0f;
            }
            
            bool first(true);
            bool indent(false);
            auto children(widget->getAllChildren());
            for(auto child : children)
            {
                if(!child->isVisible())
                {
                    continue;
                }
                std::shared_ptr <UILabel> label(std::dynamic_pointer_cast <UILabel> (child));
                if(!first)
                {
                    height += (label == nullptr) ? spacing_ : groupSpacing_;
                }
                first = false;
                
                bool indentCur(indent && label == nullptr);
                Size2f ps(Size2f(availableWidth - (indentCur ? groupIndent_ : 0.0f), child->getPreferredSize(context).height));
                Size2f fs(child->getFixedSize());
                
                Size2f targetSize(fs.width ? fs.width : ps.width, fs.height ? fs.height : ps.height);
                
                child->setPosition(Vec2f(margin_ + (indentCur ? groupIndent_ : 0.0f), height));
                child->setSize(targetSize);
                child->performLayout(context);
                
                height += targetSize.height;
                
                if(label)
                {
                    indent = !label->getCaption().empty();
                }
            }
        }
    }
}
