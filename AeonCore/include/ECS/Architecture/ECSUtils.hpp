#pragma once
/*!***************************************************************************
    \file			ECSUtils.hpp
    \author			Lam En Qing
    \par			enqing.lam\@digipen.edu
    \brief			This header file declares several utility to manipulate
					EntityID
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS.hpp"

#include "Core/AeonAPI.h"

namespace AeonCore {
	class AEON_API ECSUtils {
	public:
		static EntityID EncodeSceneID(EntityID entityId, SceneID sceneId);
		static SceneID  ExtractSceneID(EntityID entityId);
		static EntityID ExtractRealEntityId(EntityID entityId);
	};
}
