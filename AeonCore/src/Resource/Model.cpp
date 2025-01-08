#include "acpch.h"
/*****************************************************************//**
 * \file   Model.cpp
 * \brief  Contains class to handle Model creation.
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include "Resource/Model.h"
#define ASSIMP 0

namespace AeonCore
{
  

    Model::Model(std::string name, std::string path, EntityID EntID, bool gamma) : directory(path), m_name(name)
    {
        (void)gamma;
#if ASSIMP
        // Use assimp to read object
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, 
            aiProcess_Triangulate | aiProcess_GenSmoothNormals  | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            AeonCore_INFO("Fail to load Model: {0}", path);
            //std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
#endif

        SetModel(path, EntID);
       
        //std::ifstream inFile(path, std::ios::binary);

        //// Read
        //BinaryMesh binaryMesh;
        //size_t numVertices{}, numIndices{};

        //// Dump
        //std::vector<Vertex> vertices;
        //std::vector<uint32_t> indices;
        //int seperator{};

        //if (inFile.is_open())
        //{
        //    inFile.read(reinterpret_cast<char*>(&numVertices), sizeof(size_t));
        //    inFile.read(reinterpret_cast<char*>(&numIndices), sizeof(size_t));
        //    {
        //        binaryMesh.vertices.resize(numVertices);
        //        binaryMesh.indices.resize(numIndices);

        //        for (int i = 0; i < numVertices; ++i)
        //        {
        //            if (inFile.read(reinterpret_cast<char*>(&binaryMesh.vertices[i]), sizeof(Vertex))) 
        //            {
        //                // Data read successfully for a single vertex
        //                vertices.push_back(binaryMesh.vertices[i]);
        //            }
        //            else 
        //            {
        //                AeonCore_INFO("Error reading vertex data from the binary file");
        //            }
        //        }

        //        // sperartr
        //        inFile.read(reinterpret_cast<char*>(&seperator), sizeof(int));

        //        for (int i = 0; i < numIndices; ++i)
        //        {
        //            if (inFile.read(reinterpret_cast<char*>(&binaryMesh.indices[i]), sizeof(uint32_t)))
        //            {
        //                // Data read successfully for a single vertex
        //                indices.push_back(binaryMesh.indices[i]);
        //            }
        //            else
        //            {
        //                AeonCore_INFO("Error reading index data from the binary file");
        //            }
        //        }
        //    }

        //    BufferLayout layout =
        //    {
        //        {ShaderDataType::Float3, "aPos"},
        //        {ShaderDataType::Float3, "aNormal"},
        //        {ShaderDataType::Float2, "aUV"},
        //        {ShaderDataType::Float3, "aTangent"},
        //        {ShaderDataType::Float3, "aBitTangent"},
        //        {ShaderDataType::Int4,   "aBoneId"},
        //        {ShaderDataType::Float4, "aWeights"},
        //        {ShaderDataType::Int,  "aEntityID"},

        //    };
        //    // auto f_vertices = ConvertVertexToFloatFormat(vertices, EntID);
        //    // m_meshes.push_back(Mesh(f_vertices, indices, layout));
        //    m_meshes.push_back(Mesh(vertices, indices, layout));
        //    AeonCore_INFO("Model Loaded: {0}", path);
        //}

        //else
        //{
        //    AeonCore_INFO("Model failed to load at {0}", path);
        //}
        
    }
        
    //void Model::processNode(aiNode* node, const aiScene* scene)
    //{
    //    // process each mesh located at the current node
    //    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    //    {
    //        aiMesh* tmp_mesh = scene->mMeshes[node->mMeshes[i]];
    //        m_meshes.push_back(processMesh(tmp_mesh, scene));
    //    }

    //    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    //    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    //    {
    //        processNode(node->mChildren[i], scene);
    //    }
    //}


    //Mesh Model::processMesh(aiMesh* ai_mesh, const aiScene* scene)
    //{
    //    // data to fill
    //    std::vector<Vertex> vertices;
    //    std::vector<Texture> textures;
    //    std::vector<unsigned int> indices;

    //    // Walk thro' all vertex and store them in vertices
    //    for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i)
    //    {
    //        Vertex vertex;

    //        // Get vertex position
    //        vertex.Position.x = ai_mesh->mVertices[i].x;
    //        vertex.Position.y = ai_mesh->mVertices[i].y;
    //        vertex.Position.z = ai_mesh->mVertices[i].z;

    //        // Get vertex normal
    //        if (ai_mesh->HasNormals())
    //        {
    //            vertex.Normal.x = ai_mesh->mNormals[i].x;
    //            vertex.Normal.y = ai_mesh->mNormals[i].y;
    //            vertex.Normal.z = ai_mesh->mNormals[i].z;
    //        }

    //        // Get vertex uv  
    //        if (ai_mesh->mTextureCoords[0])
    //        {
    //            // textures
    //            vertex.TexCoords.x = ai_mesh->mTextureCoords[0][i].x;
    //            vertex.TexCoords.y = ai_mesh->mTextureCoords[0][i].y;

    //            // tangent
    //            vertex.Tangent.x = ai_mesh->mTangents[i].x;
    //            vertex.Tangent.y = ai_mesh->mTangents[i].y;
    //            vertex.Tangent.z = ai_mesh->mTangents[i].z;

    //            // bittangent
    //            vertex.Bitangent.x = ai_mesh->mBitangents[i].x;
    //            vertex.Bitangent.y = ai_mesh->mBitangents[i].y;
    //            vertex.Bitangent.z = ai_mesh->mBitangents[i].z;

    //        }
    //        else
    //        {
    //            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    //        }

    //        // push back vertex
    //        vertices.push_back(vertex);
    //    }

    //    // Walk thro' all faces and store them in indices
    //    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i)
    //    {
    //        aiFace face = ai_mesh->mFaces[i];

    //        // retrieve all indices of the face and store them in the indices vector
    //        for (unsigned int j = 0; j < face.mNumIndices; ++j)
    //        {
    //            indices.push_back(face.mIndices[j]);
    //        }
    //    }

    //    // most of the .mtl only pushes in diffuse, specular and normal .png textures...

    //    aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];

    //    // 1. diffuse maps
    //    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    //   /*  2. specular maps*/
    //    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    //    /* 3. normal maps*/
    //    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    //    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    //    // 4. height maps
    //    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
    //    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    //    // return Mesh data

    //    BufferLayout layout =
    //    {
    //        {ShaderDataType::Float3, "aPos"},
    //        {ShaderDataType::Float3, "aNormal"},
    //        {ShaderDataType::Float2, "aUV"},
    //        {ShaderDataType::Float3, "aTangent"},
    //        {ShaderDataType::Float3, "aBitTangent"},
    //        {ShaderDataType::Int4,   "aBoneId"},
    //        {ShaderDataType::Float4, "aWeights"},

    //    };

    //    std::vector<float> f_vertices = ConvertVertexToFloatFormat(vertices);
    //    return Mesh(f_vertices, indices, textures, layout);
    //    //return Mesh(vertices, indices, textures);
    //}

    //std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    //{
    //    std::vector<Texture> textures;

    //    unsigned int texture_cnt = mat->GetTextureCount(type);      // GetTextureCount() returns 0 

    //    for (unsigned int i = 0; i < texture_cnt; ++i)
    //    {
    //        aiString str;
    //        mat->GetTexture(type, i, &str);
    //        // skip repeated texture that was previously loaded already
    //        bool skip = false;
    //        for (unsigned j = 0; j < m_loaded_textures.size(); ++j)
    //        {
    //            if (std::strcmp(m_loaded_textures[j].GetPath().data(), str.C_Str()) == 0)
    //            {
    //                textures.push_back(m_loaded_textures[j]);
    //                skip = true;
    //                break;
    //            }
    //        }

    //        // if is new, then load it.
    //        if (!skip)
    //        {
    //            auto tmp_path = this->directory + '/' + str.C_Str();
    //            Texture tex(str.C_Str(), tmp_path);
    //            tex.SetType(typeName);
    //            tex.SetPath(str.C_Str());
    //            textures.push_back(tex);
    //            m_loaded_textures.push_back(tex);
    //        }
    //    }
    //    return textures;
    //}

    std::vector<float> Model::ConvertVertexToFloatFormat(std::vector<Vertex> vertices, int EntID)
    {
       // { ShaderDataType::Float3, "aPos"},
       // { ShaderDataType::Float3, "aNormal" },
       // { ShaderDataType::Float2, "aUV" },
       // { ShaderDataType::Float3, "aTangent" },
       // { ShaderDataType::Float3, "aBitTangent" },
       // { ShaderDataType::Int4,    "aBoneid" },
       // { ShaderDataType::Float4, "aWeights" },


        std::vector<float> tmp;
        for (auto i = 0; i < vertices.size(); ++i)
        {
            // Push back aPos
            tmp.push_back(vertices[i].Position.x);
            tmp.push_back(vertices[i].Position.y);
            tmp.push_back(vertices[i].Position.z);

            // push back aNormal
            tmp.push_back(vertices[i].Normal.x);
            tmp.push_back(vertices[i].Normal.y);
            tmp.push_back(vertices[i].Normal.z);

            // push back uv
            tmp.push_back(vertices[i].TexCoords.x);
            tmp.push_back(vertices[i].TexCoords.y);

            // push back aTangent
            tmp.push_back(vertices[i].Tangent.x);
            tmp.push_back(vertices[i].Tangent.y);
            tmp.push_back(vertices[i].Tangent.z);

            // push back bittangent
            tmp.push_back(vertices[i].Bitangent.x);
            tmp.push_back(vertices[i].Bitangent.y);
            tmp.push_back(vertices[i].Bitangent.z);

            // push back bone id
            for (int j = 0; j < 4; ++j)
                tmp.push_back((float)(vertices[j].m_BoneIDs[j]));

            // push back aWeights
            for (int k = 0; k < 4; ++k)
                tmp.push_back(vertices[k].m_Weights[k]);

            tmp.push_back((float)EntID);

            
        }
    
        return tmp;
    }

    std::vector<Vertex> Model::AddEntityIDToVertex(std::vector<Vertex>& _vertices, EntityID Ent)
    {
        std::vector<Vertex> Vertices;
        for (auto i = 0; i < _vertices.size(); ++i)
        {
            Vertex vertex;
            vertex.Position     = _vertices[i].Position;
            vertex.Normal       = _vertices[i].Normal;
            vertex.TexCoords    = _vertices[i].TexCoords;
            vertex.Tangent      = _vertices[i].Tangent;
            vertex.Bitangent    = _vertices[i].Bitangent;

            for (int a = 0; a < MAX_BONE_INFLUENCE; ++a)
            {
                vertex.m_BoneIDs[a] = _vertices[i].m_BoneIDs[a];
                vertex.m_Weights[a] = _vertices[i].m_Weights[a];
            }

            vertex.EntID = ECSUtils::ExtractRealEntityId(Ent);

            Vertices.push_back(vertex);
        }

        return Vertices;
    }

}

