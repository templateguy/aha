//
//  HelloTexture.hpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 09/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <iostream>
#include <OpenGL/gl.h>
#include <Aha.h>


class HelloTexture : public aha::Scene
{
public:
    void render() override
    {
        glUseProgram(shaderProgram_);
        
        int ph(glGetAttribLocation(shaderProgram_, "aPos"));
        int ch(glGetAttribLocation(shaderProgram_, "aTexCoord"));
        int th(glGetUniformLocation(shaderProgram_, "uTexture"));
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture_->handle());
        glUniform1i(th, 0);
        
        // position attribute
        glVertexAttribPointer(ph, 2, GL_FLOAT, GL_FALSE, 0, vertices_);
        glEnableVertexAttribArray(ph);
        // texture coord attribute
        glVertexAttribPointer(ch, 2, GL_FLOAT, GL_FALSE, 0, textureCoords_);
        glEnableVertexAttribArray(ch);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        //glDisableVertexAttribArray(ph);
        //glDisableVertexAttribArray(ch);
        
    }
    
    AHA_CREATE_FUNC(HelloTexture);
    
protected:
    bool init() override
    {
        char* glslVersion = (char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
        std::cout << "GLSL Version: " << glslVersion << std::endl;
        
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_VERTEX_ARRAY, vbo_);
        glBufferData(GL_VERTEX_ARRAY, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
        
        texture_ = aha::Texture::New("container.jpg");
        
        vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader_, 1, &vertexShaderSource_, NULL);
        glCompileShader(vertexShader_);
        
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader_, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader_, 1, &fragmentShaderSource_, NULL);
        glCompileShader(fragmentShader_);
        
        glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader_, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        shaderProgram_ = glCreateProgram();
        glAttachShader(shaderProgram_, vertexShader_);
        glAttachShader(shaderProgram_, fragmentShader_);
        glLinkProgram(shaderProgram_);
        
        glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        }
        
        glDeleteShader(vertexShader_);
        glDeleteShader(fragmentShader_);
        
        glUseProgram(shaderProgram_);
        
        return true;
    }
    
    float vertices_[8] =
    {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f
    };
    
    float textureCoords_[8] =
    {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };
    
    unsigned int vbo_;
    std::shared_ptr <aha::Texture> texture_;
    unsigned int vertexShader_;
    unsigned int fragmentShader_;
    unsigned int shaderProgram_;
    
    const char* vertexShaderSource_
    {
        "attribute vec2 aPos;"
        "attribute vec2 aTexCoord;"
        "varying vec2 vTexCoord;"
        "void main()"
        "{"
            "gl_Position = vec4(aPos.x, aPos.y, 1.0, 1.0);"
            "vTexCoord = aTexCoord;"
        "}"
    };
    const char* fragmentShaderSource_
    {
        "varying vec2 vTexCoord;"
        "uniform sampler2D uTexture;"
        "void main()"
        "{"
            "gl_FragColor = texture2D(uTexture, vTexCoord);"
        "}"
    };
};
