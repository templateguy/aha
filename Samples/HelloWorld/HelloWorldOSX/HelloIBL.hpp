//
//  HelloIBL.hpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 26/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <iostream>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <Aha.h>


class HelloIBL : public aha::Scene
{
public:
    void render() override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // render scene, supplying the convoluted irradiance map to the final shader.
        // ------------------------------------------------------------------------------------------
        pbrShader_.use();
        glm::mat4 model;
        glm::mat4 view = camera_.getViewMatrix();
        pbrShader_.setMat4("view", view);
        pbrShader_.setVec3("camPos", camera_.getPosition());
        
        // bind pre-computed IBL data
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap_);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap_);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture_);
        
        // rusted iron
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, albedoRustedIron_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, normalRustedIron_->handle());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, metallicRustedIron_->handle());
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, roughnessRustedIron_->handle());
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, aoRustedIron_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
        pbrShader_.setMat4("model", model);
        renderSphere_();
        
        // gold
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, albedoGold_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, normalGold_->handle());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, metallicGold_->handle());
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, roughnessGold_->handle());
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, aoGold_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-3.0, 0.0, 2.0));
        pbrShader_.setMat4("model", model);
        renderSphere_();
        
        // grass
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, albedoGrass_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, normalGrass_->handle());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, metallicGrass_->handle());
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, roughnessGrass_->handle());
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, aoGrass_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.0, 0.0, 2.0));
        pbrShader_.setMat4("model", model);
        renderSphere_();
        
        // plastic
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, albedoPlastic_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, normalPlastic_->handle());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, metallicPlastic_->handle());
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, roughnessPlastic_->handle());
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, aoPlastic_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0, 0.0, 2.0));
        pbrShader_.setMat4("model", model);
        renderSphere_();
        
        // wall
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, albedoWall_->handle());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, normalWall_->handle());
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, metallicWall_->handle());
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, roughnessWall_->handle());
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, aoWall_->handle());
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
        pbrShader_.setMat4("model", model);
        renderSphere_();
        
        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and
        // keeps the codeprint small.
        for (unsigned int i = 0; i < sizeof(lightPositions_) / sizeof(lightPositions_[0]); ++i)
        {
            glm::vec3 newPos = lightPositions_[i] + glm::vec3(sin(timer_.elapsed() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions_[i];
            pbrShader_.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            pbrShader_.setVec3("lightColors[" + std::to_string(i) + "]", lightColors_[i]);
            
            model = glm::mat4();
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            pbrShader_.setMat4("model", model);
            renderSphere_();
        }
        
        // render skybox (render as last to prevent overdraw)
        backgroundShader_.use();
        
        backgroundShader_.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap_);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
        //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
        renderCube_();
        
        // render BRDF map to screen
        //brdfShader.Use();
        //renderQuad();
    }
    
    AHA_CREATE_FUNC(HelloIBL);
    
