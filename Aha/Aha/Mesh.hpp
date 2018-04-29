//
//  Mesh.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 21/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <vector>
#include <string>
#include <OpenGL/gl.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace aha
{
    class Mesh
    {
    public:
        struct Vertex
        {
            glm::vec3 position;
            glm::vec2 texCoords;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec3 bitangent;
        };
        
        struct Texture
        {
            unsigned int id;
            std::string type;
            std::string path;
        };
        
        Mesh(const std::vector <Mesh::Vertex>& vertices, const std::vector <unsigned int>& indices, const std::vector <Mesh::Texture>& textures) : vertices_(vertices), indices_(indices), textures_(textures)
        {
            setup_();
        }
        
        Mesh(const std::vector <Mesh::Vertex>& vertices, const std::vector <unsigned int>& indices) : vertices_(vertices), indices_(indices)
        {
            setup_();
        }
        
        void render(unsigned int shaderId)
        {
            // bind appropriate textures
            unsigned int diffuseNr  = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr   = 1;
            unsigned int heightNr   = 1;
            for(unsigned int i = 0; i < textures_.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                std::string number;
                std::string name(textures_[i].type);
                if(name == "texture_diffuse")
                {
                    number = std::to_string(diffuseNr++);
                }
                else if(name == "texture_specular")
                {
                    number = std::to_string(specularNr++); // transfer unsigned int to stream
                }
                else if(name == "texture_normal")
                {
                    number = std::to_string(normalNr++); // transfer unsigned int to stream
                }
                else if(name == "texture_height")
                {
                    number = std::to_string(heightNr++); // transfer unsigned int to stream
                }
                
                // now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(shaderId, (name + number).c_str()), i);
                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, textures_[i].id);
            }
            
            // draw mesh
            glBindVertexArrayAPPLE(vao_);
            glDrawElements(GL_TRIANGLES, static_cast <int> (indices_.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArrayAPPLE(0);
            
            // always good practice to set everything back to defaults once configured.
            glActiveTexture(GL_TEXTURE0);
        }
        
        void render()
        {
            // draw mesh
            glBindVertexArrayAPPLE(vao_);
            glDrawElements(GL_TRIANGLES, static_cast <int> (indices_.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArrayAPPLE(0);
        }
        
    protected:
        void setup_()
        {
            glGenVertexArraysAPPLE(1, &vao_);
            glGenBuffers(1, &vbo_);
            glGenBuffers(1, &ebo_);
            glBindVertexArrayAPPLE(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);
            
            // vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            // vertex tangent
            /*glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
            // vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));*/
            
            glBindVertexArrayAPPLE(0);
        }
        
        std::vector <Mesh::Vertex> vertices_;
        std::vector <unsigned int> indices_;
        std::vector <Mesh::Texture> textures_;
        unsigned int vao_;
        unsigned int vbo_;
        unsigned int ebo_;
    };
}
