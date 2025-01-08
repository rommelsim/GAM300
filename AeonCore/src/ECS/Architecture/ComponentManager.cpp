#include "acpch.h"
/*!***************************************************************************
	\file			ComponentManager.cpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\brief			This header file consists of functions definitions
					of classes for managing component arrays in an ECS system.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Architecture/ComponentManager.hpp"

#include "ECS/Architecture/ECSUtils.hpp"

namespace AeonCore
{
	std::unordered_map<std::string, ComponentType>& ComponentManager::GetRegisteredComponents() {
		return mComponentTypes;
	}

	void ComponentManager::DestroyEntity(EntityID _entity) const
	{
		SceneID entity_SceneId = ECSUtils::ExtractSceneID(_entity);
		for ( auto const &pair : GetComponentArrays(entity_SceneId) )
		{
			auto const &_component = pair.second;

			_component->EntityDestroyed(_entity);
		}
	}
	void ComponentManager::DestroyEntity(Entity &_entity) const
	{
		DestroyEntity(_entity.GetEntityID());
	}

	std::unordered_map<std::string, std::shared_ptr<IComponentArray>>& ComponentManager::GetComponentArrays(SceneID sceneId) {
		return SceneManager::GetInstance().GetScene(sceneId)->mEntityComponents;
	}
}
