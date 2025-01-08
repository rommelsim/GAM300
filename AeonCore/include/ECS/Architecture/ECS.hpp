#pragma once
/*!***************************************************************************
	\file			ECS.hpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the types defintion that is 
					used for the ECS structure.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <bitset>
#include <cstdint>

namespace AeonCore
{
	using SceneID = std::uint8_t;
	inline SceneID MAX_SCENES = 254; // Max value is actually 254?

	// ECS
	using EntityID						= std::uint32_t; // will represent the combined EntityID, that is 24 bits (entity Id) + 8 bits (scene Id)
	const EntityID MAX_ENTITIES			= 5000;
	const EntityID INVALID_ENTITY_ID	= MAX_ENTITIES + 1;
	const EntityID ACTUAL_ENTITY_MASK	= 16777215; // first 24 bits (from right) are all 1s, last 8 bits are all 0s, binary form looks like: 00000000 11111111 11111111 11111111

	using ComponentType					= std::uint8_t;
	const ComponentType MAX_COMPONENTS	= 31;

	using Signature						= std::bitset<MAX_COMPONENTS + 1>;
}

