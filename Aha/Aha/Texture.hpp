//
//  Texture.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 10/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


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
                texture = std::shared_ptr <Texture> (new Texture(data, width, height, channels == 3? false : true));
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
        Texture(unsigned char* data, int width, int height, bool hasAlpha) : width_(width), height_(height), hasAlpha_(hasAlpha)
        {
            if(data)
            {
                glGenTextures(1, &handle_);
                glBindTexture(GL_TEXTURE_2D, handle_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                auto format(hasAlpha? GL_RGBA : GL_RGB);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                width_ = width;
                height_ = height;
                hasAlpha_ = hasAlpha;
            }
        }
        
        Handle handle_;
        size_t width_;
        size_t height_;
        bool hasAlpha_;
    };
}
