#pragma once
/*!***************************************************************************
	\file			CollisionSystem.h
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function declarations of the
					CollisionSystem class.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Architecture/System.hpp"

namespace AeonCore
{
	class CollisionSystem : public System
	{
	public:
		CollisionSystem() = default;

		void Init() override;

		void Update() override;

		void Destroy() override;

	};
}