//
//  Texture.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 10/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <iostream>
#include <OpenGL/gl.h>


namespace aha
{
    class Texture
    {
    public:
        using Handle = unsigned int;
        
        static std::shared_ptr <Texture> New(const std::string& fileName)
        {
            std::shared_ptr <Texture> texture;
            int width, height, channels;
            unsigned char* data(stbi_load(fileName.c_str(), &width, &height, &channels, 0));
            if(data)
            {
                GLenum format = GL_RGBA;
                if(channels == 1)
                {
                    format = GL_RED;
                }
                else if(channels == 3)
                {
                    format = GL_RGB;
                }
                else if(channels == 4)
                {
                    format = GL_RGBA;
                }
                texture = std::shared_ptr <Texture> (new Texture(data, width, height, format));
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
            return texture;
        }
        
        ~Texture()
        {
            glDeleteTextures(1, &handle_);
        }
        
        Handle handle() const
        {
            return handle_;
        }
        
    protected:
        Texture(unsigned char* data, int width, int height, GLenum format) : width_(width), height_(height), format_(format)
        {
            if(data)
            {
                glGenTextures(1, &handle_);
                glBindTexture(GL_TEXTURE_2D, handle_);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                width_ = width;
                height_ = height;
            }
        }
        
        Handle handle_;
        size_t width_;
        size_t height_;
        GLenum format_;
    };
}
