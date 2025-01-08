#include "acpch.h"
/*****************************************************************//**
 * \file   Mesh.cpp
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

#include <Resource/Mesh.h>
#include <omp.h>

namespace AeonCore
{

    glm::vec4 multiplyVectorByMatrixParallel(const glm::vec4& vector, const glm::mat4& matrix) {
        glm::vec4 result(0.0f);

#pragma omp parallel for
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
#pragma omp atomic
                result[i] += matrix[j][i] * vector[j];
            }
        }

        return result;
    }

    // Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    //     :m_vertices(vertices), m_indices(indices), m_textures(textures)
    // {
    //     // create buffers/arrays
    //     glGenVertexArrays(1, &m_VAO);
    //     glGenBuffers(1, &m_VBO);
    // 
    //     if (indices.size() >= 1)
    //         glGenBuffers(1, &m_EBO);
    // 
    //     glBindVertexArray(m_VAO);
    //     glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    //     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
    // 
    //     if (indices.size() >= 1)
    //     {
    //         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    //         glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);
    //     }
    // 
    //     // set the vertex attribute pointers
    //     // vertex Positions
    //     glEnableVertexAttribArray(0);
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 
    //     // vertex normals
    //     glEnableVertexAttribArray(1);
    //     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 
    //     // vertex texture coords
    //     glEnableVertexAttribArray(2);
    //     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // 
    //     // vertex tangent
    //     glEnableVertexAttribArray(3);
    //     glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // 
    //     // vertex bitangent
    //     glEnableVertexAttribArray(4);
    //     glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // 
    //     // ids
    //     glEnableVertexAttribArray(5);
    //     glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    // 
    //     // weights
    //     glEnableVertexAttribArray(6);
    //     glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    //     glBindVertexArray(0);
    // 
    // 
    // 
    // }

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, BufferLayout layout, unsigned int _instancing, std::vector<glm::mat4> instanceMatrix)
        : mVertices(vertices), mIndices(indices)
    {
        m_VertexArray.reset(VertexArray::Create());
        m_VertexBuffer.reset(VertexBuffer::Create(vertices.data(), (uint32_t)(vertices.size() * sizeof(Vertex))));

        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        if (indices.size() > 0)
        {
            m_IndexBuffer.reset(IndexBuffer::Create(indices.data(), (uint32_t)indices.size()));
            m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        }


    }

    Mesh::Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices, BufferLayout layout,
        unsigned int _instancing, std::vector<glm::mat4> instanceMatrix)
    {
        instancing = _instancing;

        m_VertexArray.reset(VertexArray::Create());
        m_VertexBuffer.reset(VertexBuffer::Create(vertices.data(), (uint32_t)(vertices.size() * sizeof(float))));

        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        if (indices.size() > 0)
        {
            m_IndexBuffer.reset(IndexBuffer::Create(indices.data(), (uint32_t)indices.size()));
            m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        }

    }

    std::vector<Vertex> Mesh::GetWorldSpaceVertices(Transform& transform)
    {
        std::vector<Vertex> vertices;
        glm::mat4 mdl(1.0f);

        mdl = glm::translate(mdl, transform.GetPosition());

        // Apply Euler angle rotations
        mdl = glm::rotate(mdl, transform.GetEulerAngle().x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
        mdl = glm::rotate(mdl, transform.GetEulerAngle().y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
        mdl = glm::rotate(mdl, transform.GetEulerAngle().z, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

        mdl = glm::scale(mdl, transform.GetScale());

        for (Vertex& vertex : mVertices)
        {
            Vertex v;
            //v.Position = glm::vec3(mdl * glm::vec4(vertex.Position, 1));
            v = vertex;
            v.Position = multiplyVectorByMatrixParallel(glm::vec4(vertex.Position, 1), mdl);
            vertices.emplace_back(v);
        }

        return vertices;
    }


    Mesh::Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures, BufferLayout layout,
        unsigned int _intancing, std::vector<glm::mat4> instanceMatrix)
    {
        // 1. create vao
        // 2. create buffer with vertices.data(), vertices.size() * sizeof(T) 
        // 3. create layout 
        // 4. add vbo to vao
        // 5. create ebo 
        // 6. add ebo to vao

        m_VertexArray.reset(VertexArray::Create());
        m_VertexBuffer.reset(VertexBuffer::Create(vertices.data(), (uint32_t)(vertices.size() * sizeof(float))));

        // Is there a dynamic way to create the layout automatically once vertices.data() is valid?

        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        if (indices.size() > 0)
        {
            m_IndexBuffer.reset(IndexBuffer::Create(indices.data(), (uint32_t)(indices.size())));
            m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        }

        m_Textures = textures;
    }

    void Mesh::Draw()
    {
        // shader.use();
        m_VertexArray->Bind();
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
        m_VertexArray->UnBind();

    }

    void Mesh::Draw(Transform& transform, Camera& camera)
    {
        // shader.use();
        m_VertexArray->Bind();
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
        m_VertexArray->UnBind();

    }
}