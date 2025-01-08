#include "acpch.h"

/*!***************************************************************************
	\file			Material.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage Material properties for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <Resource/Material.h>
#include "Serialization/RegisterManager.hpp"
#include "Resource/ResourceManager.h"

namespace AeonCore
{

	Material::Material() : 
		m_name{}
	{
		std::filesystem::path working_dir(std::filesystem::current_path());
		auto parent_path = working_dir;
		m_diffuseTexturePtr = *ResourceManager::GetInstance().LoadTexture("DefaultDiffuseTexture", parent_path.string() + "/resources/textures/container.dds", Texture::Type::Diffuse);
		m_specularTexturePtr = *ResourceManager::GetInstance().LoadTexture("DefaultSpecularTexture", parent_path.string() + "/resources/textures/specular.dds", Texture::Type::Specular);
	}
	Material::Material(std::string name)
		:m_name(name)
	{
		std::filesystem::path working_dir(std::filesystem::current_path());
		auto parent_path = working_dir;
		
		m_diffuseTexturePtr = *ResourceManager::GetInstance().LoadTexture("DefaultDiffuseTexture", parent_path.string() + "/resources/textures/container.dds", Texture::Type::Diffuse);
		m_specularTexturePtr = *ResourceManager::GetInstance().LoadTexture("DefaultSpecularTexture", parent_path.string() + "/resources/textures/specular.dds", Texture::Type::Specular);
	}

	void Material::Use(Shader& program)
	{

		AeonCore_ASSERT(program.ID, "program null");
		 // Program is valid...
		if (program.ID != 0)
		{
			glActiveTexture(GL_TEXTURE0 + diffuse_slot);
			glBindTexture(GL_TEXTURE_2D, m_diffuseTexturePtr.GetTextureID());
			program.setInt("material.diffuseTexID", diffuse_slot);

			glActiveTexture(GL_TEXTURE0 + specular_slot);
			glBindTexture(GL_TEXTURE_2D, m_specularTexturePtr.GetTextureID());
			program.setInt("material.specularTexID", specular_slot);

			program.setFloat("material.shininess", m_shininess);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}

	}

	void Material::SetShininess(float _shiniess) 
	{
		m_shininess = _shiniess;
	}

	float Material::GetShininess()
	{
		return m_shininess;
	}

	void Material::SetDiffuseTexture(std::string _path)
	{
		m_diffuseTexturePtr = *ResourceManager::GetInstance().LoadTexture(_path,  _path, Texture::Type::Diffuse);
	}

	void Material::SetSpecularTexture(std::string _path)
	{
		m_specularTexturePtr = *ResourceManager::GetInstance().LoadTexture(_path, _path, Texture::Type::Specular);
	}



	std::string Material::GetDiffuseTexture()
	{
		return m_diffuseTexturePtr.GetPath();
	}

	std::string Material::GetSpecularTexture()
	{
		return m_specularTexturePtr.GetPath();
	}

	std::string Material::GetName()
	{
		return m_name;
	}

	void Material::SetName(std::string name)
	{
		m_name = name;
	}

	//RTTR_REGISTRATION
	//{
	//	RegisterManager::In().addRegister([]() {
	//	rttr::registration::class_<Material>("Material")
	//		.constructor()()
	//		//.constructor<std::string, glm::vec3, glm::vec3, glm::vec3>()()
	//		.property("material_name", &Material::GetName, &Material::SetName)
	//		.property("Diffuse_Texture", &Material::GetDiffuseTexture, &Material::SetDiffuseTexture)
	//		.property("Specular_Texture", &Material::GetSpecularTexture, &Material::SetSpecularTexture);
	//	}, 2);
	//}

}