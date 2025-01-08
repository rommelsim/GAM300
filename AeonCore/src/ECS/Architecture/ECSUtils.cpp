#include "acpch.h"
/*!***************************************************************************
    \file			ECSUtils.cpp
    \author			Lam En Qing
    \par			enqing.lam\@digipen.edu
    \brief			This file defines several utility to manipulate
					EntityID
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Architecture/ECSUtils.hpp"

namespace AeonCore {
	EntityID ECSUtils::EncodeSceneID(EntityID entityId, SceneID sceneId) {
		/* 1. We cast SceneID to EntityID to ensure that the size before bitshifting is 32 bits.
		   2. We want our SceneID to reside in the last 8 bits, so must move left by 24 bits.
		   3. Bit OR to combine scene and entity ids together
		*/
		return entityId | (static_cast<EntityID>(sceneId) << 24);
	}

	SceneID ECSUtils::ExtractSceneID(EntityID entityId) {
		entityId = entityId & (~ACTUAL_ENTITY_MASK); // Make a bit mask using Bit NOT by setting the first 24 bits from 1 to 0 and last 8 bits from 0 to 1. Then a Bit AND to set the first 24 bits to 0
		entityId = entityId >> 24; // we want to get the SceneID so move right by 24 bits
		return static_cast<SceneID>(entityId); // type-cast to remove the extra bits, our first 8 bits will be safe. I think proper term is "truncating"
	}

	EntityID ECSUtils::ExtractRealEntityId(EntityID entityId) {
		entityId = entityId & ACTUAL_ENTITY_MASK; // remove the last 8 bits from the data using the mask using Bit AND
		return entityId;
	}
}
