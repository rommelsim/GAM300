#ifndef MATERIAL_H
#define MATERIAL_H

/*!***************************************************************************
	\file			Material.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage Material properties for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Resource/Shader.h"
#include "Resource/Texture.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/IComponent.h"
//#include "Rendering/Lighting.h"
 #include "ECS/Components/Lighting.h"
#include "ECS/Architecture/UUID.hpp"

namespace AeonCore
{
	class AEON_API Material
	{
	public:
		

		Material();
		~Material() = default;

		Material(std::string name);
		void Use(Shader& program);

		void SetShininess(float _shiniess);
		void SetDiffuseTexture(std::string _path);
		void SetSpecularTexture(std::string _path);
		
		float GetShininess();
		std::string GetDiffuseTexture();
		std::string GetSpecularTexture();

		std::string GetName();
		void SetName(std::string name);

		Texture& GetDiffuseTexturePtr() { return m_diffuseTexturePtr; }
		Texture& GetSpecularTexturePtr() { return m_specularTexturePtr; }
	private:
		
		// Textures
		Texture m_diffuseTexturePtr;
		Texture m_specularTexturePtr;

		// Slots used for texture. (Not supposed to be touched !)
		GLuint diffuse_slot = 1;
		GLuint specular_slot = 2;

		// name of material
		std::string m_name;

		float m_shininess = 0.5f;
	};
}

#endif // !MATERIAL_H
