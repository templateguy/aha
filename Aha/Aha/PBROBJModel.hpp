//
//  PBROBJModel.hpp
//  Aha
//
//  Created by Priyanshi Thakur on 09/05/18.
//  Copyright © 2018 Saurabh Sinha. All rights reserved.
//

#pragma once


#include "Texture.hpp"
#include "Shader.hpp"


namespace aha
{
    class PBROBJModel
    {
    public:
        void loadModel(const std::string& fileName)
        {
            base_ = getBaseDir_(fileName) + "/";
            parseFile_(fileName);
            printOBJModelInfo_();
            populateBuffer_();
            printMaxMin_();
            loadTextures();
            compileShader();
        }
        
        const glm::vec3& getPosition() const
        {
            return position_;
        }
        
        void setPosition(const glm::vec3& position)
        {
            position_ = position;
            hasCustomPosition_ = true;
        }
        
        void useShader() const
        {
            shader_.use();
        }
        
        const Shader& getShader() const
        {
            return shader_;
        }
        
        void render(bool wireframe = false) const
        {
            if(!wireframe)
            {
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, albedo_->handle());
                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, normal_->handle());
                glActiveTexture(GL_TEXTURE5);
                glBindTexture(GL_TEXTURE_2D, metallic_->handle());
                glActiveTexture(GL_TEXTURE6);
                glBindTexture(GL_TEXTURE_2D, roughness_->handle());
                glActiveTexture(GL_TEXTURE7);
                glBindTexture(GL_TEXTURE_2D, ao_->handle());
                
                for(const auto& mesh : meshes_)
                {
                    if(mesh.vbo < 1)
                    {
                        continue;
                    }
                    
                    shader_.use();
                    
                    // render the loaded model
                    glm::mat4 model;
                    
                    //build_rotmatrix_(model, currQuat);
                    model = glm::rotate(model, (-90.0f * 3.1415f) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                    
                    // Fit to -1, 1
                    float maxExtent(getMaxExtent());
                    model = glm::scale(model, glm::vec3(1.0f / maxExtent, 1.0f / maxExtent, 1.0f / maxExtent));
                    
                    // Centerize object
                    model = glm::translate(model, glm::vec3(-0.5 * (max_[0] + min_[0]), -0.5 * (max_[1] + min_[1]), -0.5 * (max_[2] + min_[2])));
                    
                    if(hasCustomPosition_)
                    {
                        model = glm::translate(model, position_);
                    }
                    
                    shader_.setMat4("model", model);
                    
                    glBindVertexArray(mesh.vao);
                    
                    glDrawArrays(GL_TRIANGLES, 0, 3 * static_cast <int> (mesh.numTriangles));
                    checkErrors("drawarrays");
                }
            }
        }
        
        const float* const getMax() const
        {
            return max_;
        }
        
        const float* const getMin() const
        {
            return min_;
        }
        
        float getMaxExtent() const
        {
            float maxExtent(0.5f * (max_[0] - min_[0]));
            if(maxExtent < 0.5f * (max_[1] - min_[1]))
            {
                maxExtent = 0.5f * (max_[1] - min_[1]);
            }
            if(maxExtent < 0.5f * (max_[2] - min_[2]))
            {
                maxExtent = 0.5f * (max_[2] - min_[2]);
            }
            return maxExtent;
        }
        
        void compileShader()
        {
            shader_.load("pbr/shaders/pbr.vs", "pbr/shaders/pbr.fs");
            shader_.use();
            shader_.setInt("irradianceMap", 0);
            shader_.setInt("prefilterMap", 1);
            shader_.setInt("brdfLUT", 2);
            shader_.setInt("albedoMap", 3);
            shader_.setInt("normalMap", 4);
            shader_.setInt("metallicMap", 5);
            shader_.setInt("roughnessMap", 6);
            shader_.setInt("aoMap", 7);
        }
        
        void loadTextures()
        {
            albedo_ = Texture::New(base_ + "albedo.tga");
            normal_ = Texture::New(base_ + "normal.tga");
            metallic_ = Texture::New(base_ + "metallic.tga");
            roughness_ = Texture::New(base_ + "roughness.tga");
            ao_ = Texture::New(base_ + "ao.tga");
        }
        
    protected:
        void parseFile_(const std::string& fileName)
        {
            std::string err;
            tinyobj::LoadObj(&attrib_, &shapes_, &materials_, &err, fileName.c_str(), base_.c_str());
            if(!err.empty())
            {
                std::cerr << err << std::endl;
            }
        }
        
