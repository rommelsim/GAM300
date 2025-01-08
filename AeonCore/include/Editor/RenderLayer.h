#pragma once
/*!***************************************************************************
	\file			RenderLayer.h
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function declarations for the
					RenderLayer class, which is responsible for rendering
					graphics and managing rendering-related tasks in the
					application.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "Core/Singleton.hpp"


#include "Rendering/Layer.h"
#include "Rendering/Rendering.h"

#include "Window/WindowsWindow.h"




namespace AeonCore
{
	class AEON_API RenderLayer : public Layer, public Singleton<RenderLayer>
	{
	public:
		RenderLayer();
		~RenderLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void onRender() override;

	private:
		friend class Singleton<RenderLayer>;
	};
}
