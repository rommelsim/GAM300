#pragma once
/*!***************************************************************************
	\file			Font.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage the Font
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "IComponent.h"
#include "Core/Log.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "Core/AeonAPI.h"
#include "Resource/ResourceManager.h"

namespace AeonCore
{
	class AEON_API Font : public IComponent
	{
	public:
		Font();
		Font(const char* path);

		FontResource::Character GetCharacter(char ch);			// not to be used by imgui!

		// IMGUI usage starts from here !!!
		//void					SetFontType(const char* path);
		void					SetFontType(std::string path);
		std::string				GetFontPath();
								
		void					SetText(std::string text);
		std::string				GetText();
								
		void					SetColor(glm::vec3 clr);
		glm::vec3				GetColor();
		
	private:
		RTTR_ENABLE(IComponent);
		FontResource*	m_fontRsc;
		std::string		m_text;
		glm::vec3		m_color;
	};
}