        void printOBJModelInfo_()
        {
            printf("# of vertices  = %d\n", static_cast <int> (attrib_.vertices.size() / 3));
            printf("# of normals   = %d\n", static_cast <int> (attrib_.normals.size() / 3));
            printf("# of texcoords = %d\n", static_cast <int> (attrib_.texcoords.size() / 2));
            printf("# of materials = %d\n", static_cast <int> (materials_.size()));
            printf("# of shapes    = %d\n", static_cast <int> (shapes_.size()));
            for(size_t i = 0; i < materials_.size(); i++)
            {
                printf("material[%d].diffuse_texname = %s\n", int(i), materials_[i].diffuse_texname.c_str());
            }
        }
        
        void populateBuffer_()
        {
            size_t s{};
            for(auto& shape : shapes_)
            {
                Mesh_ mesh;
                std::vector <float> buffer;
                
                // Check for smoothing group and compute smoothing normals
                std::map <int, vec3_> smoothVertexNormals;
                if(hasSmoothingGroup_(shape) > 0)
                {
                    std::cout << "Compute smoothingNormal for shape [" << s << "]" << std::endl;
                    computeSmoothingNormals(attrib_, shape, smoothVertexNormals);
                }
                
                for(size_t f(0); f < shape.mesh.indices.size() / 3; ++f)
                {
                    tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
                    tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
                    tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];
                    
                    float tc[3][2];
                    if((attrib_.texcoords.size() <= 0) || (idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) || (idx2.texcoord_index < 0))
                    {
                        // face does not contain valid uv index.
                        tc[0][0] = 0.0f;
                        tc[0][1] = 0.0f;
                        tc[1][0] = 0.0f;
                        tc[1][1] = 0.0f;
                        tc[2][0] = 0.0f;
                        tc[2][1] = 0.0f;
                    }
                    else
                    {
                        assert(attrib_.texcoords.size() > size_t(2 * idx0.texcoord_index + 1));
                        assert(attrib_.texcoords.size() > size_t(2 * idx1.texcoord_index + 1));
                        assert(attrib_.texcoords.size() > size_t(2 * idx2.texcoord_index + 1));
                        
                        // Flip Y coord.
                        tc[0][0] = attrib_.texcoords[2 * idx0.texcoord_index];
                        tc[0][1] = 1.0f - attrib_.texcoords[2 * idx0.texcoord_index + 1];
                        tc[1][0] = attrib_.texcoords[2 * idx1.texcoord_index];
                        tc[1][1] = 1.0f - attrib_.texcoords[2 * idx1.texcoord_index + 1];
                        tc[2][0] = attrib_.texcoords[2 * idx2.texcoord_index];
                        tc[2][1] = 1.0f - attrib_.texcoords[2 * idx2.texcoord_index + 1];
                    }
                    
                    // Vertices
                    float v[3][3];
                    for (int k = 0; k < 3; k++)
                    {
                        int f0 = idx0.vertex_index;
                        int f1 = idx1.vertex_index;
                        int f2 = idx2.vertex_index;
                        assert(f0 >= 0);
                        assert(f1 >= 0);
                        assert(f2 >= 0);
                        
                        v[0][k] = attrib_.vertices[3 * f0 + k];
                        v[1][k] = attrib_.vertices[3 * f1 + k];
                        v[2][k] = attrib_.vertices[3 * f2 + k];
                        min_[k] = std::min(v[0][k], min_[k]);
                        min_[k] = std::min(v[1][k], min_[k]);
                        min_[k] = std::min(v[2][k], min_[k]);
                        max_[k] = std::max(v[0][k], max_[k]);
                        max_[k] = std::max(v[1][k], max_[k]);
                        max_[k] = std::max(v[2][k], max_[k]);
                    }
                    
                    // Normals
                    float n[3][3];
                    bool invalidNormalIndex(false);
                    if(attrib_.normals.size() > 0)
                    {
                        int nf0 = idx0.normal_index;
                        int nf1 = idx1.normal_index;
                        int nf2 = idx2.normal_index;
                        
                        if((nf0 < 0) || (nf1 < 0) || (nf2 < 0))
                        {
                            // normal index is missing from this face.
                            invalidNormalIndex = true;
                        }
                        else
                        {
                            for(int k = 0; k < 3; k++)
                            {
                                assert(size_t(3 * nf0 + k) < attrib_.normals.size());
                                assert(size_t(3 * nf1 + k) < attrib_.normals.size());
                                assert(size_t(3 * nf2 + k) < attrib_.normals.size());
                                n[0][k] = attrib_.normals[3 * nf0 + k];
                                n[1][k] = attrib_.normals[3 * nf1 + k];
                                n[2][k] = attrib_.normals[3 * nf2 + k];
                            }
                        }
                    }
                    else
                    {
                        invalidNormalIndex = true;
                    }
                    
                    if(invalidNormalIndex && !smoothVertexNormals.empty())
                    {
                        // Use smoothing normals
                        int f0 = idx0.vertex_index;
                        int f1 = idx1.vertex_index;
                        int f2 = idx2.vertex_index;
                        
                        if(f0 >= 0 && f1 >= 0 && f2 >= 0)
                        {
                            n[0][0] = smoothVertexNormals[f0].v[0];
                            n[0][1] = smoothVertexNormals[f0].v[1];
                            n[0][2] = smoothVertexNormals[f0].v[2];
                            
                            n[1][0] = smoothVertexNormals[f1].v[0];
                            n[1][1] = smoothVertexNormals[f1].v[1];
                            n[1][2] = smoothVertexNormals[f1].v[2];
                            
                            n[2][0] = smoothVertexNormals[f2].v[0];
                            n[2][1] = smoothVertexNormals[f2].v[1];
                            n[2][2] = smoothVertexNormals[f2].v[2];
                            
                            invalidNormalIndex = false;
                        }
                    }
                    if(invalidNormalIndex)
                    {
                        // compute geometric normal
                        calculateNormal_(n[0], v[0], v[1], v[2]);
                        n[1][0] = n[0][0];
                        n[1][1] = n[0][1];
                        n[1][2] = n[0][2];
                        n[2][0] = n[0][0];
                        n[2][1] = n[0][1];
                        n[2][2] = n[0][2];
                    }
                    
                    // Push values into buffer
                    for(int k = 0; k < 3; k++)
                    {
                        buffer.push_back(v[k][0]);
                        buffer.push_back(v[k][1]);
                        buffer.push_back(v[k][2]);
                        buffer.push_back(n[k][0]);
                        buffer.push_back(n[k][1]);
                        buffer.push_back(n[k][2]);
                        buffer.push_back(tc[k][0]);
                        buffer.push_back(tc[k][1]);
                    }
                }
                
                // Generate vbo
                mesh.vbo = 0;
                mesh.numTriangles = 0;
                
                if(buffer.size() > 0)
                {
                    glGenVertexArrays(1, &mesh.vao);
                    glGenBuffers(1, &mesh.vbo);
                    glBindVertexArray(mesh.vao);
                    
                    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
                    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer.at(0), GL_STATIC_DRAW);
                    
                    GLsizei stride = (3 + 3 + 2) * sizeof(float);
                    
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
                    // vertex normals
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
                    glEnableVertexAttribArray(2);
                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (6 * sizeof(float)));
                    
