#pragma once
/*!***************************************************************************
	\file			Lighting.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage Lighting for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <glm/glm.hpp>
#include <ECS/Components/IComponent.h>
#include <ECS/Architecture/ECS.hpp>
#include <rttr/registration.h>
#include "Core/Log.h"
#include "Core/Singleton.hpp"
#include "ECS/Components/Transform.h"
#include <Rendering/Buffers/OpenGLBuffers.h>

namespace AeonCore
{
	class AEON_API Lighting : public IComponent
	{
	public:
		struct alignas(16) Spotlight
		{
			glm::vec3 position;			// 12b
			float PADDING_0;			// 4b

			glm::vec3 direction;
			float PADDING_1;			// 4b
			
			glm::vec3 color;
			float PADDING_2;			// 4b

			float cutOff;
			float outerCutOff;
			float constant;
			float linear;

			float quadratic;
			float intensity;
			float PADDING_3;			// 4b
			float PADDING_4;			// 4b

			glm::mat4 lightMatrix{};	// 64b

			float PADDING_5;			// 4b
			float PADDING_6;			// 4b
			float PADDING_7;			// 4b
			int index;

			Spotlight()
			{
				position = glm::vec3();
				direction = glm::vec3();
				color = glm::vec3();

				index = 0;
				PADDING_0 = PADDING_1 = PADDING_2 = PADDING_3 = PADDING_4 = PADDING_5 = PADDING_6 = PADDING_7 = 0.0f;
				cutOff = outerCutOff = constant = linear = quadratic = linear = 0.0f;
			}
		};

		struct alignas(16) Pointlight
		{
			glm::vec3 position;
			float padding_0;

			float constant;
			float linear;
			float quadratic;
			float padding_1;

			glm::vec3 color;
			float padding_2;

			glm::mat4 lightMatrix[6]{};

			float padding_3;
			float padding_4;
			float intensity;
			int index;

			Pointlight()
			{
				position = glm::vec3();
				color = glm::vec3();

				padding_0 = padding_1 = padding_2 = 0.0f;
				constant = linear = quadratic = linear = 0.0f;
			}
		};

		struct alignas(16) Directionallight
		{
			glm::vec3 direction;
			float padding_0;

			glm::vec3 color;
			float padding_1;

			glm::mat4 lightMatrix{};

			float padding_2;
			float padding_3;
			float intensity;
			int index;

			Directionallight()
			{
				direction = glm::vec3();
				color = glm::vec3();

				padding_0 = padding_1 = 0.0f;
			}
		};

		enum class LightType
		{
			SPOTLIGHT,
			POINT,
			DIRECTIONAL
		};
		Lighting();
		Lighting(EntityID entID, glm::vec3 _position = glm::vec3(0, 0, 0), glm::vec3 _color = glm::vec3(1, 1, 1)
			, glm::vec3 _direction = glm::vec3(0.0f, -1.0f, 0.0f), short _intensity = 1, short _cutoff = 12, short _outercuttoff = 17);
		void Destroy() override;

		void SetPosition(glm::vec3 _pos)					{ m_position = _pos;			}
		void SetColor(glm::vec3 _clr)						{ m_color = _clr;				}
		void SetLightType(LightType _light)					{ m_light = _light;				}
		void SetLightDirection(glm::vec3 _direction)		{ m_direction = _direction;		}
		void SetLightIntensity(short _intensity)			{ m_intensity = _intensity;		}
		void SetLightCutoff(short _cutoff)					{ m_Cutoff = static_cast<short>(_cutoff); }
		void SetLightOuterCutoff(short _outerCutoff)		{ m_outerCutoff = static_cast<short>(_outerCutoff);	}

		glm::vec3   GetPosition()							{ return m_position;		}
		glm::vec3   GetColor()								{ return m_color;			}
		LightType   GetLightType()							{ return m_light;			}
		glm::vec3   GetLightDirection()						{ return m_direction;		}
		short		GetLightIntensity()						{ return m_intensity;		}
		short		GetLightCutoff()						{ return m_Cutoff;			}
		short		GetLightOuterCutoff()					{ return m_outerCutoff;		}
		EntityID	GetEntityID()							{ return m_entID; }

		void SetVPMatrix(glm::mat4 vp)						{ m_VPMatrix = vp; }
		glm::mat4 GetVPMatrix()								{ return m_VPMatrix; }

		void SetVPMatrixPoint(std::array<glm::mat4, 6> vp)	{ m_VPMatrixPoint = vp; }
		std::array<glm::mat4, 6> GetVPMatrixPoint()			{ return m_VPMatrixPoint; }


		void SetShadowMapIndex(int idx)						{ m_shadowMapIndex = idx; }
		int GetShadowMapIndex()								{ return m_shadowMapIndex; }
		//short		GetLightCutoffProxy()					{ return m_Cutoff; }
		//short		GetLightOuterCutoffProxy()				{ return m_outerCutoff; }


		// Op overload 
		inline Lighting& operator= (const Lighting& rhs);
		Lighting* GetDataPtr() { return this; }

		inline static constexpr int MAX_LIGHTS = 5;

	private:
		RTTR_ENABLE(IComponent);
		glm::vec3   m_position;
		glm::vec3   m_color;
		glm::vec3   m_direction;
		short		m_intensity;
		short		m_Cutoff = 12;
		short		m_outerCutoff = 17;
		LightType	m_light;
		EntityID	m_entID;
		glm::mat4   m_VPMatrix{};
		std::array<glm::mat4, 6> m_VPMatrixPoint;
		int			m_shadowMapIndex{};
	};

	class AEON_API LightManager : public Singleton<LightManager>, public System
	{
	public:
		void Init() override;
		void Update() override;
		void Destroy() override;

		void Add(EntityID entID, Lighting* light);
		void Remove(EntityID entID);
		Lighting* Get(EntityID entID);
		std::unordered_map<EntityID, Lighting*>& GetLights();

		glm::vec3 GetAmbientLight();
		void SetAmbientLight(glm::vec3 _clr);
		static std::shared_ptr<OpenGLUniformBuffer> mUniformBuffer;
	private:
		
		void UpdateInternal(EntityID entID, Lighting* light, Transform* transform);
		RTTR_ENABLE();
		void Update(EntityID entID, Lighting* light, Transform* transform);
		static std::unordered_map<EntityID, Lighting*> Lights;
		static glm::vec3 AmbientLightPosition;
	};

	Frustum createFrustumFromLight(const glm::vec3& lightPosition, const glm::vec3& lightDirection,
		float aspect, float fovY, float zNear, float zFar);
}