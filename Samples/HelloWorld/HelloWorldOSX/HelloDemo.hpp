//
//  HelloDemo.hpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 06/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <iostream>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include <Aha.h>


class HelloDemo : public aha::Scene
{
public:
    void render() override
    {
        auto currentTime(timer_.elapsed());
        auto delta(currentTime - lastTime_);
        std::string fps("FPS: ");
        std::stringstream out;
        out << 1.0f / delta;
        fps += out.str();
        label_->setCaption(fps);
        lastTime_ = currentTime;
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // bind pre-computed IBL data
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_.getIrradianceMap());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_.getPrefilterMap());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cubemap_.getBRDFLUTTexture());
        
        initializeStaticShaderUniforms_();
        glm::mat4 view(camera_.getViewMatrix());
        glm::vec3 camPos(camera_.getPosition());
        
        for(const auto& model : models_)
        {
            auto shader(model.getShader());
            shader.use();
            shader.setMat4("view", view);
            shader.setVec3("camPos", camPos);
            model.render();
        }
        
        wall_.setViewMatrix(view);
        wall_.setCameraPosition(camPos);
        wall_.render();
        
        // render skybox (render as last to prevent overdraw)
        cubemap_.setViewMatrix(view);
        cubemap_.render();
        
        aha::ui::UI.render();
    }
    
    AHA_CREATE_FUNC(HelloDemo);
    
protected:
    bool init() override
    {
        loadModel_();
        //cubemap_.load("textures/hdr/newport_loft.hdr");
        //cubemap_.load("pbr/hdr/Topanga_Forest_B/Topanga_Forest_B_3k.hdr");
        cubemap_.load("pbr/hdr/PaperMill_Ruins_E/PaperMill_E_3k.hdr");
        
        setupViewport_();
        
        auto panel(std::make_shared <aha::ui::UIPanel> ());
        panel->setPosition(aha::Vec2f(1200.0f, 700.0f));
        panel->setLayout(std::make_shared <aha::ui::GroupLayout> ());
        aha::ui::UI.addChild(panel);
        
        label_ = std::make_shared <aha::ui::UILabel> ("Label");
        label_->setFontSize(48.0f);
        panel->addChild(label_);
        panel->setSize(aha::Vec2f(300.0f, 78.0f));
        
        timer_.reset();
        lastTime_ = timer_.elapsed();
        
        return true;
    }
    
    void loadModel_()
    {
        for(auto i(0); i < 1; ++i)
        {
            aha::PBROBJModel model;
            model.loadModel("pbr/models/cerberus/cerberus_lp.obj");
            model.setPosition(glm::vec3(0.0f, rand() % 100, 0.0f));
            models_.emplace_back(model);
        }
        wall_.load("pbr/textures/bathroom_tile_1/");
        wall_.setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
        wall_.setSize(glm::vec2(4.0f, 4.0f));
    }
    
    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    void initializeStaticShaderUniforms_()
    {
        glm::mat4 projection = camera_.getProjectionMatrix();
        for(const auto& model : models_)
        {
            model.useShader();
            model.getShader().setMat4("projection", projection);
        }
        wall_.setProjectionMatrix(projection);
        cubemap_.setProjectionMatrix(projection);
    }
    
    // Before rendering, configure the viewport to the original framebuffer's screen dimensions
    void setupViewport_()
    {
        glViewport(0, 0, (int) aha::Application.getWindowWidth(), (int) aha::Application.getWindowHeight());
    }
    
    // PBR Models
    std::vector <aha::PBROBJModel> models_;
    aha::PBRQuad wall_;
    
    // Environment Map Skybox
    aha::HDRCubemap cubemap_;
    
    
    // Camera
    aha::ControlledCamera camera_{glm::vec3(0.0f, 0.0f, 5.0f)};
    
    // FPS
    std::shared_ptr <aha::ui::UILabel> label_;
    aha::Timer timer_;
    float lastTime_{};
    
    // Temporary
    float rotation_ = (-90.0f * 3.1415f) / 180.0f;
};
