#pragma once
/*!***************************************************************************
	\file            ScriptingSystem.h
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file declares the ScriptingSystem class that
					 inherits from System
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Architecture/System.hpp"

namespace AeonCore
{
	class ScriptingSystem : public System
	{
	public:
		void Init() override;
		void Update() override;
		void Destroy() override;
	};
}