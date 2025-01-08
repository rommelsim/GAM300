#include "acpch.h"
/*****************************************************************//**
 * \file   ResourceManager.cpp
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

#include "Resource/ResourceManager.h"

namespace AeonCore
{
	std::map<std::string, Texture*>				ResourceManager::m_textures;
	std::map<std::string, Shader*>				ResourceManager::m_shaders;
	std::map<std::string, Model*>				ResourceManager::m_models;
	std::map<std::string, Mesh*>				ResourceManager::m_meshes;
	std::map<std::string, Material*>			ResourceManager::m_materials;
	std::map<std::string, FontResource*>		ResourceManager::m_fonts;

	void ResourceManager::Init()
	{
	}

	void ResourceManager::Destroy()
	{

		for (auto it = m_textures.begin(); it != m_textures.end(); it++)
		{
			delete it->second;
		}
		m_textures.clear();

		for (auto it = m_shaders.begin(); it != m_shaders.end(); it++)
		{
			delete it->second;
		}
		m_shaders.clear();

		for (auto it = m_models.begin(); it != m_models.end(); it++)
		{
			delete it->second;
		}
		m_models.clear();

		for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
		{
			delete it->second;
		}
		m_meshes.clear();

		for (auto it = m_materials.begin(); it != m_materials.end(); it++)
		{
			delete it->second;
		}
		m_materials.clear();

		for (auto it = m_fonts.begin(); it != m_fonts.end(); it++)
		{
			delete it->second;
		}
		m_fonts.clear();
	}


	Texture* ResourceManager::LoadTexture(std::string name, std::vector<std::string> textureFiles, Texture::Type _type)
	{
		auto find = m_textures.find(name);

		if (find == m_textures.end())
		{
			m_textures.emplace(name, std::make_unique<Texture>(textureFiles, _type).release());
			return m_textures[name];
		}

		return find->second;
	}

	Texture* ResourceManager::LoadTexture(std::string name, std::string path, Texture::Type _type)
	{
		auto find = m_textures.find(name);

		if (find == m_textures.end())
		{
			m_textures.emplace(name, std::make_unique<Texture>(path.c_str(), _type).release());
			return m_textures[name];
		}

		return find->second;
		
	}

	Texture* ResourceManager::GetTexture(std::string name)
	{
		for (const auto& obj : m_textures)
		{
			if (obj.first == name) return obj.second;
		}
		return 0;
	}

	Shader* ResourceManager::LoadShader(std::string name, std::string _vertex, std::string _fragment)
	{
		auto find = m_shaders.find(name);

		if (find == m_shaders.end())
		{
			m_shaders.emplace(name, std::make_unique<Shader>(_vertex.c_str(), _fragment.c_str()).release());
			return m_shaders[name];
		}

		return find->second;
	}

	Shader* ResourceManager::LoadShader(std::string name, std::string _vertex, std::string _fragment, std::string _geometry)
	{
		auto find = m_shaders.find(name);

		if (find == m_shaders.end())
		{
			m_shaders.emplace(name, new Shader(_vertex.c_str(), _fragment.c_str(), _geometry.c_str()));
			return m_shaders[name];
		}

		return find->second;
	}

	Shader* ResourceManager::GetShader(std::string name)
	{
		for (const auto& obj : m_shaders)
		{
			if (obj.first == name) return obj.second;
		}
		return 0;
	}

	Mesh* ResourceManager::LoadMesh(std::string name, std::vector<float> vertices, std::vector<uint32_t> indices, BufferLayout layout,
		unsigned int _instancing, std::vector<glm::mat4> instanceMatrix)
	{
		auto find = m_meshes.find(name);

		if (find == m_meshes.end())
		{
			m_meshes.emplace(name, std::make_unique<Mesh>(vertices, indices, layout, _instancing, instanceMatrix).release());
			return m_meshes[name];
		}

		return find->second;
	}

	Mesh* ResourceManager::LoadMesh(std::string name, std::vector<Vertex> vertices, std::vector<uint32_t> indices, BufferLayout layout,
		unsigned int _instancing, std::vector<glm::mat4> instanceMatrix)
	{
		auto find = m_meshes.find(name);

		if (find == m_meshes.end())
		{
			m_meshes.emplace(name, std::make_unique<Mesh>(vertices, indices, layout, _instancing, instanceMatrix).release());
			return m_meshes[name];
		}

		return find->second;
	}

	Mesh* ResourceManager::GetMesh(std::string name)
	{
		for (const auto& obj : m_meshes)
		{
			if (obj.first == name) return obj.second;
		}
		return 0;
	}

	Model* ResourceManager::LoadModel(std::string name, std::string path, EntityID EntID)
	{
		auto find = m_models.find(name);

		if (find == m_models.end())
		{
			// std::unique_ptr<Model> model = std::make_unique<Model>(path);
			m_models.emplace(name, std::make_unique<Model>(name, path, EntID).release());
			return m_models[name];
		}

		return find->second;
	}

	Model* ResourceManager::GetModel(std::string name)
	{
		for (const auto& obj : m_models)
		{
			if (obj.first == name) return obj.second;
		}
		return 0;
	}

	Material* ResourceManager::LoadMaterial(std::string name)
	{
		auto find = m_materials.find(name);

		if (find == m_materials.end())
		{
			auto insertResult = m_materials.emplace(name, std::make_unique<Material>().release());
			insertResult.first->second->SetName(name);
			return m_materials[name];
		}

		return find->second;
	}

	Material* ResourceManager::GetMaterial(std::string name)
	{
		for (const auto& obj : m_materials)
		{
			if (obj.first == name) return obj.second;
		}
		return 0;
	}

	FontResource* ResourceManager::LoadFont(std::string name, std::string path)
	{
		auto find = m_fonts.find(name);

		if (find == m_fonts.end())
		{
			m_fonts.emplace(name, std::make_unique<FontResource>(path.c_str()).release());
			return m_fonts[name];
		}

		return find->second;
	}

	FontResource* ResourceManager::GetFont(std::string name)
	{
		for (const auto& obj : m_fonts)
		{
			if (obj.first == name) return obj.second;
		}
		return 0;
	}

}
