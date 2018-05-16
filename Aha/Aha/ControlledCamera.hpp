//
//  ControlledCamera.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 12/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Camera.hpp"
#include "Event.hpp"


namespace aha
{
    class ControlledCamera : public Camera
    {
    public:
        ControlledCamera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f) : Camera(position, up, yaw, pitch)
        {
            Event.addListener <void (unsigned short, bool)> ("KeyDown", std::bind(&ControlledCamera::onKeyDown, this, std::placeholders::_1, std::placeholders::_2));
            Event.addListener <void (float, float)> ("MouseLeftButtonDown", std::bind(&ControlledCamera::onMouseLeftButtonDown, this, std::placeholders::_1, std::placeholders::_2));
            Event.addListener <void (float, float)> ("MouseLeftButtonDragged", std::bind(&ControlledCamera::onMouseLeftButtonDragged, this, std::placeholders::_1, std::placeholders::_2));
            Event.addListener <void (float, float)> ("MouseWheelScrolled", std::bind(&ControlledCamera::onMouseWheelScrolled, this, std::placeholders::_1, std::placeholders::_2));
        }
        
    protected:
        void onKeyDown(unsigned short keyCode, bool isRepeat)
        {
            float cameraSpeed = 0.05f; // adjust accordingly
            if(keyCode == 13)
            {
                position_ += cameraSpeed * front_;
            }
            if(keyCode == 1)
            {
                position_ -= cameraSpeed * front_;
            }
            if(keyCode == 0)
            {
                position_ -= glm::normalize(glm::cross(front_, up_)) * cameraSpeed;
            }
            if(keyCode == 2)
            {
                position_ += glm::normalize(glm::cross(front_, up_)) * cameraSpeed;
            }
        }
        
        void onMouseLeftButtonDown(float x, float y)
        {
            lastMouseX_ = x;
            lastMouseY_ = y;
        }
        
        void onMouseLeftButtonDragged(float x, float y)
        {
            float xoffset = lastMouseX_ - x;
            float yoffset = lastMouseY_ - y;
            lastMouseX_ = x;
            lastMouseY_ = y;
            
            xoffset *= sensitivity_;
            yoffset *= sensitivity_;
            
            yaw_   += xoffset;
            pitch_ += yoffset;
            
            if(pitch_ > 89.0f)
            {
                pitch_ = 89.0f;
            }
            if(pitch_ < -89.0f)
            {
                pitch_ = -89.0f;
            }
            updateVectors_();
        }
        
        void onMouseWheelScrolled(float deltaX, float deltaY)
        {
            if(zoom_ >= 1.0f && zoom_ <= 90.0f)
            {
                zoom_ -= deltaY;
            }
            if(zoom_ <= 1.0f)
            {
                zoom_ = 1.0f;
            }
            if(zoom_ >= 90.0f)
            {
                zoom_ = 90.0f;
            }
        }
        
        float lastMouseX_{};
        float lastMouseY_{};
    };
}
