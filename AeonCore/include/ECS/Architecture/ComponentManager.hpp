#pragma once
/*!***************************************************************************
	\file			ComponentManager.hpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\brief			This header file consists of functions declarations
					of classes for managing component arrays in an ECS system.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <any>
#include <memory>
#include <unordered_map>

#include "ECS.hpp"
#include "ComponentArray.hpp"
#include "Core/SceneManager.h"

namespace AeonCore
{
	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent();

		template<typename T>
		ComponentType GetComponentType();

		std::unordered_map<std::string, ComponentType>& GetRegisteredComponents();

		template<typename T>
		T& AddComponent(EntityID _entity, const T& _component, std::shared_ptr<Scene> scene);

		template<typename T>
		void RemoveComponent(EntityID _entity);

		/*template<typename T>
		bool HasComponent(Entity & _entity);

		template<typename T>
		bool HasComponent(EntityID _entity);*/

		template<typename T>
		T &GetComponent(EntityID _entity);

		void DestroyEntity(EntityID _entity/*, Scene* scene*/) const;

		void DestroyEntity(Entity & _entity/*, Scene* scene*/) const;
	private:
		friend class Scene;

		std::unordered_map<std::string, ComponentType>						mComponentTypes{};
		ComponentType														mNextComponentType{};

		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray(std::shared_ptr<Scene> scene);

		static std::unordered_map<std::string, std::shared_ptr<IComponentArray>>& GetComponentArrays(SceneID sceneId);
	};

	template<typename T>
	void ComponentManager::RegisterComponent()
	{
		// TODO: Change the string value to use the RTTR registered name instead

		const char *typeName = typeid(T).name();

		AeonCore_ASSERT(mComponentTypes.find(typeName) == mComponentTypes.end(), "Registering _component type more than once.");

		mComponentTypes.insert({ typeName, mNextComponentType });

		// TODO: Move creation of component arrays to Scene.cpp
		//mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });


		++mNextComponentType;
	}

	template<typename T>
	ComponentType ComponentManager::GetComponentType()
	{
		const char *typeName = typeid(T).name();

		AeonCore_ASSERT(mComponentTypes.find(typeName) != mComponentTypes.end(), "Component not registered before use.");

		return mComponentTypes[typeName];
	}


	template<typename T>
	T& ComponentManager::AddComponent(EntityID _entity, const T& _component, std::shared_ptr<Scene> scene)
	{
		return GetComponentArray<T>(scene)->AddComponent(_entity, _component);
	}

	// TODO: If got nullptr for Scene* when calling RemoveComponent and GetComponent, that means active scene is null!

	template<typename T>
	void ComponentManager::RemoveComponent(EntityID _entity)
	{
		GetComponentArray<T>(SceneManager::GetInstance().GetSceneFromEntity(_entity))->RemoveComponent(_entity);
	}

	template<typename T>
	T & ComponentManager::GetComponent(EntityID _entity)
	{
		return GetComponentArray<T>(SceneManager::GetInstance().GetSceneFromEntity(_entity))->GetData(_entity);
	}

	// TODO: Phase out ComponentManager's HasComponent and ComponentArray's HasData
	/*template<typename T>
	bool ComponentManager::HasComponent(Entity &_entity)
	{
		if ( GetComponentArray<T>() )
		{
			return GetComponentArray<T>()->HasData(_entity);
		}
		return false;
	}

	template<typename T>
	bool ComponentManager::HasComponent(EntityID _entity)
	{
		if ( GetComponentArray<T>() )
		{
			return GetComponentArray<T>()->HasData(_entity);
		}
		return false;
	}*/

	template<typename T>
	std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray(std::shared_ptr<Scene> scene)
	{
		const char *typeName = typeid(T).name();

		AeonCore_ASSERT(mComponentTypes.find(typeName) != mComponentTypes.end(), "Component not registered before use.");

		auto& componentArrays = scene->mEntityComponents;

		auto findResult = componentArrays.find(typeName);
		if (findResult != componentArrays.end()) {
			return std::static_pointer_cast<ComponentArray<T>>(findResult->second);
		}

		auto newPtr = std::make_shared<ComponentArray<T>>(scene->GetMaxEntityCount());
		componentArrays.emplace(typeName, newPtr);

		return std::static_pointer_cast<ComponentArray<T>>(newPtr);
	}
}
