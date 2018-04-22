//
//  HelloModel.hpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 21/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <iostream>
#include <OpenGL/gl.h>
#include <Aha.h>


class HelloModel : public aha::Scene
{
public:
    void render() override
    {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // don't forget to enable shader before setting uniforms
        shader_.use();
        
        // view/projection transformations
        const float width(aha::Application.getWindowWidth());
        const float height(aha::Application.getWindowHeight());
        const glm::vec3 position(0.0f, 0.0f, 3.0f);
        const glm::vec3 front(0.0f, 0.0f, -1.0f);
        const glm::vec3 up(0.0f, 1.0f, 0.0f);
        const float zoom(45.0f);
        glm::mat4 projection = glm::perspective(glm::radians(zoom), (float) width / (float) height, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(position, position + front, up);;
        shader_.setMat4("projection", projection);
        shader_.setMat4("view", view);
        
        // render the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, -1.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));    // it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, delta_, glm::vec3(0.0f, 1.0f, 0.0f));
        shader_.setMat4("model", model);
        model_.render(shader_.ID);
        delta_ += 0.025f;
        delta_ = delta_ > 360.0f? delta_ - 360.0f : delta_;
    }
    
    AHA_CREATE_FUNC(HelloModel);
    
protected:
    bool init() override
    {
        glEnable(GL_DEPTH_TEST);
        shader_.load("shaders/model.vs", "shaders/model.fs");
        model_.load("objects/nanosuit/nanosuit.obj");
        return true;
    }
    
    aha::Shader shader_;
    aha::Model model_;
    float delta_ = 0.0f;
};
