#pragma once
/*!***************************************************************************
	\file			ImGuiLayer.h
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file contains an abstract class, ImGuiLayer, for
					Editor to implement
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "Rendering/Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseKeyEvent.h"
#include "Events/KeyEvent.h"
#include "Core/Singleton.hpp"

namespace AeonCore
{
	class AEON_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& _name="ImGuiLayer");
		~ImGuiLayer();
		/*!***************************************************************************
		*   \brief			Initialize imgui for rendering
			\param			nil
			\return			void
		*******************************************************************************/
		virtual void Begin() = 0;
		/*!***************************************************************************
		*   \brief			End and render ImGui items
			\param			nil
			\return			void
		*******************************************************************************/
		virtual void End() = 0;
	};
}