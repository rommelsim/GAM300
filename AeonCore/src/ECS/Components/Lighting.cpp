#include "acpch.h"
/*!***************************************************************************
	\file			Lighting.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage Lighting for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Components/Lighting.h"
#include "Serialization/RegisterManager.hpp"
#include "Resource/ResourceManager.h"
#include "Camera/CameraManager.h"
#include "Core/Application.h"

namespace AeonCore
{

	std::unordered_map<EntityID, Lighting*> LightManager::Lights;
	std::shared_ptr<OpenGLUniformBuffer> LightManager::mUniformBuffer;
	glm::vec3 LightManager::AmbientLightPosition = glm::vec3(0.5, 0.5, 0.5);

	Lighting::Lighting() : m_position(glm::vec3(0, 0, 0)), m_color(glm::vec3(1, 1, 1)), m_light(LightType::SPOTLIGHT), m_direction(0.0f, -1.0f, 0.0f), m_intensity(1)
	{
		// Used only for ECS init
	}

	Lighting::Lighting(EntityID EntID, glm::vec3 _position, glm::vec3 _color, glm::vec3 _direction, short _intensity, short _cutoff, short _outercuttoff) :
		m_position(_position), m_color(_color), m_light(LightType::SPOTLIGHT), m_direction(_direction), m_intensity(_intensity), m_entID(EntID),
		m_Cutoff(_cutoff), m_outerCutoff(_outercuttoff)
	{
		if (Application::GetApplication().GetLoadStatus() == LoadStatus::NotRunning)
			LightManager::GetInstance().Add(m_entID, this);
	}

	void Lighting::Destroy()
	{
		LightManager::GetInstance().Remove(m_entID);
	}

	Lighting& Lighting::operator=(const Lighting& rhs)
	{
		// Prevent self-assignemnt
		if (this != &rhs)
		{
			this->m_entID = rhs.m_entID;
			this->m_position = rhs.m_position;
			this->m_color = rhs.m_color;
			this->m_direction = rhs.m_direction;
			this->m_intensity = rhs.m_intensity;
			this->m_light = rhs.m_light;
		}

		return *this;
	}

	struct gpu
	{
		std::array<Lighting::Spotlight, Lighting::MAX_LIGHTS> spotlight;
		std::array<Lighting::Pointlight, Lighting::MAX_LIGHTS> pointlight;
		std::array<Lighting::Directionallight, Lighting::MAX_LIGHTS> directionallight;
		glm::vec3 ambience;

		int size_spotLight;
		int size_pointLight;
		int size_directionalLight;

		glm::vec3 viewPos;
	};

	void LightManager::Init()
	{
		// Creating UBO for Lighting
		Shader* shader = ResourceManager::GetInstance().GetShader("AEON_MODEL_SHADER");
		mUniformBuffer = std::make_shared<OpenGLUniformBuffer>(sizeof(gpu));
		mUniformBuffer->AttachShaderUniformBlock(*shader, "Lighting", OpenGLUniformBuffer::BLOCK_BINDING_POINT::LIGHTING);
	}

	void LightManager::Update()
	{

		Coordinator& coord = Coordinator::GetInstance();
		for (auto id : mEntities)
		{
			{
				Lighting& light = coord.GetComponent<Lighting>(id);
				Transform& trans = coord.GetComponent<Transform>(id);
				LightManager::GetInstance().UpdateInternal(id, &light, &trans);
			}
		}

		auto viewPos = AeonCore::CameraManager::GetInstance().GetActiveCamera().GetCameraPosition();

		gpu  GPU = {};
		GPU.viewPos = viewPos;
		GPU.ambience = AmbientLightPosition;

		for (auto& val : Lights)
		{
			switch (val.second->GetLightType())
			{
			case Lighting::LightType::SPOTLIGHT:

				GPU.spotlight[GPU.size_spotLight].position = val.second->GetPosition();
				GPU.spotlight[GPU.size_spotLight].direction = val.second->GetLightDirection();
				GPU.spotlight[GPU.size_spotLight].color = val.second->GetColor();
				GPU.spotlight[GPU.size_spotLight].cutOff = glm::cos(glm::radians((float)val.second->GetLightCutoff()));
				GPU.spotlight[GPU.size_spotLight].outerCutOff = glm::cos(glm::radians((float)val.second->GetLightOuterCutoff()));
				GPU.spotlight[GPU.size_spotLight].constant = 1.0f;
				GPU.spotlight[GPU.size_spotLight].linear = 0.09f;
				GPU.spotlight[GPU.size_spotLight].quadratic = 0.032f;
				GPU.spotlight[GPU.size_spotLight].intensity = val.second->GetLightIntensity();
				GPU.spotlight[GPU.size_spotLight].lightMatrix = val.second->GetVPMatrix();
				GPU.spotlight[GPU.size_spotLight].index = val.second->GetShadowMapIndex();
				GPU.size_spotLight++;

				break;
			case Lighting::LightType::POINT:

				GPU.pointlight[GPU.size_pointLight].position = val.second->GetPosition();

				GPU.pointlight[GPU.size_pointLight].constant = 1.0f;
				GPU.pointlight[GPU.size_pointLight].linear = 0.09f;
				GPU.pointlight[GPU.size_pointLight].quadratic = 0.032f;
				GPU.pointlight[GPU.size_pointLight].color = val.second->GetColor();
				GPU.pointlight[GPU.size_pointLight].intensity = val.second->GetLightIntensity();
				for (int i = 0; i < val.second->GetVPMatrixPoint().size(); ++i)
				{
					GPU.pointlight[GPU.size_pointLight].lightMatrix[i] = val.second->GetVPMatrixPoint()[i];
				}
				GPU.pointlight[GPU.size_pointLight].index = val.second->GetShadowMapIndex();
				GPU.size_pointLight++;

				break;
			case Lighting::LightType::DIRECTIONAL:

				GPU.directionallight[GPU.size_directionalLight].color = val.second->GetColor();
				GPU.directionallight[GPU.size_directionalLight].direction = val.second->GetLightDirection();
				GPU.directionallight[GPU.size_directionalLight].lightMatrix = val.second->GetVPMatrix();
				GPU.directionallight[GPU.size_directionalLight].intensity = val.second->GetLightIntensity();
				GPU.directionallight[GPU.size_directionalLight].index = val.second->GetShadowMapIndex();

				GPU.size_directionalLight++;
				break;
			}
		}

		// Transfer data
		mUniformBuffer->SetData( 0, sizeof(gpu), &GPU);
	}

	
	

	void LightManager::Destroy()
	{
		Lights.clear();
	}

	 void LightManager::UpdateInternal(EntityID entID, Lighting* light, Transform* trans)
	 {

	 	auto find = Lights.find(entID);
	 	if (find != Lights.end())
	 	{
	 		find->second = light;
	 		find->second->SetPosition(trans->GetPosition());
	 	}
		else {
		    Add(entID, light);
	    }
	 }

	void LightManager::Add(EntityID entID, Lighting* light)
	{
		auto find = Lights.find(entID);
		if (find == Lights.end())
		{
			Lights.insert({ entID, light });
			AeonCore_INFO("Added: Entity {}", entID);
		}
	}

	void LightManager::Remove(EntityID entID)
	{
		auto find = Lights.find(entID);
		if (find != Lights.end())
		{
			Lights.erase(find);
		}
	}

	std::unordered_map<EntityID, Lighting*>& LightManager::GetLights()
	{
		return Lights;
	}

	glm::vec3 LightManager::GetAmbientLight()
	{
		return AmbientLightPosition;
	}

	void LightManager::SetAmbientLight(glm::vec3 _clr)
	{
		AmbientLightPosition = _clr;
	}

	Frustum createFrustumFromLight(const glm::vec3& lightPosition, const glm::vec3& lightDirection,
		float aspect, float fovY, float zNear, float zFar)
	{
		Frustum frustum;
		const float halfVSide = zFar * tanf(fovY * 0.5f);
		const float halfHSide = halfVSide * aspect;

		// Normalize the light direction
		glm::vec3 lightDir = glm::normalize(lightDirection);

		// Calculate the right and up vectors based on the light's direction
		glm::vec3 right = glm::normalize(glm::cross(lightDir, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::vec3 up = glm::cross(right, lightDir);

		const glm::vec3 frontMultFar = zFar * lightDir;

		// Compute the frustum planes from the light's perspective
		frustum.nearFace = { lightPosition + zNear * lightDir, -lightDir };
		frustum.farFace = { lightPosition + frontMultFar, lightDir };
		frustum.rightFace = { lightPosition,
							  glm::cross(lightDir - right * halfHSide, up) };
		frustum.leftFace = { lightPosition,
							 glm::cross(up, lightDir + right * halfHSide) };
		frustum.topFace = { lightPosition,
							glm::cross(right, lightDir - up * halfVSide) };
		frustum.bottomFace = { lightPosition,
							   glm::cross(lightDir + up * halfVSide, right) };

		return frustum;
	}

	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<Lighting>("Lighting")
		.constructor<>()()
		.constructor<EntityID, glm::vec3, glm::vec3, glm::vec3, short, char, char>()()
		.property("Position", &Lighting::GetPosition, &Lighting::SetPosition)
		.property("Color", &Lighting::GetColor, &Lighting::SetColor)
		.property("Light Type", &Lighting::GetLightType, &Lighting::SetLightType)
		.property("Direction", &Lighting::GetLightDirection, &Lighting::SetLightDirection)
		.property("Intensity", &Lighting::GetLightIntensity, &Lighting::SetLightIntensity)
		.property("Cutoff", &Lighting::GetLightCutoff, &Lighting::SetLightCutoff)
		.property("Outer Cutoff", &Lighting::GetLightOuterCutoff, &Lighting::SetLightOuterCutoff);
		}, 2);

		rttr::registration::class_<LightManager>("LightManager")
		.constructor()
		.property("ambient light", &LightManager::GetAmbientLight, &LightManager::SetAmbientLight);
	}
}
