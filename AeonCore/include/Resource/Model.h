
#ifndef MODEL_H
#define MODEL_H
/*****************************************************************//**
 * \file   Model.h
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

#include <vector>
#include <Resource/Mesh.h>
#include <Resource/Texture.h>
#include "Core/Log.h"

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

#include "Geom/geom_compiler.h"

namespace AeonCore
{
	class AEON_API Model
	{
	private:
        struct BinaryMesh
        {
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
        };

		std::vector<Mesh>		m_meshes;
		//std::vector<Texture>	m_loaded_textures;
		std::string directory;
		std::string m_name;

        

		//void processNode(aiNode* node, const aiScene* scene);
		//Mesh processMesh(aiMesh* ai_mesh, const aiScene* scene);
		//std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		std::vector<float> ConvertVertexToFloatFormat(std::vector<Vertex> vertices, int EntID);
        std::vector<Vertex> AddEntityIDToVertex(std::vector<Vertex>& vertices, EntityID Ent);

		std::map<std::string, Geometry::BoneInfo> mBoneInfoMap;
		int mBoneCounter = 0;

	public:
		Model() = default;
		Model(std::string name, std::string path, EntityID EntID, bool gamma = false);

		// Getter
		std::vector<Mesh>& GetMeshes()			{ return m_meshes; }
		void SetName(std::string name)			{ m_name = name; }
		std::string& GetName()					{ return m_name; }
		std::string GetDirectory()				{ return directory; }

		void SetModel(std::string path, EntityID EntID)
		{
            directory = path;
            m_meshes.clear();

            std::ifstream inFile(path, std::ios::binary);

            // Read
            BinaryMesh binaryMesh;
            size_t numVertices{}, numIndices{};

            // Dump
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            int seperator{};

            if (inFile.is_open())
            {
                inFile.read(reinterpret_cast<char*>(&numVertices), sizeof(size_t));
                inFile.read(reinterpret_cast<char*>(&numIndices), sizeof(size_t));
                {
                    binaryMesh.vertices.resize(numVertices);
                    binaryMesh.indices.resize(numIndices);

                    for (int i = 0; i < numVertices; ++i)
                    {
                        if (inFile.read(reinterpret_cast<char*>(&binaryMesh.vertices[i]), sizeof(Vertex)))
                        {
                            // Data read successfully for a single vertex
                            vertices.push_back(binaryMesh.vertices[i]);
                        }
                        else
                        {
                            AeonCore_INFO("Error reading vertex data from the binary file");
                        }
                    }

                    // sperartr
                    inFile.read(reinterpret_cast<char*>(&seperator), sizeof(int));

                    for (int i = 0; i < numIndices; ++i)
                    {
                        if (inFile.read(reinterpret_cast<char*>(&binaryMesh.indices[i]), sizeof(uint32_t)))
                        {
                            // Data read successfully for a single vertex
                            indices.push_back(binaryMesh.indices[i]);
                        }
                        else
                        {
                            AeonCore_INFO("Error reading index data from the binary file");
                        }
                    }
                }

                BufferLayout layout =
                {
                    {ShaderDataType::Float3, "aPos"},
                    {ShaderDataType::Float3, "aNormal"},
                    {ShaderDataType::Float2, "aUV"},
                    {ShaderDataType::Float3, "aTangent"},
                    {ShaderDataType::Float3, "aBitTangent"},
                    {ShaderDataType::Int4,   "aBoneId"},
                    {ShaderDataType::Float4, "aWeights"},
                    {ShaderDataType::Int,    "aEntityID"},          

                };
               // auto f_vertices = ConvertVertexToFloatFormat(vertices, EntID);

                vertices = AddEntityIDToVertex(vertices, EntID);
                m_meshes.push_back(Mesh(vertices, indices, layout));
                AeonCore_INFO("Model Loaded: {0}", path);
            }

            else
            {
                AeonCore_INFO("Model failed to load at {0}", path);
            }
		}
		//std::vector<Texture> GetTextures()	{ return m_loaded_textures; }
	};
}

#endif // !MODEL_H
