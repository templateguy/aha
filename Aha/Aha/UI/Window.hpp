//
//  Window.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 29/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <string>
#include "../nanovg/nanovg.h"
#ifndef NANOVG_GL3_IMPLEMENTATION
#   define NANOVG_GL3_IMPLEMENTATION
#endif
#include "../nanovg/nanovg_gl.h"
#include "TitleBar.hpp"


namespace aha
{
    namespace ui
    {
        class Window : public std::enable_shared_from_this <Window>, public UI
        {
        public:
            Window(const std::string& title = "Window", float x = 100.0f, float y = 100.0f, float width = 800.0f, float height = 600.0f, float cornerRadius = 3.0f) : titleBar_(std::make_shared <TitleBar <Window>> (title, cornerRadius)), cornerRadius_(cornerRadius)
            {
                setPosition(glm::vec2(x, y));
                setSize(glm::vec2(width, height));
                titleBar_->setPosition(glm::vec2(0.0f, 0.0f));
                titleBar_->setSize(glm::vec2(width, 30.0f));
                addChild(titleBar_);
                titleBar_->setParent(this);
            }
            
            void render(NVGcontext* vg) const override
            {
                NVGpaint shadowPaint;
                nvgSave(vg);
                
                // Window
                nvgBeginPath(vg);
                nvgRoundedRect(vg, position_.x, position_.y, size_.x, size_.y, cornerRadius_);
                nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
                nvgFill(vg);
                
                // Drop shadow
                shadowPaint = nvgBoxGradient(vg, position_.x, position_.y + 2, size_.x, size_.y, cornerRadius_ * 2.0f, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
                nvgBeginPath(vg);
                nvgRect(vg, position_.x - 10.0f, position_.y - 10.0f, size_.x + 20.0f, size_.y + 30.0f);
                nvgRoundedRect(vg, position_.x, position_.y, size_.x, size_.y, cornerRadius_);
                nvgPathWinding(vg, NVG_HOLE);
                nvgFillPaint(vg, shadowPaint);
                nvgFill(vg);
                
                titleBar_->render(vg);
                
                nvgRestore(vg);
            }
            
        protected:
            std::shared_ptr <TitleBar <Window>> titleBar_;
            float cornerRadius_{3.0f};
        };
    }
}
