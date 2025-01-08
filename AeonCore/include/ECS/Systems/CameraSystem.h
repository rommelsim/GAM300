#pragma once
/*!***************************************************************************
	\file			CameraSystem.h
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\brief			Declaration of the CameraSystem responsible for managing
					cameras. This file contains the declaration of the
					CameraSystem, responsible for managing cameras. It provides
					functions for initializing the default active camera,
					updating camera positions based on associated entities and
					transformations, and managing camera components.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Architecture/System.hpp"
#include "ECS/Architecture/Coordinator.hpp"

#include "ECS/Components/Camera.h"
#include "ECS/Components/Transform.h"

#include "Editor/EditorCamera.h"

#include "Camera/CameraManager.h"

namespace AeonCore
{
	class CameraSystem : public System
	{
	public:
		void Init()		override;
		void Update()	override;
		void Destroy()	override;

	private:
		Camera* mGameCamera = nullptr;

	};
}