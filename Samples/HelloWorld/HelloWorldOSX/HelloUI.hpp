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
        aha::ui::UI.render();
    }
    
    AHA_CREATE_FUNC(HelloUI);
    
protected:
    bool init() override
    {
        window_ = std::make_shared <aha::ui::UIWindow> ();
        window_->setPosition(aha::Vec2f(100.0f, 100.0f));
        window_->setSize(aha::Size2f(160.0f, 230.0f));
        aha::ui::UI.addChild(window_);
        
        auto panel(std::make_shared <aha::ui::UIPanel> ());
        panel->setPosition(aha::Vec2f(3.0f, window_->getTheme()->windowHeaderHeight + 1.0f));
        panel->setLayout(std::make_shared <aha::ui::GroupLayout> ());
        window_->addChild(panel);
        
        label_ = std::make_shared <aha::ui::UILabel> ("Label");
        label_->setPosition(aha::Vec2f(100.0f, 100.0f));
        label_->setColor(aha::Color(0.0f, 1.0f, 0.0f, 1.0f));
        panel->addChild(label_);
        
        slider_ = std::make_shared <aha::ui::UISlider> ();
        slider_->setPosition(aha::Vec2f(0.0f, 100.0f));
        panel->addChild(slider_);
        
        auto object(std::make_shared <aha::ui::UIWindow> ());
        object->setSize(aha::Size2f(50.0f, 50.0f));
        //object->setFontSize(48);
        
        checkbox_ = std::make_shared <aha::ui::UICheckBox> (object);
        checkbox_->setPosition(aha::Vec2f(50.0f, 50.0f));
        panel->addChild(checkbox_);
        
        button_ = std::make_shared <aha::ui::UIButton> ("Click Me...", ICON_LOGIN);
        button_->setPosition(aha::Vec2f(100.0f, 150.0f));
        panel->addChild(button_);
        
        std::vector <std::string> fileTypes;
        fileTypes.emplace_back("png");
        fileTypes.emplace_back("jpg");
        fileTypes.emplace_back("tga");
        fileTypes.emplace_back("hdr");
        auto file(std::make_shared <aha::ui::UIOpenFileDialog> (fileTypes));
        file->setFileCallback([](std::vector <std::string> urls)
        {
            for(auto file : urls)
            {
                std::cout << file << std::endl;
            }
        });
        file->setPosition(aha::Vec2f(20.0f, 20.0f));
        panel->addChild(file);
        
        return true;
    }
    
    std::shared_ptr <aha::ui::UIWindow> window_{};
    std::shared_ptr <aha::ui::UILabel> label_{};
    std::shared_ptr <aha::ui::UISlider> slider_{};
    std::shared_ptr <aha::ui::UICheckBox> checkbox_{};
    std::shared_ptr <aha::ui::UIButton> button_{};
};
