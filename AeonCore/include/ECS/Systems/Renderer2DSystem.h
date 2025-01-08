#pragma once
/*!***************************************************************************
	\file			Render2DSystem.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					create and manage UI Component for rendering
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Architecture/System.hpp"
#include "ECS/Components/UI.h"
#include "Rendering/Rendering.h"

namespace AeonCore
{
	class AEON_API Renderer2DSystem : public System
	{
		using Interface = std::unordered_map<EntityID, UI*>;

	public:
		void Init() override;

		void Update() override;

		void Destroy() override;

		static void UpdateLayer(EntityID id, UI* first, UI::Layer _layer);
		static void RemoveLayer(UI* ui);
		static std::map<UI::Layer, Interface>& GetLayers();

	private:
		static std::map<UI::Layer, Interface> m_Layers;

	private:
		static bool Comparator(std::pair<EntityID, UI*>& left, std::pair<EntityID, UI*>& right);
		Interface Sort(UI::Layer _layer, Interface& _in);
	};
}
