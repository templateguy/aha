//
//  HelloPBR.hpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 24/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <iostream>
#include <OpenGL/gl.h>
#include <Aha.h>


class HelloPBR : public aha::Scene
{
public:
    void render() override
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader_.use();
        glm::mat4 view = camera_.getViewMatrix();
        shader_.setMat4("view", view);
        shader_.setVec3("camPos", camera_.getPosition());
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedo_->handle());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normal_->handle());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallic_->handle());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughness_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ao_->handle());
        
        // render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
        glm::mat4 model;
        for (int row = 0; row < rows_; ++row)
        {
            for (int col = 0; col < columns_; ++col)
            {
                model = glm::mat4();
                model = glm::translate(model, glm::vec3((float)(col - (rows_ / 2)) * spacing_, (float)(row - (columns_ / 2)) * spacing_, 0.0f));
                shader_.setMat4("model", model);
                renderSphere();
            }
        }
        
        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and
        // keeps the codeprint small.
        for (unsigned int i = 0; i < sizeof(lightPositions_) / sizeof(lightPositions_[0]); ++i)
        {
            glm::vec3 newPos = lightPositions_[i] + glm::vec3(sin(timer_.elapsed() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions_[i];
            shader_.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            shader_.setVec3("lightColors[" + std::to_string(i) + "]", lightColors_[i]);
            
            model = glm::mat4();
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            shader_.setMat4("model", model);
            renderSphere();
        }
    }
    
    AHA_CREATE_FUNC(HelloPBR);
    
protected:
    bool init() override
    {
        glEnable(GL_DEPTH_TEST);
        shader_.load("shaders/hello_pbr.vs", "shaders/hello_pbr.fs");
        shader_.use();
        shader_.setInt("albedoMap", 0);
        shader_.setInt("normalMap", 1);
        shader_.setInt("metallicMap", 2);
        shader_.setInt("roughnessMap", 3);
        shader_.setInt("aoMap", 4);
        
        // load PBR material textures
        // --------------------------
        albedo_    = aha::Texture::New("textures/pbr/rusted_iron/albedo.png");
        normal_    = aha::Texture::New("textures/pbr/rusted_iron/normal.png");
        metallic_  = aha::Texture::New("textures/pbr/rusted_iron/metallic1.png");
        roughness_ = aha::Texture::New("textures/pbr/rusted_iron/roughness1.png");
        ao_        = aha::Texture::New("textures/pbr/rusted_iron/ao.png");
        
        // initialize static shader uniforms before rendering
        // --------------------------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera_.getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);
        shader_.use();
        shader_.setMat4("projection", projection);
        
        return true;
    }
    
    void renderSphere()
    {
        if(sphereVAO_ == 0)
        {
            glGenVertexArraysAPPLE(1, &sphereVAO_);
            
            unsigned int vbo, ebo;
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);
            
            std::vector<glm::vec3> positions;
            std::vector<glm::vec2> uv;
            std::vector<glm::vec3> normals;
            std::vector<unsigned int> indices;
            
            const unsigned int X_SEGMENTS = 64;
            const unsigned int Y_SEGMENTS = 64;
            const float PI = 3.14159265359;
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    float xSegment = (float)x / (float)X_SEGMENTS;
                    float ySegment = (float)y / (float)Y_SEGMENTS;
                    float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                    float yPos = std::cos(ySegment * PI);
                    float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                    
                    positions.push_back(glm::vec3(xPos, yPos, zPos));
                    uv.push_back(glm::vec2(xSegment, ySegment));
                    normals.push_back(glm::vec3(xPos, yPos, zPos));
                }
            }
            
            bool oddRow = false;
            for (int y = 0; y < Y_SEGMENTS; ++y)
            {
                if (!oddRow) // even rows: y == 0, y == 2; and so on
                {
                    for (int x = 0; x <= X_SEGMENTS; ++x)
                    {
                        indices.push_back(y       * (X_SEGMENTS + 1) + x);
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    }
                }
                else
                {
                    for (int x = X_SEGMENTS; x >= 0; --x)
                    {
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    }
                }
                oddRow = !oddRow;
            }
            indexCount_ = static_cast <unsigned int > (indices.size());
            
            std::vector<float> data;
            for (int i = 0; i < positions.size(); ++i)
            {
                data.push_back(positions[i].x);
                data.push_back(positions[i].y);
                data.push_back(positions[i].z);
                if (uv.size() > 0)
                {
                    data.push_back(uv[i].x);
                    data.push_back(uv[i].y);
                }
                if (normals.size() > 0)
                {
                    data.push_back(normals[i].x);
                    data.push_back(normals[i].y);
                    data.push_back(normals[i].z);
                }
            }
            glBindVertexArrayAPPLE(sphereVAO_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
            float stride = (3 + 2 + 3) * sizeof(float);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
        }
        
        glBindVertexArrayAPPLE(sphereVAO_);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount_, GL_UNSIGNED_INT, 0);
    }
    
    aha::Camera camera_{glm::vec3(0.0f, 0.0f, 3.0f)};
    aha::Shader shader_;
    std::shared_ptr <aha::Texture> albedo_;
    std::shared_ptr <aha::Texture> normal_;
    std::shared_ptr <aha::Texture> metallic_;
    std::shared_ptr <aha::Texture> roughness_;
    std::shared_ptr <aha::Texture> ao_;
    static constexpr int rows_{7};
    static constexpr int columns_{7};
    static constexpr float spacing_{1.5f};
    glm::vec3 lightPositions_[1] =
    {
        glm::vec3(0.0f, 0.0f, 10.0f),
    };
    glm::vec3 lightColors_[1] =
    {
        glm::vec3(150.0f, 150.0f, 150.0f),
    };
    unsigned int sphereVAO_{0};
    unsigned int indexCount_{};
    aha::Timer timer_{};
};
