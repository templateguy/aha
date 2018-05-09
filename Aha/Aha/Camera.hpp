//
//  Camera.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 22/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace aha
{
    class Camera
    {
    public:
        Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f)
        : position_(position), worldUp_(up), yaw_(yaw), pitch_(pitch)
        {
            updateVectors_();
        }
        
        glm::mat4 getViewMatrix() const
        {
            return glm::lookAt(position_, position_ + front_, up_);
        }
        
        glm::vec3 getPosition() const
        {
            return position_;
        }
        
        void setPosition(const glm::vec3& position)
        {
            position_ = position;
        }
        
        float getZoom() const
        {
            return zoom_;
        }
        
        void setZoom(float zoom)
        {
            zoom_ = zoom;
        }
        
    protected:
        void updateVectors_()
        {
            // Calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
            front.y = sin(glm::radians(pitch_));
            front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
            front_ = glm::normalize(front);
            // Also re-calculate the Right and Up vector
            // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            right_ = glm::normalize(glm::cross(front_, worldUp_));
            up_ = glm::normalize(glm::cross(right_, front_));
        }
        
        glm::vec3 position_{glm::vec3(0.0f, 0.0f, 0.0f)};
        glm::vec3 worldUp_{glm::vec3(0.0f, 1.0f, 0.0f)};
        float yaw_{-90.0f};
        float pitch_{0.0f};
        glm::vec3 front_{glm::vec3(0.0f, 0.0f, 0.0f)};
        glm::vec3 up_{};
        glm::vec3 right_{};
        
        float speed_{2.5f};
        float sensitivity_{0.1f};
        float zoom_{45.0f};
    };
}
