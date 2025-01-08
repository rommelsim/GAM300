#pragma once
/*!***************************************************************************
	\file			Coordinator.hpp
	\author			Chua Yip Xuan, Lam En Qing
	\par			yipxuan.chua\@digipen.edu, enqing.lam\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declarations of 
					classes for	coordinating entities, components, and systems 
					in an ECS (Entity-Component-System) framework. It provides 
					functionality for creating and managing entities, adding 
					and removing components, and handling parent-child 
					relationships among entities. Additionally, 
					it manages systems responsible for processing entities 
					based on their component signatures. The coordinator
					wraps the internal functions of the ECS.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <map>
#include <memory>

#include "ComponentManager.hpp"
#include "ECS.hpp"
#include "ECSUtils.hpp"
#include "Entity.hpp"
#include "SystemManager.hpp"
#include "Core/AeonAPI.h"
#include "Core/Singleton.hpp"
#include "ECS/Components/IComponent.h"

namespace AeonCore
{
	class AEON_API Coordinator : public Singleton<Coordinator>
	{
	public:
		~Coordinator() override;

		void Init();
		void Destroy();
		// Make the active scene empty
		void EmptyActiveScene();

		// ----------------- Entity -----------------
		Entity& CreateEntity(std::string _name = "");
		void DuplicateEntity(Entity& entity, EntityID parentID);
		Entity *GetEntity(EntityID id);
		Entity *GetEntityByName(std::string name);
		Entity *GetEntityByUUID(UUID _uuid);
		Signature GetEntitySignature(EntityID _entityID);
		Signature GetEntitySignature(Entity &_entity);
		bool EntityExists(EntityID id);

		// Parent-Child 
		Entity& CreateChild(EntityID parent, const std::string& _name = "");
		void ToChild(EntityID parent, EntityID child);
		void ToChild(Entity& parent, Entity& child);
		void UnChild(EntityID parent, EntityID child);
		std::vector<EntityID> GetChildObjects(EntityID id);
		Entity *GetChildObject(EntityID parent, uint32_t index = 0);
		void GetAllChildren(std::vector<EntityID> &container, EntityID id);

		/// <summary>
		/// Make a request to the entity's Scene to destroy it
		/// </summary>
		/// <param name="e">A reference to an Entity, for validity sake it does not take in by-value</param>
		/// <param name="scene">If you are aware of the scene that the Entity belongs to, then you can pass the relevant SceneRef, otherwise let DestroyEntity find the scene for you</param>
		void DestroyEntity(Entity &e, SceneRef scene = nullptr);
		/// <summary>
		/// Make a request to the entity's Scene to destroy it
		/// </summary>
		/// <param name="e">The ID of the Entity you wish to destroy</param>
		/// <param name="scene">If you are aware of the scene that the Entity belongs to, then you can pass the relevant SceneRef, otherwise let DestroyEntity find the scene for you</param>
		void DestroyEntity(EntityID e, SceneRef scene = nullptr);

		// Retrieve Containers
		//std::map<EntityID, std::vector<EntityID>> &GetMap(); // for parent child apparently???
		std::vector<Entity> &GetEntities();
		std::vector<EntityID> &GetPrefabContainer(std::string prefabName);

		// PrefabReloading functions, only stores the "head" EntityID
		void UntagPrefab(EntityID id);
		void AddToPrefabMap(std::string prefabName, EntityID id);
		void RemoveFromPrefabMap(std::string prefabName, EntityID id);

		// ----------------- Component -----------------
		template<typename T>
		void RegisterComponent();
		template<typename T>
		ComponentType GetComponentType();

		// For scene parameter, if it's nullptr, it will use the active scene to add components in
		template<typename T>
		T& AddComponent(Entity& _entity, const T& _component, std::shared_ptr<Scene> scene = nullptr);
		// For scene parameter, if it's nullptr, it will use the active scene to add components in
		template<typename T>
		T& AddComponent(EntityID _entity, const T& _component, std::shared_ptr<Scene> _scene = nullptr);

		template<typename T>
		void RemoveComponent(Entity & _entity);
		template<typename T>
		void RemoveComponent(EntityID _entity);

		template <typename T>
		bool HasComponent(Entity &_entity);
		template <typename T>
		bool HasComponent(EntityID _entity);

		template<typename T>
		T & GetComponent(Entity& _entity);
		template<typename T>
		T & GetComponent(EntityID _entity);

		// ----------------- System -----------------
		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template <typename T>
		std::shared_ptr<System> GetSystem();

		template<typename T>
		void SetSystemSignature(const Signature& _signature);

		// Go through all systems, clear their entities set and add new entities from the active scene
		void RefreshSystems();

		// ----------------- Others -----------------
		bool CheckParentIsActive(Entity &e);
		bool CheckParentIsActive(EntityID id);
	private:
		void DeleteRequestedEntities();
	private:
		friend class Singleton<Coordinator>;
		friend class Application;
		friend class SceneManager;

		std::unique_ptr<ComponentManager>						mComponentManager;
		std::unique_ptr<SystemManager>							mSystemManager;
		std::unordered_map<std::string, std::vector<EntityID>>	mPrefabReloading{};
	};

	//template functions implementations
	// ----------------- Component -----------------
	template<typename T>
	T& Coordinator::AddComponent(Entity& _entity, const T& _component, std::shared_ptr<Scene> scene)
	{
		return AddComponent<T>(_entity.GetEntityID(), _component, scene);
	}

	template<typename T>
	T& Coordinator::AddComponent(EntityID _entity, const T& _component, std::shared_ptr<Scene> _scene)
	{
		std::shared_ptr<Scene> scene = _scene ? _scene : SceneManager::GetInstance().GetSceneFromEntity(_entity);
		T& addedComp = mComponentManager->AddComponent<T>(_entity, _component, scene);

		auto _signature = scene->GetSignature(_entity);
		_signature.set(mComponentManager->GetComponentType<T>(), true);
		scene->SetSignature(_entity, _signature);

		// We only update systems if the scene that the entity was added to IS the active scene
		if (scene == SceneManager::GetInstance().GetActiveScene())
			mSystemManager->EntitySignatureChanged(_entity, _signature);

		return addedComp;
	}

	template<typename T>
	void Coordinator::RemoveComponent(Entity &_entity)
	{
		RemoveComponent<T>(_entity.GetEntityID());
	}

	template<typename T>
	void Coordinator::RemoveComponent(EntityID _entity)
	{
		mComponentManager->RemoveComponent<T>(_entity);
		std::shared_ptr<Scene> scene = SceneManager::GetInstance().GetSceneFromEntity(_entity);

		auto _signature = scene->GetSignature(_entity);
		_signature.set(mComponentManager->GetComponentType<T>(), false);
		scene->SetSignature(_entity, _signature);

		mSystemManager->EntitySignatureChanged(_entity, _signature);
	}

	template<typename T>
	bool Coordinator::HasComponent(Entity &_entity)
	{
		return HasComponent<T>(_entity.GetEntityID());
	}

	template<typename T>
	bool Coordinator::HasComponent(EntityID _entity)
	{
		EntityID actEntId = ECSUtils::ExtractRealEntityId(_entity);
		if (actEntId <= 0 && actEntId > MAX_ENTITIES) return false;

		ComponentType componentType = mComponentManager->GetComponentType<T>();

		auto scene = SceneManager::GetInstance().GetSceneFromEntity(_entity);
		if (scene == nullptr)
			return false;

		Signature entitySignature = scene->GetSignature(_entity);
		return entitySignature.test(componentType);
	}

	template<typename T>
	T & Coordinator::GetComponent(Entity &_entity)
	{
		return mComponentManager->GetComponent<T>(_entity.GetEntityID());
	}

	template<typename T>
	T & Coordinator::GetComponent(EntityID _entity)
	{
		return mComponentManager->GetComponent<T>(_entity);
	}


	// ----------------- System -----------------
	template<typename T>
	std::shared_ptr<System> Coordinator::GetSystem()
	{
		return mSystemManager->GetSystem<T>();
	}

	template<typename T>
	void Coordinator::SetSystemSignature(const Signature& _signature)
	{
		mSystemManager->SetSignature<T>(_signature);
	}

	template<typename T>
	void Coordinator::RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	ComponentType Coordinator::GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	template<typename T>
	std::shared_ptr<T> Coordinator::RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}
}

