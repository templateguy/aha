//
//  UI.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 29/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <vector>
#include <memory>
#include "../Preprocessor/Preprocessor.h"
#include "Widget.hpp"


#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729
#define ICON_FOLDER 0x1f4c1


namespace aha
{
    namespace ui
    {
        class UI_ final
        {
        public:
            void addChild(const std::shared_ptr <Widget>& child)
            {
                if(child)
                {
                    children_.emplace_back(child);
                }
            }
            
            bool removeChild(const std::shared_ptr <Widget>& child)
            {
                if(auto i(std::find(children_.begin(), children_.end(), child)); i != children_.end())
                {
                    children_.erase(i);
                    return true;
                }
                return false;
            }
            
            const std::shared_ptr <Widget>& getChildAt(size_t index) const
            {
                return children_[index];
            }
            
            const std::vector <std::shared_ptr <Widget>>& getAllChildren() const
            {
                return children_;
            }
            
            void requestFocus(Widget* widget)
            {
                for(auto child : focusedChildren_)
                {
                    child->setFocus(false);
                }
                focusedChildren_.erase(focusedChildren_.begin(), focusedChildren_.end());
                focusedChildren_.clear();
                Widget* parent{};
                while(widget)
                {
                    focusedChildren_.emplace_back(widget);
                    parent = widget;
                    widget = widget->getParent();
                }
                for(auto i(focusedChildren_.rbegin()); i != focusedChildren_.rend(); ++i)
                {
                    //(*i)->focusEvent(true);
                }
                moveToFront(parent->shared_from_this());
            }
            
            void moveToFront(const std::shared_ptr <Widget>& widget)
            {
                children_.erase(std::remove(children_.begin(), children_.end(), widget), children_.end());
                children_.emplace_back(widget);
            }
            
            void render() const
            {
                nvgBeginFrame(context_, static_cast <int> (aha::Application.getWindowWidth()), static_cast <int> (aha::Application.getWindowHeight()), 1.0f);
                for(auto child : children_)
                {
                    child->visit(context_);
                    child->render(context_);
                }
                nvgEndFrame(context_);
            }
            
            static UI_& getInstance()
            {
                static UI_ instance;
                return instance;
            }
            
            UI_(const UI_&) = delete;
            const UI_& operator =(const UI_&) = delete;
            
        private:
            UI_()
            {
                context_ = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
                if(context_ == NULL)
                {
                    printf("Could not init nanovg.\n");
                }
                fontIcons_ = nvgCreateFont(context_, "icons", "fonts/entypo.ttf");
                if(fontIcons_ == -1)
                {
                    printf("Could not add font icons.\n");
                }
                fontNormal_ = nvgCreateFont(context_, "sans", "fonts/Roboto-Regular.ttf");
                if(fontNormal_ == -1)
                {
                    printf("Could not add font italic.\n");
                }
                fontBold_ = nvgCreateFont(context_, "sans-bold", "fonts/Roboto-Bold.ttf");
                if(fontBold_ == -1)
                {
                    printf("Could not add font bold.\n");
                }
                fontEmoji_ = nvgCreateFont(context_, "emoji", "fonts/NotoEmoji-Regular.ttf");
                if(fontEmoji_ == -1)
                {
                    printf("Could not add font emoji.\n");
                }
                nvgAddFallbackFontId(context_, fontNormal_, fontEmoji_);
                nvgAddFallbackFontId(context_, fontBold_, fontEmoji_);
            }
            
            NVGcontext* context_{};
            int fontIcons_{};
            int fontNormal_{};
            int fontBold_{};
            int fontEmoji_{};
            std::vector <std::shared_ptr <Widget>> children_;
            std::vector <Widget*> focusedChildren_;
        };
    }
}
#define UI UI_::getInstance()
