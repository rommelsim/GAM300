#include "acpch.h"

/*!***************************************************************************
	\file			Font.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage the Font 
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Components/Font.h"
#include "Serialization/RegisterManager.hpp"

namespace AeonCore
{
	Font::Font()
	{
		// default

	}
	Font::Font(const char* path)
	{
		m_fontRsc = ResourceManager::GetInstance().LoadFont(path, path);
	}

	FontResource::Character Font::GetCharacter(char ch)
	{
		return m_fontRsc->GetCharacter(ch);
	}
	//void Font::SetFontType(const char* path)
	//{
	//	m_fontRsc = ResourceManager::GetInstance().LoadFont(path, path);
	//}
	void Font::SetFontType(std::string path)
	{
		m_fontRsc = ResourceManager::GetInstance().LoadFont(path, path);
	}
	std::string Font::GetFontPath()
	{
		return m_fontRsc->GetPath();
	}
	void Font::SetText(std::string text)
	{
		m_text = text;
	}
	std::string Font::GetText()
	{
		return m_text;
	}
	void Font::SetColor(glm::vec3 clr)
	{
		m_color = clr;
	}
	glm::vec3 Font::GetColor()
	{
		return m_color;
	}
	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<Font>("Font")
		.constructor<const char*>()()
		.property_readonly("id", &Font::GetComponentID)
		.property("type", &Font::GetFontPath, &Font::SetFontType)
		.property("text", &Font::GetText, &Font::SetText)
		.property("colour", &Font::GetColor, &Font::SetColor); }, 8);
	}
}
