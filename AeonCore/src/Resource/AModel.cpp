#include "acpch.h"
/*!***************************************************************************
	\file			AModel.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\brief			Implementation of the AModel class. This class is
					responsible for loading 3D models using the ASSIMP library,
					processing their meshes, and rendering them. It supports
					features such as gamma correction, texture loading, and
					bone weight extraction for skeletal animation. The code
					also handles loading and processing of materials,
					including textures, and organizes the meshes in the model.
					The AModel class is used in the context of a game engine
					or graphics framework and is designed for OpenGL rendering.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Resource/AModel.h"

namespace AeonCore
{
	AModel::AModel(std::string const& _path, bool _gamma)
		: mGammaCorrection(_gamma)
	{
		loadModel(_path);
	}

	void AModel::Draw(Animator& _anima)
	{
		Camera& cam = CameraManager::GetInstance().GetActiveCamera();

		for (unsigned int i{}; i < mMeshes.size(); ++i)
		{
			mMeshes[i].Draw(cam, _anima);
		}
	}

	//	PRIVATE FUNCTIONS

	void AModel::loadModel(std::string const& _path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			AeonCore_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return;
		}
		// retrieve the directory path of the filepath
		mDirectory = _path.substr(0, _path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	void AModel::processNode(aiNode* _node, const aiScene* _scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < _node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
			mMeshes.push_back(processMesh(mesh, _scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < _node->mNumChildren; i++)
		{
			processNode(_node->mChildren[i], _scene);
		}
	}

	void AModel::SetVertexBoneDataToDefault(AVertex& _vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			_vertex.BoneIDs[i] = -1;
			_vertex.Weights[i] = 0.0f;
		}
	}

	AMesh AModel::processMesh(aiMesh* _mesh, const aiScene* _scene)
	{
		std::vector<AVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<ATexture> textures;

		for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
		{
			AVertex vertex{};
			SetVertexBoneDataToDefault(vertex);
			vertex.Position = AssimpGLMHelpers::GetGLMVec(_mesh->mVertices[i]);
			vertex.Normal = AssimpGLMHelpers::GetGLMVec(_mesh->mNormals[i]);

			if (_mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = _mesh->mTextureCoords[0][i].x;
				vec.y = _mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
		{
			aiFace face = _mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];

		std::vector<ATexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<ATexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<ATexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<ATexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		ExtractBoneWeightForVertices(vertices, _mesh, _scene);

		return AMesh(vertices, indices, textures);
	}

	void AModel::SetVertexBoneData(AVertex& _vertex, int _boneID, float _weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (_vertex.BoneIDs[i] < 0)
			{
				_vertex.Weights[i] = _weight;
				_vertex.BoneIDs[i] = _boneID;
				break;
			}
		}
	}

	void AModel::ExtractBoneWeightForVertices(std::vector<AVertex>& _vertices, aiMesh* _mesh, const aiScene* _scene)
	{
		auto& boneInfoMap = mBoneInfoMap;
		int& boneCount = mBoneCounter;

		for (unsigned int boneIndex = 0; boneIndex < _mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = _mesh->mBones[boneIndex]->mName.C_Str();
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				Geometry::BoneInfo newBoneInfo{};
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
			assert(boneID != -1);
			auto weights = _mesh->mBones[boneIndex]->mWeights;
			int numWeights = static_cast<int>(_mesh->mBones[boneIndex]->mNumWeights);

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				unsigned int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				assert(vertexId <= _vertices.size());
				SetVertexBoneData(_vertices[vertexId], boneID, weight);
			}
		}
	}

	unsigned AModel::TextureFromFile(const char* _path, const std::string& _directory, bool _gamma)
	{
		std::string filename = std::string(_path);

		//	extract filename from path
		std::filesystem::path path (filename);
		std::string file_name = path.filename().string();

		filename = _directory + '/' + file_name;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format{};
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			AeonCore_WARN("Texture failed to load at path:", _path);
			stbi_image_free(data);
		}

		return textureID;
	}

	std::vector<ATexture> AModel::loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName)
	{
		std::vector<ATexture> textures;
		for (unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
		{
			aiString str;
			_mat->GetTexture(_type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < mTextureLoaded.size(); j++)
			{
				if (std::strcmp(mTextureLoaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(mTextureLoaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				ATexture texture;
				texture.id = TextureFromFile(str.C_Str(), this->mDirectory);
				texture.type = _typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				mTextureLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			}
		}
		return textures;
	}
}
