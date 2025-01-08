#pragma once
/*!***************************************************************************
	\file			AModel.h
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\brief			Definition of the AModel class. This class represents a 3D
					model and provides functionality for loading and rendering
					3D models using the ASSIMP library. It includes features
					such as gamma correction, texture loading, and bone weight
					extraction for skeletal animation. The class defines the
					model's data structures, including vertices, indices, and
					textures, and provides methods for rendering and processing
					the model's meshes. The AModel class is designed for use in
					a game engine or graphics framework, particularly in an
					OpenGL rendering context.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Resource/AMesh.h"

/*----- from compiler project -----*/
#include <Geom/geom_compiler.h>	

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace AeonCore
{
	class AModel
	{
	public:
		AModel(std::string const& _path, bool _gamma = false);

		void	Draw(Animator& _anima);

		auto&	GetBoneInfoMap() { return mBoneInfoMap; }
		int&	GetBoneCount()		{ return mBoneCounter; }
		
	private:
		std::vector<ATexture> mTextureLoaded;
		std::vector<AMesh> mMeshes;
		std::string mDirectory;
		bool mGammaCorrection = false;
		int mBoneCounter = 0;
		std::map<std::string, Geometry::BoneInfo> mBoneInfoMap;

		//	Functions

		void					loadModel(std::string const& _path);
		void					processNode(aiNode* _node, const aiScene* _scene);	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		void					SetVertexBoneDataToDefault(AVertex& _vertex);
		AMesh					processMesh(aiMesh* _mesh, const aiScene* _scene);
		void					SetVertexBoneData(AVertex& _vertex, int _boneID, float _weight);
		void					ExtractBoneWeightForVertices(std::vector<AVertex>& _vertices, aiMesh* _mesh, const aiScene* _scene);
		unsigned int			TextureFromFile(const char* _path, const std::string& _directory, bool _gamma = false);
		std::vector<ATexture>	loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName);	// checks all material textures of a given type and loads the textures if they're not loaded yet.
																													// the required info is returned as a Texture struct.
	};

}

