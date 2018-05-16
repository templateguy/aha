//
//  PBRQuad.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 12/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Shader.hpp"
#include "Texture.hpp"


namespace aha
{
    class PBRQuad
    {
    public:
        PBRQuad() = default;
        
        PBRQuad(const std::string& path)
        {
            load(path);
        }
        
        void load(const std::string& path)
        {
            createVAOAndVBO_();
            loadTextures_(path);
            compileShader_();
        }
        
        const glm::vec3& getPosition() const
        {
            return position_;
        }
        
        void setPosition(const glm::vec3& position)
        {
            position_ = position;
        }
        
        const glm::vec2& getSize() const
        {
            return size_;
        }
        
        void setSize(const glm::vec2& size)
        {
            size_ = size;
            createVAOAndVBO_();
        }
        
        void useShader() const
        {
            shader_.use();
        }
        
        const Shader& getShader() const
        {
            return shader_;
        }
        
        void setProjectionMatrix(const glm::mat4& projection)
        {
            useShader();
            shader_.setMat4("projection", projection);
        }
        
        void setViewMatrix(const glm::mat4& view) const
        {
            useShader();
            shader_.setMat4("view", view);
        }
        
        void setCameraPosition(const glm::vec3& camPos) const
        {
            useShader();
            shader_.setVec3("camPos", camPos);
        }
        
        void render() const
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, albedo_->handle());
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, normal_->handle());
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, metallic_->handle());
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, roughness_->handle());
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, ao_->handle());
            
            useShader();
            
            glm::mat4 model;
            model = glm::translate(model, position_);
            model = glm::rotate(model, (-45.0f * 3.1415f) / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            shader_.setMat4("model", model);
            
            glBindVertexArray(vao_);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
        
    protected:
        void createVAOAndVBO_()
        {
            if(vao_ || vbo_)
            {
                destroyVAOAndVBO_();
            }
            const float left(position_.x - (size_.x / 2.0f));
            const float right(position_.x + (size_.x / 2.0f));
            const float bottom(position_.y - (size_.y / 2.0f));
            const float top(position_.y + (size_.y / 2.0f));
            
            glm::vec3 v1(left, top, position_.z);
            glm::vec3 v2(left, bottom, position_.z);
            glm::vec3 v3(right, top, position_.z);
            glm::vec3 v4(right, bottom, position_.z);
            
            //glm::vec3 n1(glm::normalize(glm::cross(v3 - v1, v1 - v2)));
            //glm::vec3 n2(glm::normalize(glm::cross(v4 - v2, v1 - v2)));
            //glm::vec3 n3(glm::normalize(glm::cross(v3 - v1, v3 - v4)));
            //glm::vec3 n4(glm::normalize(glm::cross(v4 - v2, v3 - v4)));
            
            float quadVertices[] =
            {
                // positions      // normals        // texture Coords
                v1.x, v1.y, v1.z, v1.x, v1.y, 1.0f, 0.0f, 1.0f,
                v2.x, v2.y, v2.z, v2.x, v2.y, 1.0f, 0.0f, 0.0f,
                v3.x, v3.y, v3.z, v3.x, v3.y, 1.0f, 1.0f, 1.0f,
                v4.x, v4.y, v4.z, v4.x, v4.y, 1.0f, 1.0f, 0.0f
            };
            // setup plane VAO
            glGenVertexArrays(1, &vao_);
            glGenBuffers(1, &vbo_);
            glBindVertexArray(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        }
        
        void destroyVAOAndVBO_()
        {
            glDeleteVertexArrays(1, &vao_);
            glBindVertexArray(0);
            glDeleteBuffers(1, &vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            vao_ = vbo_ = 0;
        }
        
        void loadTextures_(const std::string& path)
        {
            albedo_ = Texture::New(path + "albedo.png");
            normal_ = Texture::New(path + "normal.png");
            metallic_ = Texture::New(path + "metallic.png");
            roughness_ = Texture::New(path + "roughness.png");
            ao_ = Texture::New(path + "ao.png");
        }
        
        void compileShader_()
        {
            shader_.load("pbr/shaders/pbr.vs", "pbr/shaders/pbr.fs");
            shader_.use();
            shader_.setInt("irradianceMap", 0);
            shader_.setInt("prefilterMap", 1);
            shader_.setInt("brdfLUT", 2);
            shader_.setInt("albedoMap", 3);
            shader_.setInt("normalMap", 4);
            shader_.setInt("metallicMap", 5);
            shader_.setInt("roughnessMap", 6);
            shader_.setInt("aoMap", 7);
        }
        
        unsigned int vao_{0};
        unsigned int vbo_{0};
        
        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::vec2 size_{2.0f, 2.0f};
        
        std::shared_ptr <Texture> albedo_;
        std::shared_ptr <Texture> normal_;
        std::shared_ptr <Texture> metallic_;
        std::shared_ptr <Texture> roughness_;
        std::shared_ptr <Texture> ao_;
        
        // Shader
        aha::Shader shader_;
    };
}