protected:
    bool init() override
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        
        pbrShader_.load("shaders/ibl/pbr.vs", "shaders/ibl/pbr.fs");
        equirectangularToCubemapShader_.load("shaders/ibl/cubemap.vs", "shaders/ibl/equirectangular_to_cubemap.fs");
        irradianceShader_.load("shaders/ibl/cubemap.vs", "shaders/ibl/irradiance_convolution.fs");
        prefilterShader_.load("shaders/ibl/cubemap.vs", "shaders/ibl/prefilter.fs");
        brdfShader_.load("shaders/ibl/brdf.vs", "shaders/ibl/brdf.fs");
        backgroundShader_.load("shaders/ibl/background.vs", "shaders/ibl/background.fs");
        
        pbrShader_.use();
        pbrShader_.setInt("irradianceMap", 0);
        pbrShader_.setInt("prefilterMap", 1);
        pbrShader_.setInt("brdfLUT", 2);
        pbrShader_.setInt("albedoMap", 3);
        pbrShader_.setInt("normalMap", 4);
        pbrShader_.setInt("metallicMap", 5);
        pbrShader_.setInt("roughnessMap", 6);
        pbrShader_.setInt("aoMap", 7);
        
        backgroundShader_.use();
        backgroundShader_.setInt("environmentMap", 0);
        
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
        
        glGenFramebuffers(1, &fbo_);
        glGenRenderbuffers(1, &rbo_);
        
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);
        
        // pbr: load the HDR environment map
        // ---------------------------------
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrComponents;
        float *data = stbi_loadf("textures/hdr/newport_loft.hdr", &width, &height, &nrComponents, 0);
        if (data)
        {
            glGenTextures(1, &hdrTexture_);
            glBindTexture(GL_TEXTURE_2D, hdrTexture_);
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
        
        // pbr: setup cubemap to render to and attach to framebuffer
        // ---------------------------------------------------------
        glGenTextures(1, &envCubeMap_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap_);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
        // ----------------------------------------------------------------------------------------------
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };
        
        // pbr: convert HDR equirectangular environment map to cubemap equivalent
        // ----------------------------------------------------------------------
        equirectangularToCubemapShader_.use();
        equirectangularToCubemapShader_.setInt("equirectangularMap", 0);
        equirectangularToCubemapShader_.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture_);
        
        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        for (unsigned int i = 0; i < 6; ++i)
        {
            equirectangularToCubemapShader_.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap_, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            renderCube_();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap_);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
        // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
        // --------------------------------------------------------------------------------
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
        irradianceShader_.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap_);
        
        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader_.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap_, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            renderCube_();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
        // --------------------------------------------------------------------------------
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
        prefilterShader_.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap_);
        
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = 128 * std::pow(0.5, mip);
            unsigned int mipHeight = 128 * std::pow(0.5, mip);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);
            
            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterShader_.setFloat("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader_.setMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap_, mip);
                
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderCube_();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // pbr: generate a 2D LUT from the BRDF equations used.
        // ----------------------------------------------------
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
        
        
        // initialize static shader uniforms before rendering
        // --------------------------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera_.getZoom()), 1280.0f / 720.0f, 0.1f, 100.0f);
        pbrShader_.use();
        pbrShader_.setMat4("projection", projection);
        backgroundShader_.use();
        backgroundShader_.setMat4("projection", projection);
        
        // then before rendering, configure the viewport to the original framebuffer's screen dimensions
        int scrWidth, scrHeight;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &scrWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &scrHeight);
        glViewport(0, 0, scrWidth, scrHeight);
        
        timer_.reset();
        
        return true;
    }
    
    void renderSphere_()
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
    
    void renderCube_()
    {
        // initialize (if necessary)
        if (cubeVAO_ == 0)
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
            glGenVertexArraysAPPLE(1, &cubeVAO_);
            glGenBuffers(1, &cubeVBO_);
            // fill buffer
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // link vertex attributes
            glBindVertexArrayAPPLE(cubeVAO_);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArrayAPPLE(0);
        }
        // render Cube
        glBindVertexArrayAPPLE(cubeVAO_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArrayAPPLE(0);
    }
    
    void renderQuad_()
    {
        if(quadVAO_ == 0)
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
            glGenVertexArraysAPPLE(1, &quadVAO_);
            glGenBuffers(1, &quadVBO_);
            glBindVertexArray(quadVAO_);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArrayAPPLE(quadVAO_);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArrayAPPLE(0);
    }
    
    aha::Camera camera_{glm::vec3(0.0f, 0.0f, 3.0f)};
    aha::Shader pbrShader_;
    aha::Shader equirectangularToCubemapShader_;
    aha::Shader irradianceShader_;
    aha::Shader prefilterShader_;
    aha::Shader brdfShader_;
    aha::Shader backgroundShader_;
    
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
    
    std::shared_ptr <aha::Texture> albedoRustedIron_;
    std::shared_ptr <aha::Texture> normalRustedIron_;
    std::shared_ptr <aha::Texture> metallicRustedIron_;
    std::shared_ptr <aha::Texture> roughnessRustedIron_;
    std::shared_ptr <aha::Texture> aoRustedIron_;
    
    std::shared_ptr <aha::Texture> albedoWall_;
    std::shared_ptr <aha::Texture> normalWall_;
    std::shared_ptr <aha::Texture> metallicWall_;
    std::shared_ptr <aha::Texture> roughnessWall_;
    std::shared_ptr <aha::Texture> aoWall_;
    
    unsigned int envCubeMap_;
    unsigned int irradianceMap_;
    unsigned int prefilterMap_;
    unsigned int brdfLUTTexture_;
    unsigned int hdrTexture_;
    
    static constexpr int rows_{7};
    static constexpr int columns_{7};
    static constexpr float spacing_{0.5f};
    const glm::vec3 lightPositions_[4] =
    {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f)
    };
    const glm::vec3 lightColors_[4] =
    {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    unsigned int fbo_{0};
    unsigned int rbo_{0};
    unsigned int sphereVAO_{0};
    unsigned int indexCount_{0};
    unsigned int cubeVAO_{0};
    unsigned int cubeVBO_{0};
    unsigned int quadVAO_{0};
    unsigned int quadVBO_{0};
    
    aha::Timer timer_{};
};
