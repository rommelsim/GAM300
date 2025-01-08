#pragma once
/*!***************************************************************************
	\file			GameViewport.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definition to show
					a game viewport, from the player perspective on the editor. (Upcoming)
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "GameViewport.h"
#include <imgui.h>
#include <Core/Application.h>
#include <Rendering/Framebuffer/FramebufferManager.h>
#include <Rendering/Rendering.h>
#include <ECS/Systems/Renderer2DSystem.h>
#include <ECS/Components/Particle.h>
#include <ECS/Components/Camera.h>
#include "Camera/CameraManager.h"

namespace Editor {
	GameViewport::GameViewport()
	{
		Init();
	}
	GameViewport::~GameViewport()
	{
		ShutDown();

	}
	void GameViewport::Init()
	{
	}
	void GameViewport::ShowViewport()
	{ 
		


	}
	void GameViewport::ShutDown()
	{
	}
}