#include "acpch.h"
/*!***************************************************************************
	\file			UI.cpp
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage UI component for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Components/UI.h"

#include "Core/Application.h"
#include "ECS/Systems/Renderer2DSystem.h"		// prevent cyclic ref
#include "Serialization/RegisterManager.hpp"

namespace AeonCore
{
	std::filesystem::path working_dir;

	UI::UI()
	{
		working_dir = std::filesystem::current_path();
		// Empty by default, for ECS Startup.
	}

	UI::UI(EntityID id) : m_format(Format::Image), m_FontClr(glm::vec3(1,0,0)), m_opacity(1.0f), m_TextureClr(glm::vec4(0.5, 0.5, 0.5, 1))
	{
		 
		working_dir = std::filesystem::current_path();

		m_layer = Layer::Default;
		m_order = 0;
		m_EntID = id;
		
		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning) {
			// Need to set texture* ??? yes, set it to some random crate photo for now...
			m_texture = *ResourceManager::GetInstance().LoadTexture(
				working_dir.string() + "/resources/textures/container.dds",
				working_dir.string() + "/resources/textures/container.dds",
				Texture::Type::None);

			m_fontRsc = *ResourceManager::GetInstance().LoadFont("OpenSans-Bold.ttf", working_dir.string() + "/resources/fonts/OpenSans-Bold.ttf");
		} else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID]() {
				/* Use entity ID to retrieve the UI component because when deserializing the component is constructed twice:
				    1. One on the stack to create the component instance and pass it to rttr to deserialize thru the rttr properties
				    2. Passing that stack-allocated component to Coordinator#AddComponent to actually add the component.
				       Within AddComponent, the component is assigned to a position in an std::array container which is a copy-assignment
				*/
				Coordinator& coord = Coordinator::GetInstance();
				UI& uiComp = coord.GetComponent<UI>(entID);

				uiComp.SetFontType(working_dir.string() + "/resources/fonts/OpenSans-Bold.ttf");
				uiComp.SetTexturePath(working_dir.string() + "/resources/textures/container.dds");
			});
		}
	}

	UI::~UI()
	{

	}

	void UI::Destroy()
	{
		Renderer2DSystem::RemoveLayer(this);
	}

	UI::Layer UI::GetLayer()									
	{
		return m_layer; 
	}

	void UI::SetLayer(Layer _layer)						
	{
		m_layer = _layer; 
	}

	Texture* UI::GetTexturePtr()							
	{ 
		return &m_texture; 
	}
	
	std::string UI::GetTexturePath()							
	{
		return m_texture.GetPath();
	}

	void UI::SetTexturePath(std::string _texturePath) 
	{
		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning || app.GetLoadStatus() == LoadStatus::LoadResources)
			m_texture = *ResourceManager::GetInstance().LoadTexture(_texturePath, _texturePath, Texture::Type::None);
		else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID, imagePath = _texturePath]() {
				Coordinator& coord = Coordinator::GetInstance();
				coord.GetComponent<UI>(entID).SetTexturePath(imagePath);
			});
		}
	}

	UI::Order UI::GetOrder()									
	{
		return m_order; 
	}

	void UI::SetOrder(Order _order)						
	{ 
		m_order = _order; 
	}

	EntityID UI::GetEntityID()									
	{
		return m_EntID; 
	}
	
	void UI::SetEntityID(EntityID _id)							
	{
		m_EntID = id;
	}

	UI::Format UI::GetFormat()									
	{ 
		return m_format;	
	}

	FontResource* UI::GetFontPtr()								
	{
		return &m_fontRsc; 
	}

	void UI::SetFontType(std::string _path)
	{
		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning || app.GetLoadStatus() == LoadStatus::LoadResources)
			m_fontRsc = *ResourceManager::GetInstance().LoadFont("LilitaOne-Regular.ttf", working_dir.string() + "/resources/fonts/LilitaOne-Regular.ttf");
		else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID]() {
				Coordinator& coord = Coordinator::GetInstance();
				coord.GetComponent<UI>(entID).SetFontType(working_dir.string() + "/resources/fonts/LilitaOne-Regular.ttf");
			});
		}
	}

	std::string UI::GetFontType()
	{
		return m_fontRsc.GetPath();
	}

	std::string UI::GetText()
	{
		return m_text;
	}

	void UI::SetText(std::string _text)
	{
		m_text = _text;
	}

	void UI::SetFormat(Format _format)
	{
		m_format = _format;
	}

	glm::vec3 UI::GetColor()
	{
		return m_FontClr;
	}

	void UI::SetColor(glm::vec3 _clr)
	{
		m_FontClr = _clr;
	}

	void UI::SetOpacity(float _opacity)
	{
		m_opacity = _opacity;
	}

	float UI::GetOpacity()
	{
		return m_opacity;
	}

	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
			rttr::registration::class_<UI>("UI")
				.constructor<EntityID>()()
				.property_readonly("id", &UI::GetComponentID)
				.property("Layer", &UI::GetLayer, &UI::SetLayer)
				.property("Order", &UI::GetOrder, &UI::SetOrder)
				.property("Texture Path", &UI::GetTexturePath, &UI::SetTexturePath)
				.property("Format", &UI::GetFormat, &UI::SetFormat)
				.property("Font Text", &UI::GetText, &UI::SetText)
				.property("Font Type", &UI::GetFontType, &UI::SetFontType)
				.property("Font Color", &UI::GetColor, &UI::SetColor)
				.property("Opacity", &UI::GetOpacity, &UI::SetOpacity);		// disable because serialisation :(
			}, 9);
	}
}
