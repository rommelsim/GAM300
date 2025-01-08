#pragma once
/*!***************************************************************************
	\file			Systems.hpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of virtual functions to be
					inherited by other systems as a base system class.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <set>
#include "ECS.hpp"

namespace AeonCore
{
	class System
	{
	public:
		virtual ~System() = default;
		virtual void Init()		= 0;
		virtual void Update()	= 0;
		virtual void Destroy()	= 0;

		std::set<EntityID> mEntities{};
	};
}
