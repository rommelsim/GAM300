#pragma once
/*!***************************************************************************
	\file			Render.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage Render component
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "IComponent.h"
#include "Core/AeonAPI.h"
#include "Resource/ResourceManager.h"

#include "ECS/Components/Camera.h"

#include <glm/glm.hpp>
#include <utility>
#include <filesystem>
#include <rttr/registration_friend.h>

#include <Resource/Material.h>


namespace AeonCore
{
	 enum class Shape3D
	 {
	 	sphere,
	 	cube,
	 	cylinder
	 };

	class AEON_API Render : public IComponent
	{
	public:
		
		Render()=default;
		Render(std::string name, EntityID entityID);		// best to not use... By right UX should always provide a mesh for me to render!!!
		Render(std::string meshFile, std::string materialName, EntityID entID);

		void Destroy() override;

		// Material Getter
		float GetShininess()						{ return m_material.GetShininess(); }
		std::string    GetDiffuseTexture()			{ return m_material.GetDiffuseTexture();		}
		std::string    GetSpecularTexture()			{ return m_material.GetSpecularTexture();		}
		Material*	   GetMaterialPtr()				{ return &m_material;							}

		// Material Setter
		void SetDiffuseTexture(std::string _path);// { m_material.SetDiffuseTexture(_path);			}
		void SetSpecularTexture(std::string _path);// { m_material.SetSpecularTexture(_path);			}
		void SetShininess(float _shininess)			{ m_material.SetShininess(_shininess);			}

		// Model Getter 
		Model*		GetModelPtr()					{ return &m_model;								}
		std::string GetModelPath()					{ return m_model.GetDirectory();				}

		// Model Setter
		void SetModelName(std::string _name)		{ m_model.SetName(_name);						}
		void SetModel(std::string _path);

	private:
		RTTR_ENABLE(IComponent);
		RTTR_REGISTRATION_FRIEND

		// inline void SetMaterial_Internal(Material* _mat) { m_material = _mat; }

		// Properties
		Material	m_material;
		Model		m_model;
		std::string  m_text;

		EntityID m_EntID;
	};

}
