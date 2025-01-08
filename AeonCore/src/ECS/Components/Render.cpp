#include "acpch.h"

/*!***************************************************************************
	\file			Render.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage Render component
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Components/Render.h"

#include "Core/Application.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "Serialization/RegisterManager.hpp"

namespace AeonCore
{
	Render::Render(std::string name, EntityID entityID) : m_EntID(entityID)
	{
		id = Coordinator::GetInstance().GetComponentType<Render>();
		//m_model = ResourceManager::GetInstance().LoadModel(name, parent_path.string() + "/resources/objects/Cat1.bin", m_EntID);
		//m_material = ResourceManager::GetInstance().LoadMaterial(std::to_string (entityID));
		
		m_text = "Empty Text";
	}

	Render::Render(std::string mdlFile, std::string materialName, EntityID entID) :m_EntID(entID)
	{
		id = Coordinator::GetInstance().GetComponentType<Render>();
		m_text = "Empty Text";

		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning) {
			m_model = *ResourceManager::GetInstance().LoadModel(mdlFile,mdlFile, m_EntID);
			m_material = *ResourceManager::GetInstance().LoadMaterial(materialName);
		} else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID, modelPath = mdlFile, matName = materialName]() {
				Render& renderComp = Coordinator::GetInstance().GetComponent<Render>(entID);
				renderComp.m_model = *ResourceManager::GetInstance().LoadModel(modelPath,modelPath,entID);
				renderComp.m_material = *ResourceManager::GetInstance().LoadMaterial(matName);
			});
		}
	}

	void Render::Destroy()
	{
		
	}

	void Render::SetDiffuseTexture(std::string _path) {
		//m_material.SetDiffuseTexture(_path);
		
		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning || app.GetLoadStatus() == LoadStatus::LoadResources) {
			m_material.SetDiffuseTexture(_path);
		} else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID, texturePath = _path]() {
				Coordinator::GetInstance().GetComponent<Render>(entID).SetDiffuseTexture(texturePath);
			});
		}
	}

	void Render::SetSpecularTexture(std::string _path) {
		//m_material.SetSpecularTexture(_path);
		
		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning || app.GetLoadStatus() == LoadStatus::LoadResources) {
			m_material.SetSpecularTexture(_path);
		} else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID, texturePath = _path]() {
				Coordinator::GetInstance().GetComponent<Render>(entID).SetSpecularTexture(texturePath);
			});
		}
	}

	void Render::SetModel(std::string _path) {
		//m_model.SetModel(_path, m_EntID);

		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning || app.GetLoadStatus() == LoadStatus::LoadResources) {
			m_model.SetModel(_path, m_EntID);
		} else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID, modelPath = _path]() {
				Coordinator::GetInstance().GetComponent<Render>(entID).SetModel(modelPath);
			});
		}
	}



	//RTTR_REGISTRATION
	//{
	//	rttr::registration::class_<Render>("Render")
	//		.constructor<std::string const&, std::string const&>()() //constructor with 2 args
	//		.property_readonly("id", &Render::GetComponentID)
	//		.property("ambient_texture", &Render::GetAmbientTexture, &Render::SetAmbientTexture)
	//		.property("diffuse_texture", &Render::GetDiffuseTexture, &Render::SetDiffuseTexture)
	//		.property("specular_texture", &Render::GetSpecularTexture, &Render::SetSpecularTexture)
	//		.property("ambient", &Render::GetAmbient, &Render::SetAmbient)
	//		.property("diffuse", &Render::GetDiffuse, &Render::SetDiffuse)
	//		.property("specular", &Render::GetSpecular, &Render::SetSpecular)
	//		.property("shininess", &Render::GetShininess, &Render::SetShininess)
	//		.property("model_path", &Render::GetModelPath, &Render::SetModel)
	//		.property("model_name", &Render::GetModelName, &Render::SetModelName);
	//}
}


RTTR_REGISTRATION
{
	using namespace AeonCore;

RegisterManager::Instance().AddRegister([]() {
	/*rttr::registration::class_<Material>("Material")
		.constructor<std::string, glm::vec3, glm::vec3, glm::vec3>()()
		.property("ambient", &Material::GetAmbient, &Material::SetAmbient)
		.property("diffuse", &Material::GetDiffuse, &Material::SetDiffuse)
		.property("specular", &Material::GetSpecular, &Material::SetSpecular)
		.property("shininess", &Material::GetShininess, &Material::SetShininess)
		.property("ambient_texture_mat", &Material::GetAmbientTextureID, &Material::SetAmbientTexture)
		.property("diffuse_texture_mat", &Material::GetDiffuseTextureID, &Material::SetDiffuseTexture)
		.property("specular_texture_mat", &Material::GetSpecularTextureID, &Material::SetSpecularTexture);*/

	rttr::registration::class_<Render>("Render")
		.constructor()()
		.constructor<std::string,int>()()
		.constructor<std::string, std::string, int>()()
		.property_readonly("id", &Render::GetComponentID)
		.property("diffuse_texture", &Render::GetDiffuseTexture, &Render::SetDiffuseTexture)
		.property("specular_texture", &Render::GetSpecularTexture, &Render::SetSpecularTexture)
		.property("model_path", &Render::GetModelPath, &Render::SetModel); }, 8);

}