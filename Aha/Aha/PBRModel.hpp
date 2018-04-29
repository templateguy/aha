//
//  PBRModel.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 28/04/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include <vector>
#include <string>
#include <OpenGL/gl3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace aha
{
    class PBRMesh
    {
    public:
        friend class PBRModel;
        
    protected:
        PBRMesh(const std::vector <glm::vec3>& positions, const std::vector <glm::vec2>& uvs, const std::vector <glm::vec3>& normals, const std::vector <unsigned int>& indices)
        {
            setup_(positions, uvs, normals, indices);
        }
        
        void render() const
        {
            glBindVertexArray(vao_);
            glDrawElements(GL_TRIANGLE_STRIP, indexCount_, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        
        void setup_(const std::vector <glm::vec3>& positions, const std::vector <glm::vec2>& uvs, const std::vector <glm::vec3>& normals, const std::vector <unsigned int>& indices)
        {
            // Create data
            std::vector <float> data;
            size_t size(positions.size());
            for(size_t i(0); i < size; ++i)
            {
                // Position
                data.push_back(positions[i].x);
                data.push_back(positions[i].y);
                data.push_back(positions[i].z);
                
                // UV
                if(uvs.size())
                {
                    data.push_back(uvs[i].x);
                    data.push_back(uvs[i].y);
                }
                
                // Normal
                if(normals.size())
                {
                    data.push_back(normals[i].x);
                    data.push_back(normals[i].y);
                    data.push_back(normals[i].z);
                }
            }
            unsigned int vbo{};
            unsigned int ebo{};
            glGenVertexArrays(1, &vao_);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);
            glBindVertexArray(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
            indexCount_ = static_cast <int> (indices.size());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount_ * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
            float stride = (3 + 2 + 3) * sizeof(float);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
        }
        
        unsigned int vao_{};
        int indexCount_{0};
    };
    
    class PBRModel
    {
    public:
        PBRModel() = default;
        
        PBRModel(const std::string& fileName)
        {
            load(fileName);
        }
        
        void load(const std::string& path)
        {
            Assimp::Importer import;
            const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
                return;
            }
            processNode_(scene->mRootNode, scene);
        }
        
        void render() const
        {
            for(auto mesh : meshes_)
            {
                mesh.render();
            }
        }
        
    protected:
        void processNode_(aiNode* node, const aiScene* scene)
        {
            // process all the node's meshes (if any)
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes_.push_back(processMesh_(mesh, scene));
            }
            // then do the same for each of its children
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode_(node->mChildren[i], scene);
            }
        }
        
        PBRMesh processMesh_(aiMesh* mesh, const aiScene* scene)
        {
            // data to fill
            std::vector <glm::vec3> positions;
            std::vector <glm::vec2> uvs;
            std::vector <glm::vec3> normals;
            std::vector<unsigned int> indices;
            
            // Walk through each of the mesh's vertices
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                positions.push_back(vector);
                // normals
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                normals.push_back(vector);
                // texture coordinates
                glm::vec2 uv(0.0f, 0.0f);
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    uv.x = mesh->mTextureCoords[0][i].x;
                    uv.y = mesh->mTextureCoords[0][i].y;
                    uvs.push_back(uv);
                }
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            return PBRMesh(positions, uvs, normals, indices);
        }
        
        std::vector <PBRMesh> meshes_;
    };
}
