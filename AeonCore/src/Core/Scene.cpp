#include "acpch.h"
/*!***************************************************************************
	\file            Scene.cpp
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file defines the Scene class declared in Scene.h
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/Scene.h"

#include "ECS/Architecture/ECSUtils.hpp"
#include "ECS/Architecture/ComponentArray.hpp"

#include <rttr/registration>

#include "Core/SceneManager.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Systems/PhysicsSystem.h"
#include "Scripting/Scripting.h"

namespace AeonCore {
	std::vector<std::string> Scene::s_TypeNames{};

	Scene::Scene(SceneID sceneId, SceneType sceneType, unsigned int maxEntities) : mSceneID(sceneId), mSceneType(sceneType), mAmbientLight(0.5f) {
		SetMaxEntities(maxEntities);
	}

	/*Scene::Scene(const Scene& sceneCopy) : mSceneType(sceneCopy.mSceneType) {
		SetMaxEntities(sceneCopy.mMaxEntities);

		mAvailableEntities = sceneCopy.mAvailableEntities;
		//mEntities = sceneCopy.mEntities;
		mSignatures = sceneCopy.mSignatures;
	}*/

	Scene::~Scene() {
		void* param = &mSceneID;
		Scripting::InvokeEventFunction("SceneDestroyed", &param);

		

		// Importantly is to call Destroy() for every components
		for (auto& [_, compArr] : mEntityComponents) {
			compArr->DestroyAllEntities();
		}
	
		SceneManager::GetInstance().mIdPool.push(mSceneID);
	}

	SceneID Scene::GetSceneID() {
		return mSceneID;
	}

	void Scene::SetSceneType(SceneType sceneType) {
		mSceneType = sceneType;
	}

	SceneType Scene::GetSceneType() {
		return mSceneType;
	}

	bool Scene::Empty() {
		return mEntities.empty();
	}

	void Scene::Clear() {
		mSceneType = SceneType::WillSpawn;

		// All living entities' signatures are reset. This is a "faster" loop than going through all mSignatures
		for (Entity& ent : mEntities) {
			mAvailableEntities.push(ent.GetEntityID());
			mSignatures[ECSUtils::ExtractRealEntityId(ent.GetEntityID()) - 1].reset();
		}

		mEntities.clear();
		mParentChild.clear();
		mDeleteEntitiesRequest.clear();

		for ( auto& [_, compArr]: mEntityComponents )
		{
			compArr->DestroyAllEntities();
		}
	}

	const std::vector<std::string>& Scene::GetTypeNames() {
		return s_TypeNames;
	}

	unsigned int Scene::GetMaxEntityCount() {
		return mMaxEntities;
	}

	unsigned int Scene::GetLivingCount() {
		return static_cast<unsigned int>(mEntities.size());
	}

	Entity* Scene::GetEntity(EntityID id) {
		for (Entity& ent : mEntities) {
			if (ent.GetEntityID() == id)
				return &ent;
		}
		return nullptr;
	}

	std::vector<Entity>& Scene::GetEntities() {
		return mEntities;
	}

	bool Scene::IsEntityNameRepeated(const std::string& name) {
		for (Entity& ent : mEntities) {
			if (ent.GetEntityName() == name)
				return true;
		}
		return false;
	}

	bool Scene::EntityExists(EntityID id) {
		return GetSignature(id).any();
	}

	Entity& Scene::CreateEntity(std::string name, EntityID parent) {
		if (GetLivingCount() >= mMaxEntities) {
			AeonCore_ASSERT(false, "Too many entities in existence.");

			return *mEntities.begin();
		}

		EntityID id = mAvailableEntities.front();
		mAvailableEntities.pop();

		if (name.empty())
		{
			name = "Entity " + std::to_string(id);
		}

		int i = 1;
		while(IsEntityNameRepeated(name)) {
			name = name + " (" + std::to_string(i++) + ")";
		}

		Entity newEnt(id, name);

		mSignatures[ECSUtils::ExtractRealEntityId(id) - 1].set(MAX_COMPONENTS, true); // set the last bit or termed "living bit" to true, to indicate the entity is alive

		EntityID actParentEntId = ECSUtils::ExtractRealEntityId(parent);
		if (actParentEntId > 0 && actParentEntId <= MAX_ENTITIES ) // has parent, so 
		{
			newEnt.SetParentID(parent);
			newEnt.SetIsChild(true);
		}

		return mEntities.emplace_back(newEnt);
	}

	Entity& Scene::CreateChild(EntityID parent, const std::string& name) {
		// Attempt to create child first
		Entity& newChild = CreateEntity(name, parent);

		// if the newChild is same as first entity, that means the child creation failed
		if (&newChild == &(*mEntities.begin())) {
			AeonCore_ASSERT(false, "Failed to create child entity")
			return *mEntities.begin();
		}

		// Managed to create child entity from the pool, move on!
		GetEntity(parent)->SetIsParent(true);
		mParentChild[parent].emplace_back(newChild.GetEntityID());

		return newChild;
	}

	void Scene::DestroyEntity(EntityID entityId) {
		EntityID actEntId = ECSUtils::ExtractRealEntityId(entityId);

		if (actEntId <= 0 || actEntId > MAX_ENTITIES) {
			AeonCore_ASSERT(false, "EntityID out of range.");
			return;
		}
		
		mSignatures[actEntId - 1].reset();
		mAvailableEntities.push(entityId);
	}

	void Scene::SetSignature(EntityID _entity, const Signature& _signature) {
		EntityID actEntId = ECSUtils::ExtractRealEntityId(_entity);

		if (actEntId <= 0 || actEntId > MAX_ENTITIES) {
			AeonCore_ASSERT(false, "EntityID out of range.");
			return;
		}

		mSignatures[actEntId - 1] = _signature;
	}

	Signature Scene::GetSignature(EntityID _entity) const {
		EntityID actEntId = ECSUtils::ExtractRealEntityId(_entity);

		if (actEntId <= 0 || actEntId > MAX_ENTITIES) {
			return Signature{};
		}

		return mSignatures[actEntId - 1];
	}

	void Scene::ToChild(Entity& parent, Entity& child) {
		parent.SetIsParent(true);

		// if child entity was a child of another entity, modify the previous parent to remove the child
		if ( child.IsChild() && child.GetParent() != parent.GetEntityID() && child.GetParent() != INVALID_ENTITY_ID)
		{
			auto childrenIt = mParentChild.find(child.GetParent());

			if (childrenIt != mParentChild.end()) {
				// Remove child from the vector in mParentChild
				std::vector<EntityID> &childs = childrenIt->second;
				
				childs.erase(std::find(childs.begin(), childs.end(), child.GetEntityID()));

				if (childs.empty())
				{
					GetEntity(child.GetParent())->SetIsParent(false);
					mParentChild.erase(childrenIt);
				}
			}
		}
		child.SetIsChild(true);
		child.SetParentID(parent.GetEntityID());

		mParentChild[parent.GetEntityID()].emplace_back(child.GetEntityID());
	}

	void Scene::RemoveParent(Entity& child) {
		if (!child.IsChild()) return;

		child.SetIsChild(false);

		std::vector<EntityID>& children = mParentChild[child.GetParent()];

		auto itr = std::find(children.begin(), children.end(), child.GetEntityID());
		if ( itr != children.end() )
		{
			children.erase(itr);
		}

		if (children.empty()) {
			Entity* parentEnt = GetEntity(child.GetParent());
			if (parentEnt)
				parentEnt->SetIsParent(false);
			mParentChild.erase(child.GetParent());
		}
		
		child.SetParentID(INVALID_ENTITY_ID);
	}

	void Scene::SetMaxEntities(unsigned maxEntities) {
		if (maxEntities > MAX_ENTITIES) {
			AeonCore_ASSERT(false, "Cannot set scene's max entities to be more than MAX_ENTITIES limit");
			return;
		}
		mMaxEntities = maxEntities;

		mEntities.reserve(mMaxEntities);
		mEntityComponents.reserve(mMaxEntities);

		mSignatures.resize(mMaxEntities);

		while (!mAvailableEntities.empty())
			mAvailableEntities.pop();

		for (EntityID _entity = 1; _entity <= mMaxEntities; _entity++) {
			mAvailableEntities.push(ECSUtils::EncodeSceneID(_entity, mSceneID));
		}
	}

	void Scene::SetAmbientLight(glm::vec3 light) {
		mAmbientLight = light;
	}
}

// outside of namespace to allow private member to be referenced
RTTR_REGISTRATION {
	using namespace AeonCore;
	//std::cout << "Registered Scene for RTTR" << std::endl;

	// For serialization, we just use the integer representations!
	rttr::registration::enumeration<SceneType>("SceneType")
	(
		rttr::value("Will Spawn", SceneType::WillSpawn),
		rttr::value("Will Not Spawn", SceneType::WillNotSpawn)
	);
	auto rttrEnumNames = rttr::type::get<SceneType>().get_enumeration().get_names();

	Scene::s_TypeNames.reserve(rttrEnumNames.size());
	for (auto& name : rttrEnumNames) {
		//std::cout << name << std::endl;
		Scene::s_TypeNames.emplace_back(name);
	}

	rttr::registration::class_<Scene>("Scene")
		.constructor<SceneID, SceneType, EntityID>()()
		.property("type", &Scene::mSceneType);
}
