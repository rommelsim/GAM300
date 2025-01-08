/*!***************************************************************************
    \file			geom_compiler.cpp
    \author			Rommel sim
    \par			zhenpengrommel.sim\@digipen.edu
    \brief			Definitions and utilities for geometry compilation in
                    the Aeon game engine. This file provides functionality
                    related to processing and compiling geometry data.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "geom_compiler.h"

bool Geometry::ReadOBJ(const char* path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices);
    if (!scene)
    {
        std::cout << "Error reading " << path << std::endl;
        return false;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        std::cout << "Cild: " << scene->mRootNode->mNumChildren << "Meshes count: " << scene->mRootNode->mNumMeshes << std::endl;
        aiMesh *mesh = scene->mMeshes[i];
        if (!mesh) return false;

        auto vert_count = vertices.size();
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
        {
            Vertex vertex;
            if (mesh->HasPositions())
            {
                aiVector3D position = mesh->mVertices[j];
                vertex.Position[0] = position.x;
                vertex.Position[1] = position.y;
                vertex.Position[2] = position.z;
            }
            if (mesh->HasNormals())
            {
                aiVector3D normal = mesh->mNormals[j];
                vertex.Normal[0] = normal.x;
                vertex.Normal[1] = normal.y;
                vertex.Normal[2] = normal.z;
            }
            if (mesh->HasTextureCoords(0))
            {
                aiVector3D texCoord = mesh->mTextureCoords[0][j];
                vertex.TexCoords[0] = texCoord.x;
                vertex.TexCoords[1] = texCoord.y;
            }
            if (mesh->HasTangentsAndBitangents())
            {
                std::cout << "Has Tangent\n";
                aiVector3D tangent = mesh->mTangents[i];
                vertex.Tangent[0] = tangent.x;
                vertex.Tangent[1] = tangent.y;
                vertex.Tangent[2] = tangent.z;

                aiVector3D bit_tangent = mesh->mBitangents[i];
                vertex.Bitangent[0] = bit_tangent.x;
                vertex.Bitangent[1] = bit_tangent.y;
                vertex.Bitangent[2] = bit_tangent.z;
            }
            if (mesh->HasBones())
            {
                std::cout << "Has Bones\n";
                for (int u = 0; u < MAX_BONE_INFLUENCE; ++i)
                {
                    aiBone* bone = mesh->mBones[u];

                    // Get bone ID (idx) and weight for this influence
                    vertex.m_BoneIDs[u] = bone->mWeights[u].mVertexId;
                    vertex.m_Weights[u] = bone->mWeights[u].mWeight;

                }
            }
            vertices.push_back(vertex);
        }

        // Loop through indices (for indexed rendering)
        for (unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            aiFace face = mesh->mFaces[j];

            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                unsigned int index = static_cast<unsigned int>(vert_count + face.mIndices[k]);

                // Process index
                indices.push_back(index);

            }
        }
        ExtractBoneWeightForVertices(vertices, mesh, scene);
    }
    return true;
}

bool Geometry::Compress(const char* location, std::vector<Vertex>& _vertices, std::vector<uint32_t>& _indices)
{
    // seperator
    int seperator = 10;

    // Assign read data into binaryMesh. get ready to export bin file.
    binaryMesh.vertices = vertices;
    binaryMesh.indices = indices;

    std::string output = std::string(location) + ".bin";
    std::ofstream out(output, std::ios::binary);

    if (out.is_open())
    {
        size_t numVertices = binaryMesh.vertices.size();
        size_t numIndices = binaryMesh.indices.size();
        std::cout << "Vertex Count: " << numVertices << std::endl;
        std::cout << "Index Count: " << numIndices << std::endl;
        out.write(reinterpret_cast<char*>(&numVertices), sizeof(size_t));
        out.write(reinterpret_cast<char*>(&numIndices), sizeof(size_t));

        // Write vertices
        for (Vertex& vertex : binaryMesh.vertices)
        {
            out.write(reinterpret_cast<char*>(&vertex), sizeof(Vertex));
        }

        // sperator
        out.write(reinterpret_cast<char*>(&seperator), sizeof(int));

        // Write indices
        for (uint32_t& index : binaryMesh.indices)
        {
            out.write(reinterpret_cast<char*>(&index), sizeof(uint32_t));
        }


        out.close();
        return true;
    }
    else
    {
        std::cout << "Error writing to " << output << "\n";
        return false;
    }
}

void Geometry::ExtractBoneWeightForVertices(std::vector<Vertex>& _vertices, aiMesh* _mesh, const aiScene* _scene)
{
    auto& boneInfoMap = m_BoneInfoMap;
    int& boneCount = m_BoneCounter;

    for (unsigned int boneIndex = 0; boneIndex < _mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = _mesh->mBones[boneIndex]->mName.C_Str();

        // If boneInfo is new, allocate .
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = boneCount;
            newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(_mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
            boneID = boneCount;
            boneCount++;
        }
        else
        {
            boneID = boneInfoMap[boneName].id;
        }
        if (boneID < 0) std::cout << "Bone ID != -1\n";
        auto weights = _mesh->mBones[boneIndex]->mWeights;
        int numWeights = _mesh->mBones[boneIndex]->mNumWeights;
        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            if (vertexId <= vertices.size()) std::cout << "vertex id <= vertices size\n";
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

void Geometry::SetVertexBoneData(Vertex& _vertex, int _boneID, float _weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (_vertex.m_BoneIDs[i] < 0)
        {
            _vertex.m_Weights[i] = _weight;
            _vertex.m_BoneIDs[i] = static_cast<int>(_weight);
            break;
        }
    }
}
