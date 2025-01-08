#include "acpch.h"
/*!***************************************************************************
	\file			ImGuiLayer.cpp
	\author			Chew Shi Ru, Lam En Qing
	\par			shiru.chew\@digipen.edu, enqing.lam\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file defines the ImGuiLayer class
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Editor/ImGuiLayer.h"

namespace AeonCore {
	ImGuiLayer::ImGuiLayer(const std::string& _name) : Layer(_name) {}
	ImGuiLayer::~ImGuiLayer() {}
}