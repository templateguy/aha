//
//  HelloUI.hpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 29/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <iostream>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <Aha.h>


class HelloUI : public aha::Scene
{
public:
    void render() override
    {
        glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        nvgBeginFrame(vg_, aha::Application.getWindowWidth(), aha::Application.getWindowHeight(), 1.0f);
        window_.render(vg_);
        slider_.render(vg_);
        checkbox_.render(vg_);
        nvgEndFrame(vg_);
    }
    
    AHA_CREATE_FUNC(HelloUI);
    
protected:
    bool init() override
    {
        vg_ = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
        if(vg_ == NULL)
        {
            printf("Could not init nanovg.\n");
        }
        fontIcons_ = nvgCreateFont(vg_, "icons", "fonts/entypo.ttf");
        if(fontIcons_ == -1)
        {
            printf("Could not add font icons.\n");
            return -1;
        }
        fontNormal_ = nvgCreateFont(vg_, "sans", "fonts/Roboto-Regular.ttf");
        if(fontNormal_ == -1)
        {
            printf("Could not add font italic.\n");
            return -1;
        }
        fontBold_ = nvgCreateFont(vg_, "sans-bold", "fonts/Roboto-Bold.ttf");
        if(fontBold_ == -1)
        {
            printf("Could not add font bold.\n");
            return -1;
        }
        fontEmoji_ = nvgCreateFont(vg_, "emoji", "fonts/NotoEmoji-Regular.ttf");
        if(fontEmoji_ == -1)
        {
            printf("Could not add font emoji.\n");
            return -1;
        }
        nvgAddFallbackFontId(vg_, fontNormal_, fontEmoji_);
        nvgAddFallbackFontId(vg_, fontBold_, fontEmoji_);
        slider_.setPosition(glm::vec2(200.0f, 200.0f));
        checkbox_.setPosition(glm::vec2(200.0f, 250.0f));
        return true;
    }
    
    NVGcontext* vg_{};
    aha::ui::Window window_{};
    aha::ui::Slider slider_{0.5f};
    aha::ui::CheckBox checkbox_{true};
    int fontIcons_;
    int fontNormal_;
    int fontBold_;
    int fontEmoji_;
};
