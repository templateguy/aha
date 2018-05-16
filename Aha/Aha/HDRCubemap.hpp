//
//  HDRCubemap.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 12/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


namespace aha
{
    class HDRCubemap
    {
    public:
        HDRCubemap() = default;
        
        HDRCubemap(const std::string& fileName)
        {
            load(fileName);
        }
        
        void load(const std::string& fileName)
        {
            glDisable(GL_DEPTH_TEST);
            //glDepthFunc(GL_LEQUAL);
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
            buildAndCompileShaders_();
            setupFrameAndRenderBuffer_();
            loadHDREnvironmentMap_(fileName);
            setupCubemap_();
            convertEquirectangularToCubemap_();
            createIrradianceMap_();
            createPrefilterMap_();
            generate2DLUT_();
        }
        
        void useBackgroundShader() const
        {
            backgroundShader_.use();
        }
        
        const Shader& getBackgroundShader() const
        {
            return backgroundShader_;
        }
        
        unsigned int getIrradianceMap() const
        {
            return irradianceMap_;
        }
        
        unsigned int getPrefilterMap() const
        {
            return prefilterMap_;
        }
        
        unsigned int getBRDFLUTTexture() const
        {
            return brdfLUTTexture_;
        }
        
        void setProjectionMatrix(const glm::mat4& projection)
        {
            useBackgroundShader();
            backgroundShader_.setMat4("projection", projection);
        }
        
        void setViewMatrix(const glm::mat4& view) const
        {
            useBackgroundShader();
            backgroundShader_.setMat4("view", view);
        }
        
        void render() const
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_);
            renderCube_();
        }
        
    protected:
        void buildAndCompileShaders_()
        {
            equirectangularToCubemapShader_.load("pbr/shaders/cubemap.vs", "pbr/shaders/equirectangular_to_cubemap.fs");
            irradianceShader_.load("pbr/shaders/cubemap.vs", "pbr/shaders/irradiance_convolution.fs");
            prefilterShader_.load("pbr/shaders/cubemap.vs", "pbr/shaders/prefilter.fs");
            brdfShader_.load("pbr/shaders/brdf.vs", "pbr/shaders/brdf.fs");
            backgroundShader_.load("pbr/shaders/background.vs", "pbr/shaders/background.fs");
            
            backgroundShader_.use();
            backgroundShader_.setInt("environmentMap", 0);
        }
        
        void setupFrameAndRenderBuffer_()
        {
            glGenFramebuffers(1, &fbo_);
            glGenRenderbuffers(1, &rbo_);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);
        }
        
        void loadHDREnvironmentMap_(const std::string& fileName)
        {
            stbi_set_flip_vertically_on_load(true);
            int width, height, nrComponents;
            float* data = stbi_loadf(fileName.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                glGenTextures(1, &hdr_);
                glBindTexture(GL_TEXTURE_2D, hdr_);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Failed to load HDR image." << std::endl;
            }
        }
        
        void setupCubemap_()
        {
            glGenTextures(1, &cubemap_);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        
        void convertEquirectangularToCubemap_()
        {
            equirectangularToCubemapShader_.use();
            equirectangularToCubemapShader_.setInt("equirectangularMap", 0);
            equirectangularToCubemapShader_.setMat4("projection", captureProjection_);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, hdr_);
            
            glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
            for (unsigned int i = 0; i < 6; ++i)
            {
                equirectangularToCubemapShader_.setMat4("view", captureViews_[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap_, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                renderCube_();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        
        // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
        // --------------------------------------------------------------------------------
        void createIrradianceMap_()
        {
            glGenTextures(1, &irradianceMap_);
            glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap_);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
            
            // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
            // -----------------------------------------------------------------------------
            irradianceShader_.use();
            irradianceShader_.setInt("environmentMap", 0);
            irradianceShader_.setMat4("projection", captureProjection_);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_);
            
            glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
            for (unsigned int i = 0; i < 6; ++i)
            {
                irradianceShader_.setMat4("view", captureViews_[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap_, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                renderCube_();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        
        // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
        // --------------------------------------------------------------------------------
        void createPrefilterMap_()
        {
            glGenTextures(1, &prefilterMap_);
            glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap_);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            
            // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
            // ----------------------------------------------------------------------------------------------------
            prefilterShader_.use();
            prefilterShader_.setInt("environmentMap", 0);
            prefilterShader_.setMat4("projection", captureProjection_);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_);
            
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
            for (unsigned int mip = 0; mip < maxMipLevels_; ++mip)
            {
                // reisze framebuffer according to mip-level size.
                unsigned int mipWidth = 128 * std::pow(0.5, mip);
                unsigned int mipHeight = 128 * std::pow(0.5, mip);
                glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);
                
                float roughness = (float)mip / (float)(maxMipLevels_ - 1);
                prefilterShader_.setFloat("roughness", roughness);
                for (unsigned int i = 0; i < 6; ++i)
                {
                    prefilterShader_.setMat4("view", captureViews_[i]);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap_, mip);
                    
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    renderCube_();
                }
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        
        // pbr: generate a 2D LUT from the BRDF equations used.
        // ----------------------------------------------------
        void generate2DLUT_()
        {
            glGenTextures(1, &brdfLUTTexture_);
            
            // pre-allocate enough memory for the LUT texture.
            glBindTexture(GL_TEXTURE_2D, brdfLUTTexture_);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
            // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture_, 0);
            
            glViewport(0, 0, 512, 512);
            brdfShader_.use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderQuad_();
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        
        void renderCube_() const
        {
            static unsigned int vao(0);
            static unsigned int vbo(0);
            // initialize (if necessary)
            if(vao == 0)
            {
                float vertices[] =
                {
                    // back face
                    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                    1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                    1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                    1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                    // front face
                    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                    1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                    1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                    1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                    // left face
                    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                    // right face
                    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                    1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                    1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
                    1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                    1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
                    // bottom face
                    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                    1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                    1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                    1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                    // top face
                    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                    1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                    1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
                    1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
                };
                glGenVertexArrays(1, &vao);
                glGenBuffers(1, &vbo);
                // fill buffer
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                // link vertex attributes
                glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
            // render Cube
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
        
        // renderQuad() renders a 1x1 XY quad in NDC
        // -----------------------------------------
        void renderQuad_() const
        {
            static unsigned int vao(0);
            static unsigned int vbo(0);
            if (vao == 0)
            {
                float quadVertices[] =
                {
                    // positions        // texture Coords
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                };
                // setup plane VAO
                glGenVertexArrays(1, &vao);
                glGenBuffers(1, &vbo);
                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            }
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
        
        // Frame and Render buffers
        unsigned int fbo_;
        unsigned int rbo_;
        
        // Shaders
        aha::Shader backgroundShader_{};
        aha::Shader equirectangularToCubemapShader_{};
        aha::Shader irradianceShader_{};
        aha::Shader prefilterShader_{};
        aha::Shader brdfShader_{};
        
        unsigned int hdr_;
        
        unsigned int cubemap_;
        unsigned int irradianceMap_;
        unsigned int prefilterMap_;
        
        unsigned int brdfLUTTexture_;
        
        unsigned int maxMipLevels_ = 5;
        
        glm::mat4 captureProjection_ = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews_[18] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };
    };
}
