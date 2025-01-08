#pragma once
/*!***************************************************************************
	\file			SystemManager.hpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage the systems in the ECS architecture.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <cassert>
#include <memory>
#include <unordered_map>
#include "System.hpp"
#include "Core/AeonAPI.h"
#include "ECS.hpp"
#include "Entity.hpp"
#include "Core/Log.h"

namespace AeonCore
{
	class AEON_API SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		void SetSignature(const Signature& _signature);

		void DestroyEntity(EntityID _entity);
		void DestroyEntity(Entity &_entity);
		void DestroyAllEntities();

		void EntitySignatureChanged(EntityID _entity, const Signature& _entitySignature);
		void EntitySignatureChanged(Entity &_entity, const Signature& _entitySignature);

		template <typename T>
		std::shared_ptr<System> GetSystem();
	private:
		std::unordered_map<const char *, Signature>					mSignatures{};
		std::unordered_map<const char *, std::shared_ptr<System>>	mSystems{};
	};

	//template functions implementation

	template<typename T>
	std::shared_ptr<T> SystemManager::RegisterSystem()
	{
		const char *typeName = typeid(T).name();

		AeonCore_ASSERT(mSystems.find(typeName) == mSystems.end(), "Registering system more than once.");

		auto system = std::make_shared<T>();
		mSystems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	void SystemManager::SetSignature(const Signature& _signature)
	{
		const char *typeName = typeid(T).name();

		AeonCore_ASSERT(mSystems.find(typeName) != mSystems.end(), "System used before registered.");

		mSignatures.insert({ typeName, _signature });
	}

	template<typename T>
	std::shared_ptr<System> SystemManager::GetSystem()
	{
		const char *name = typeid(T).name();

		for(const auto& system : mSystems)
		{
			// if found
			if(std::strcmp( system.first, name) == 0)
			{
				return system.second;
			}
		}

		return 0;
	}
}