                    mesh.numTriangles = static_cast <int> (buffer.size()) / (3 + 3 + 2) / 3;  // 3:vtx, 3:normal, 3:col, 2:texcoord
                    
                    printf("shape[%d] # of triangles = %d\n", static_cast <int> (s), static_cast <int> (mesh.numTriangles));
                }
                
                meshes_.push_back(mesh);
                ++s;
            }
        }
        
        void printMaxMin_()
        {
            printf("bmin = %f, %f, %f\n", min_[0], min_[1], min_[2]);
            printf("bmax = %f, %f, %f\n", max_[0], max_[1], max_[2]);
        }
        
        static void calculateNormal_(float N[3], float v0[3], float v1[3], float v2[3])
        {
            const float v10[] = {v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]};
            const float v20[] = {v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]};
            
            N[0] = v20[1] * v10[2] - v20[2] * v10[1];
            N[1] = v20[2] * v10[0] - v20[0] * v10[2];
            N[2] = v20[0] * v10[1] - v20[1] * v10[0];
            
            const float len2(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
            if(len2 > 0.0f)
            {
                float len = sqrtf(len2);
                
                N[0] /= len;
                N[1] /= len;
                N[2] /= len;
            }
        }
        
        struct vec3_
        {
            float v[3] = {0.0f, 0.0f, 0.0f};
        };
        
        void normalizeVector_(vec3_& v)
        {
            float len2 = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
            if(len2 > 0.0f)
            {
                float len = sqrtf(len2);
                
                v.v[0] /= len;
                v.v[1] /= len;
                v.v[2] /= len;
            }
        }
        
        // Check if`mesh_t` contains smoothing group id.
        bool hasSmoothingGroup_(const tinyobj::shape_t& shape)
        {
            for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++)
            {
                if(shape.mesh.smoothing_group_ids[i] > 0)
                {
                    return true;
                }
            }
            return false;
        }
        
        void computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape, std::map<int, vec3_>& smoothVertexNormals)
        {
            smoothVertexNormals.clear();
            std::map <int, vec3_>::iterator iter;
            
            for(size_t f = 0; f < shape.mesh.indices.size() / 3; f++)
            {
                // Get the three indexes of the face (all faces are triangular)
                tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
                tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
                tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];
                
                // Get the three vertex indexes and coordinates
                int vi[3];      // indexes
                float v[3][3];  // coordinates
                
                for (int k = 0; k < 3; k++)
                {
                    vi[0] = idx0.vertex_index;
                    vi[1] = idx1.vertex_index;
                    vi[2] = idx2.vertex_index;
                    assert(vi[0] >= 0);
                    assert(vi[1] >= 0);
                    assert(vi[2] >= 0);
                    
                    v[0][k] = attrib.vertices[3 * vi[0] + k];
                    v[1][k] = attrib.vertices[3 * vi[1] + k];
                    v[2][k] = attrib.vertices[3 * vi[2] + k];
                }
                
                // Compute the normal of the face
                float normal[3];
                calculateNormal_(normal, v[0], v[1], v[2]);
                
                // Add the normal to the three vertexes
                for(size_t i = 0; i < 3; ++i)
                {
                    iter = smoothVertexNormals.find(vi[i]);
                    if(iter != smoothVertexNormals.end())
                    {
                        // add
                        iter->second.v[0] += normal[0];
                        iter->second.v[1] += normal[1];
                        iter->second.v[2] += normal[2];
                    }
                    else
                    {
                        smoothVertexNormals[vi[i]].v[0] = normal[0];
                        smoothVertexNormals[vi[i]].v[1] = normal[1];
                        smoothVertexNormals[vi[i]].v[2] = normal[2];
                    }
                }
            }
            
            // Normalize the normals, that is, make them unit vectors
            for(iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end(); iter++)
            {
                normalizeVector_(iter->second);
            }
        }
        
        void build_rotmatrix_(glm::mat4& m, const float q[4]) const
        {
            m[0][0] = 1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
            m[0][1] = 2.0 * (q[0] * q[1] - q[2] * q[3]);
            m[0][2] = 2.0 * (q[2] * q[0] + q[1] * q[3]);
            m[0][3] = 0.0;
            
            m[1][0] = 2.0 * (q[0] * q[1] + q[2] * q[3]);
            m[1][1] = 1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]);
            m[1][2] = 2.0 * (q[1] * q[2] - q[0] * q[3]);
            m[1][3] = 0.0;
            
            m[2][0] = 2.0 * (q[2] * q[0] - q[1] * q[3]);
            m[2][1] = 2.0 * (q[1] * q[2] + q[0] * q[3]);
            m[2][2] = 1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]);
            m[2][3] = 0.0;
            
            m[3][0] = 0.0;
            m[3][1] = 0.0;
            m[3][2] = 0.0;
            m[3][3] = 1.0;
        }
        
        static std::string getBaseDir_(const std::string& filepath)
        {
            if(filepath.find_last_of("/\\") != std::string::npos)
            {
                return filepath.substr(0, filepath.find_last_of("/\\"));
            }
            return "";
        }
        
        static void checkErrors(std::string desc)
        {
            GLenum e = glGetError();
            if(e != GL_NO_ERROR)
            {
                fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", desc.c_str(), e, e);
                exit(20);
            }
        }
        
        struct Mesh_
        {
            unsigned int vao{};
            unsigned int vbo{};
            size_t numTriangles{};
            size_t materialId{};
        };
        
        std::string base_{};
        
        std::vector <Mesh_> meshes_;
        
        std::shared_ptr <Texture> albedo_;
        std::shared_ptr <Texture> normal_;
        std::shared_ptr <Texture> metallic_;
        std::shared_ptr <Texture> roughness_;
        std::shared_ptr <Texture> ao_;
        
        // tinyobj objects
        tinyobj::attrib_t attrib_;
        std::vector <tinyobj::shape_t> shapes_;
        std::vector <tinyobj::material_t> materials_;
        float min_[3] = {std::numeric_limits <float>::max(), std::numeric_limits <float>::max(), std::numeric_limits <float>::max()};
        float max_[3] = {-std::numeric_limits <float>::max(), -std::numeric_limits <float>::max(), -std::numeric_limits <float>::max()};
        float currQuat[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        
        // Shader
        aha::Shader shader_;
        
        glm::vec3 position_{};
        bool hasCustomPosition_{false};
    };
}
