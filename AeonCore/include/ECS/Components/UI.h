#pragma once
/*!***************************************************************************
	\file			UI.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage UI component for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "IComponent.h"
#include "Core/Log.h"
#include "Core/AeonAPI.h"
#include "Resource/ResourceManager.h"
#include "Transform.h"
#include <algorithm>
#include <filesystem>

namespace AeonCore
{
	class AEON_API UI : public IComponent
	{
	public:
		enum class Format {Image = 0, Word};
		enum class Layer { Default = 0, Foreground, Background };
		using Order = unsigned int;

		// Ctor
		UI();
		UI(EntityID id);
		~UI();

		void Destroy() override;

		// Layer Get/Set
		Layer			GetLayer();
		void			SetLayer(Layer _layer);

		// Texture Get/Set
		Texture*		GetTexturePtr();
		std::string		GetTexturePath();
		void			SetTexturePath(std::string _texturePath);

		// Order Get/Set
		Order			GetOrder();
		void			SetOrder(Order _order);

		// Font Get/Set
		FontResource*	GetFontPtr();
		void			SetFontType(std::string _path);
		std::string		GetFontType();
		std::string		GetText();
		void			SetText(std::string _text);

		// Color of font get/set
		glm::vec3		GetColor();
		void			SetColor(glm::vec3 _clr);

		glm::vec4		GetTextureColor();
		void			SetTextureColor(glm::vec4 _clr);

		EntityID		GetEntityID();
		void			SetEntityID(EntityID _id);

		void			SetFormat(Format _format);
		Format			GetFormat();

		void			SetOpacity(float _opacity);
		float			GetOpacity();
	private:
		RTTR_ENABLE(IComponent);
		FontResource m_fontRsc;
		Layer		 m_layer;
		Texture		 m_texture;
		Order		 m_order;
		EntityID	 m_EntID;
		Format		 m_format;
		std::string	 m_text;
		glm::vec3	 m_FontClr;
		glm::vec4	 m_TextureClr;
		float		 m_opacity = 1.0f;
	};

}

