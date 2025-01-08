
#ifndef R_MANAGER
#define R_MANAGER
/*****************************************************************//**
 * \file   ResourceManager.h
 * \brief  Handles all resources during runtime
 *		   Has container for textures, shaders, models, and meshes.
 * 
 *
 *
 * \author Rommel
 * \date   October 2023
 *
 * Copyright (C) 2023 Digipen Insitute Of Technology
 * Reproduction or disclosure of this file or its contents without
 * the prior consent of Digipen Insitute Of Technology is prohibited.
 *********************************************************************/

#include <Core/AeonAPI.h>
#include <Core/Singleton.hpp>

#include <audio/FmodCore.h>

#include <Resource/Texture.h>
#include <Resource/Shader.h>
#include <Resource/Mesh.h>
#include <Resource/Model.h>
#include <Resource/Material.h>
#include <Resource/FontResource.h>
#include "Core/Log.h"


namespace AeonCore
{
	class AEON_API ResourceManager : public Singleton<ResourceManager>
	{
	private:
		static std::map<std::string, Texture*>			m_textures;
		static std::map<std::string, Shader*>			m_shaders;
		static std::map<std::string, Model*>			m_models;
		static std::map<std::string, Mesh*>				m_meshes;
		static std::map<std::string, Material*>			m_materials;
		static std::map<std::string, FontResource*>		m_fonts;

		std::map<std::string, FMOD::Sound*> m_Sounds;

		std::shared_ptr<VertexArray>  m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer>  m_IndexBuffer;

		
	public:

		std::map<std::string, FMOD::Sound*> GetSound()
		{
			return m_Sounds;
		}

		enum class TextureType
		{
			Diffuse, Specular, Ambient
		};

		void Init();
		void Destroy();

		void LoadSound(std::string path)
		{
			FMODCoreClass& FmodInstance = FMODCoreClass::GetInstance();

			auto tFoundIt = m_Sounds.find(path);
			if (tFoundIt != m_Sounds.end())
			{
				// Sound already loaded, no need to reload
				return;
			}
			FMOD_MODE eMode = FMOD_DEFAULT;

			FMOD::Sound* pSound = nullptr;
			FMOD_RESULT result = FmodInstance.GetFMODSystem()->createSound(path.c_str(), eMode, nullptr, &pSound);

			if (FmodInstance.cErrorCheck(result) != FMOD_OK)
			{
				// Handle the error here (e.g., throw an exception or log it)
				AeonCore_ERROR("Failed to create sound: {}", static_cast<int>(result));
				return;
			}

			if (pSound)
			{
				m_Sounds[path] = pSound;
			}
		}

		Texture* LoadTexture(std::string name, std::vector<std::string> textureFiles, Texture::Type _type);
		Texture* LoadTexture(std::string name, std::string path, Texture::Type _type);
		Texture* GetTexture(std::string name);

		Shader* LoadShader(std::string name, std::string vertex, std::string fragment);
		Shader* LoadShader(std::string name, std::string vertex, std::string fragment, std::string geometry);
		Shader* GetShader(std::string name);

		Mesh* LoadMesh(std::string name, std::vector<float> vertices, std::vector<uint32_t> indices, BufferLayout layout,
			unsigned int _instancing = 1, std::vector<glm::mat4> instanceMatrix = {});
		Mesh* LoadMesh(std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices, BufferLayout layout,
			unsigned int _instancing = 1, std::vector<glm::mat4> instanceMatrix = {});
		Mesh* GetMesh(std::string name);
	
		Model* LoadModel(std::string name, std::string path, EntityID EntID);
		Model* GetModel(std::string name);

		Material* LoadMaterial(std::string name);
		Material* GetMaterial(std::string name);

		FontResource* LoadFont(std::string name, std::string path);
		FontResource* GetFont(std::string name);
	};
}

#endif // !R_MANAGER

