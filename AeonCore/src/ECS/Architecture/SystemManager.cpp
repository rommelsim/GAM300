#include "acpch.h"
/*!***************************************************************************
	\file			SystemManager.cpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function definitions to
					manage the systems in the ECS architecture.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Architecture/SystemManager.hpp"

namespace AeonCore
{
	void SystemManager::DestroyEntity(EntityID _entity)
	{
		for ( auto const &pair : mSystems )
		{
			auto const &system = pair.second;


			system->mEntities.erase(_entity);
		}
	}

	void SystemManager::DestroyEntity(Entity &_entity)
	{
		DestroyEntity(_entity.GetEntityID());
	}

	void SystemManager::DestroyAllEntities() {
		for (auto const& pair : mSystems) {
			pair.second->mEntities.clear();
		}
	}

	void SystemManager::EntitySignatureChanged(EntityID _entity, const Signature& _entitySignature)
	{
		for ( auto const &pair : mSystems )
		{
			auto const &type = pair.first;
			auto const &system = pair.second;
			auto const &systemSignature = mSignatures[type];

			if ( (_entitySignature & systemSignature) == systemSignature )
			{
				system->mEntities.insert(_entity);
			}
			else
			{
				system->mEntities.erase(_entity);
			}
		}
	}
	void SystemManager::EntitySignatureChanged(Entity &_entity, const Signature& _entitySignature)
	{
		EntitySignatureChanged(_entity.GetEntityID(), _entitySignature);
	}
}