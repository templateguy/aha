//
//  HelloTriangle.hpp
//  HelloWorld
//
//  Created by Priyanshi Thakur on 08/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <iostream>
#include <OpenGL/gl.h>
#include <Aha.h>


class HelloTriangle : public aha::Scene
{
public:
    void render() override
    {
        glUseProgram(shaderProgram_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
    AHA_CREATE_FUNC(HelloTriangle);
    
protected:
    bool init() override
    {
        char* glslVersion = (char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
        std::cout << "GLSL Version: " << glslVersion << std::endl;
        glGenVertexArraysAPPLE(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);
        
        glBindVertexArrayAPPLE(vao_);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);
        
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
        
        return true;
    }
    
    float vertices_[12] =
    {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    
    unsigned int indices_[6] =
    {
        // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
    unsigned int vertexShader_;
    unsigned int fragmentShader_;
    unsigned int shaderProgram_;
    
    const char* vertexShaderSource_
    {
        "attribute vec3 aPos;"
        "void main()"
        "{"
            "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}"
    };
    const char* fragmentShaderSource_
    {
        "void main()"
        "{"
            "gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);"
        "}"
    };
};
