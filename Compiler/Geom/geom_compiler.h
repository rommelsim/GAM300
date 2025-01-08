#pragma once
/*!***************************************************************************
    \file			geom_compiler.h
    \author			Rommel sim
    \par			zhenpengrommel.sim\@digipen.edu
    \brief			Declarations and utilities for geometry compilation in
					the Aeon game engine. This file provides functionality
					related to processing and compiling geometry data.

    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define MAX_BONE_INFLUENCE 4

class AssimpGLMHelpers
{
public:
    static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
    {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }
    static inline glm::vec3 GetGLMVec(const aiVector3D& vec)
    {
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
    {
        return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
    }
};

namespace Geometry
{
    struct Vertex {
        // position
        float Position[3];
        // normal
        float Normal[3];
        // texCoords
        float TexCoords[2];
        // tangent
        float Tangent[3];
        // bitangent
        float Bitangent[3];
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];

        int EntID;
    };

    struct BoneInfo
    {
        int id;
        glm::mat4 offset;
    };

    struct BinaryMesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    static size_t bytesWritten;
    static std::vector<Vertex> vertices;
    static std::vector<uint32_t> indices;
    static BinaryMesh binaryMesh;
    extern bool ReadOBJ(const char* path);
    extern bool Compress(const char* location, std::vector<Vertex>& _vertices, std::vector<uint32_t>& _indices);
    
    
    // Helper func to read Animation 
    static std::map<std::string, BoneInfo> m_BoneInfoMap;
    static int m_BoneCounter;
    extern void ExtractBoneWeightForVertices(std::vector<Vertex>& _vertices, aiMesh* _mesh, const aiScene* _scene);
    extern void SetVertexBoneData(Vertex& _vertex, int _boneID, float _weight);
}

