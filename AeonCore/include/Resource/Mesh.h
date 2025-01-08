
#ifndef MESH_H
#define MESH_H
/*****************************************************************//**
 * \file   Mesh.h
 * \brief  Contain class to handle Mesh creation.
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/
#include "Core/AeonAPI.h"

#include <vector>

#define MAX_BONE_INFLUENCE 4
#include <glm/glm.hpp>

#include <Resource/Texture.h>
#include <Rendering/Buffers/Buffer.h>
#include <Rendering/Arrays/VertexArray.h>
#include <ECS/Components/Transform.h>
#include <ECS/Components/Camera.h>
#include "Resource/Shader.h"

namespace AeonCore
{

   
    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];

        int EntID;

        Vertex& operator=(const Vertex& vertex)
        {
            this->Position = vertex.Position;
            this->Normal = vertex.Normal;
            this->TexCoords = vertex.TexCoords;
            this->Tangent = vertex.Tangent;
            this->Bitangent = vertex.Bitangent;
            
            for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
            {
                this->m_BoneIDs[i] = vertex.m_BoneIDs[i];
                this->m_Weights[i] = vertex.m_Weights[i];
            }

            this->EntID = vertex.EntID;
            return *this;
        }
    };

   


    class AEON_API Mesh
    {
    private:
        std::vector<Vertex> mVertices;
        std::vector<uint32_t> mIndices;

    public:
        unsigned int instancing;

        Mesh() = delete;

        Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, BufferLayout layout, unsigned int _instancing = 1, std::vector<glm::mat4> instanceMatrix = {});
        Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures, BufferLayout layout = {}, unsigned int _instancing = 1, std::vector<glm::mat4> instanceMatrix = {});
        Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices, BufferLayout layout, unsigned int _instancing = 1, std::vector<glm::mat4> instanceMatrix = {});

        std::vector<Vertex> GetWorldSpaceVertices(Transform& transform);
        void Draw(Transform& transform, Camera& camera);
        void Draw();

        std::vector<Vertex>& GetVertices() { return mVertices; }
        std::vector<uint32_t>& GetIndices() { return mIndices; }
    public:
        std::shared_ptr<VertexArray>  m_VertexArray;                // VAO
        std::shared_ptr<VertexBuffer> m_VertexBuffer;               // VBO
        std::shared_ptr<VertexBuffer> m_InstanceVertexBuffer;       // VBO_Instancing
        std::shared_ptr<IndexBuffer>  m_IndexBuffer;                // EBO
        std::vector<Texture>          m_Textures;
    };

}


#endif // ! MESH_H
