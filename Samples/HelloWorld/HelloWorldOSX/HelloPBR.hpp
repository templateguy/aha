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
        
        delta_ += 0.005f;
        delta_ = delta_ > 360.0f? delta_ - 360.0f : delta_;
        
        float scale(0.1f);
        float start(-0.5f);
        float distance(0.25f);
        
        shader_.use();
        glm::mat4 model;
        glm::mat4 view = camera_.getViewMatrix();
        shader_.setMat4("view", view);
        shader_.setVec3("camPos", camera_.getPosition());
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedoGold_->handle());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalGold_->handle());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallicGold_->handle());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughnessGold_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, aoGold_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(start, 0.0f, 2.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, delta_, glm::vec3(0.0f, 1.0f, 0.0f));
        shader_.setMat4("model", model);
        renderSphere();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedoGrass_->handle());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalGrass_->handle());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallicGrass_->handle());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughnessGrass_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, aoGrass_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(start + distance, 0.0f, 2.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, delta_, glm::vec3(0.0f, 1.0f, 0.0f));
        shader_.setMat4("model", model);
        renderSphere();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedoPlastic_->handle());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalPlastic_->handle());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallicPlastic_->handle());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughnessPlastic_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, aoPlastic_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(start + distance + distance, 0.0f, 2.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, delta_, glm::vec3(0.0f, 1.0f, 0.0f));
        shader_.setMat4("model", model);
        renderSphere();
        
        // Rusted Iron
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedoRustedIron_->handle());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalRustedIron_->handle());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallicRustedIron_->handle());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughnessRustedIron_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, aoRustedIron_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(start + distance + distance + distance, 0.0f, 2.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, delta_, glm::vec3(0.0f, 1.0f, 0.0f));
        shader_.setMat4("model", model);
        renderSphere();
        
        // Wall
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedoWall_->handle());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalWall_->handle());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallicWall_->handle());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughnessWall_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, aoWall_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(start + distance + distance + distance + distance, 0.0f, 2.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, delta_, glm::vec3(0.0f, 1.0f, 0.0f));
        shader_.setMat4("model", model);
        renderSphere();
        
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
        
        albedoGold_             = aha::Texture::New("textures/pbr/gold/albedo.png");
        normalGold_             = aha::Texture::New("textures/pbr/gold/normal.png");
        metallicGold_           = aha::Texture::New("textures/pbr/gold/metallic.png");
        roughnessGold_          = aha::Texture::New("textures/pbr/gold/roughness.png");
        aoGold_                 = aha::Texture::New("textures/pbr/gold/ao.png");
        
        albedoGrass_            = aha::Texture::New("textures/pbr/grass/albedo.png");
        normalGrass_            = aha::Texture::New("textures/pbr/grass/normal.png");
        metallicGrass_          = aha::Texture::New("textures/pbr/grass/metallic.png");
        roughnessGrass_         = aha::Texture::New("textures/pbr/grass/roughness.png");
        aoGrass_                = aha::Texture::New("textures/pbr/grass/ao.png");
        
        albedoPlastic_          = aha::Texture::New("textures/pbr/plastic/albedo.png");
        normalPlastic_          = aha::Texture::New("textures/pbr/plastic/normal.png");
        metallicPlastic_        = aha::Texture::New("textures/pbr/plastic/metallic.png");
        roughnessPlastic_       = aha::Texture::New("textures/pbr/plastic/roughness.png");
        aoPlastic_              = aha::Texture::New("textures/pbr/plastic/ao.png");
        
        albedoRustedIron_       = aha::Texture::New("textures/pbr/rusted_iron/albedo.png");
        normalRustedIron_       = aha::Texture::New("textures/pbr/rusted_iron/normal.png");
        metallicRustedIron_     = aha::Texture::New("textures/pbr/rusted_iron/metallic.png");
        roughnessRustedIron_    = aha::Texture::New("textures/pbr/rusted_iron/roughness.png");
        aoRustedIron_           = aha::Texture::New("textures/pbr/rusted_iron/ao.png");
        
        albedoWall_             = aha::Texture::New("textures/pbr/wall/albedo.png");
        normalWall_             = aha::Texture::New("textures/pbr/wall/normal.png");
        metallicWall_           = aha::Texture::New("textures/pbr/wall/metallic.png");
        roughnessWall_          = aha::Texture::New("textures/pbr/wall/roughness.png");
        aoWall_                 = aha::Texture::New("textures/pbr/wall/ao.png");
        
        glm::mat4 projection = glm::perspective(glm::radians(camera_.getZoom()), 1280.0f / 720.0f, 0.1f, 100.0f);
        shader_.use();
        shader_.setMat4("projection", projection);
        timer_.reset();
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
    std::shared_ptr <aha::Texture> albedoRustedIron_;
    std::shared_ptr <aha::Texture> normalRustedIron_;
    std::shared_ptr <aha::Texture> metallicRustedIron_;
    std::shared_ptr <aha::Texture> roughnessRustedIron_;
    std::shared_ptr <aha::Texture> aoRustedIron_;
    
    std::shared_ptr <aha::Texture> albedoGold_;
    std::shared_ptr <aha::Texture> normalGold_;
    std::shared_ptr <aha::Texture> metallicGold_;
    std::shared_ptr <aha::Texture> roughnessGold_;
    std::shared_ptr <aha::Texture> aoGold_;
    
    std::shared_ptr <aha::Texture> albedoGrass_;
    std::shared_ptr <aha::Texture> normalGrass_;
    std::shared_ptr <aha::Texture> metallicGrass_;
    std::shared_ptr <aha::Texture> roughnessGrass_;
    std::shared_ptr <aha::Texture> aoGrass_;
    
    std::shared_ptr <aha::Texture> albedoPlastic_;
    std::shared_ptr <aha::Texture> normalPlastic_;
    std::shared_ptr <aha::Texture> metallicPlastic_;
    std::shared_ptr <aha::Texture> roughnessPlastic_;
    std::shared_ptr <aha::Texture> aoPlastic_;
    
    std::shared_ptr <aha::Texture> albedoWall_;
    std::shared_ptr <aha::Texture> normalWall_;
    std::shared_ptr <aha::Texture> metallicWall_;
    std::shared_ptr <aha::Texture> roughnessWall_;
    std::shared_ptr <aha::Texture> aoWall_;
    
    static constexpr int rows_{7};
    static constexpr int columns_{7};
    static constexpr float spacing_{0.5f};
    glm::vec3 lightPositions_[1] =
    {
        glm::vec3(0.0f, 0.0f, 10.0f)
    };
    glm::vec3 lightColors_[1] =
    {
        glm::vec3(150.0f, 150.0f, 150.0f)
    };
    unsigned int sphereVAO_{0};
    unsigned int indexCount_{0};
    aha::Timer timer_{};
    float delta_ = 0.0f;
};
