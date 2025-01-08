#include "acpch.h"
/*!***************************************************************************
	\file			Coordinator.cpp
	\author			Chua Yip Xuan, Lam En Qing
	\par			yipxuan.chua\@digipen.edu, enqing.lam\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function definitions of
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

#include "ECS/Architecture/Coordinator.hpp"

#include "ECS/Architecture/ECSUtils.hpp"
#include "ECS/Components/Transform.h"			
#include "ECS/Components/Render.h"		
#include "ECS/Components/Font.h"	
#include "ECS/Components/UI.h"	
#include "ECS/Components/Camera.h"	
#include "ECS/Components/Audio.h"	
#include "ECS/Components/Behaviour.h"	
#include "ECS/Components/Collider.h"
#include "ECS/Components/Particle.h"
#include "ECS/Components/RigidBody.h"

namespace AeonCore
{
	Coordinator::~Coordinator()
	{
		//Destroy();
	}
	void Coordinator::Init()
	{
		mComponentManager = std::make_unique<ComponentManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}

	void Coordinator::Destroy()
	{
		// Free all entities before allocators
		EmptyActiveScene();
		mPrefabReloading.clear();
	}

	void Coordinator::EmptyActiveScene()
	{
		mSystemManager->DestroyAllEntities();

		SceneManager::GetInstance().GetActiveScene()->Clear();
	}

	Entity& Coordinator::CreateEntity(std::string _name)
	{
		/*Entity entity = mEntityManager->CreateEntity(_name);

		int i = 1;
		std::string name = entity.GetEntityName();
		while ( IsNameRepeated(entity) )
		{
			entity.SetEntityName(name + " (" + std::to_string(i++) + ")");
		}

		return SceneManager::GetInstance().GetActiveScene()->mEntities.emplace_back(entity);*/
		return SceneManager::GetInstance().GetActiveScene()->CreateEntity(_name);
	}

	void Coordinator::DuplicateEntity(Entity& entityToCopy, EntityID parentID)
	{
		//EntityID duplicated_id = entity.GetParent() < MAX_ENTITIES ? CreateChild(parentID) : CreateEntity();
		//Entity &duplicated_entity = *GetEntity(duplicated_id);
		std::shared_ptr<Scene> scene = SceneManager::GetInstance().GetSceneFromEntity(entityToCopy.GetEntityID());

		if (scene == nullptr) {
			AeonCore_ASSERT(false, "Failed to get scene in DuplicateEntity")
			return;
		}

		EntityID act_oldParentId = ECSUtils::ExtractRealEntityId(entityToCopy.GetParent());
		Entity& duplicated_entity = act_oldParentId > 0 && act_oldParentId <= MAX_ENTITIES ? CreateChild(parentID) : CreateEntity();
		duplicated_entity.Copy(entityToCopy);
		if ( std::string prefabName = duplicated_entity.GetPrefab(); prefabName != "" )
		{
			AddToPrefabMap(prefabName, duplicated_entity.GetEntityID());
		}

		std::string entity_name = entityToCopy.GetEntityName();
		std::string duplicate_name{};

		int i = 1;
		for ( int size = 0; size < scene->mEntities.size(); ++size )
		{
			duplicate_name = entity_name + "(" + std::to_string(i) + ")";

			if ( scene->mEntities[size].GetEntityName() == duplicate_name )
			{
				++i;
				size = 0;
			}
		}
		duplicated_entity.SetEntityName(duplicate_name);

		// Camera
		if (HasComponent<Camera>(entityToCopy))
		{
			AddComponent<Camera>(duplicated_entity, GetComponent<Camera>(entityToCopy.GetEntityID()));
		}
		// Font
		if (HasComponent<Font>(entityToCopy))
		{
			AddComponent<Font>(duplicated_entity, GetComponent<Font>(entityToCopy.GetEntityID()));
		}
		// Lightning
		if (HasComponent<Lighting>(entityToCopy))
		{
			Lighting& lightCompSrc = GetComponent<Lighting>(entityToCopy);
			Lighting lightCompDst(duplicated_entity.GetEntityID());

			// RTTR to copy values
			{
				rttr::instance rttrInstDst(lightCompDst);
				rttr::instance rttrInstSrc(lightCompSrc);
				rttr::type rttrRenderType = rttr::type::get(lightCompSrc);

				// copy all properties' values from src to dst
				for (const auto& prop : rttrRenderType.get_properties()) {
					prop.set_value(rttrInstDst, prop.get_value(rttrInstSrc));
				}
			}

			AddComponent<Lighting>(duplicated_entity, lightCompDst);
		}
		// Particles
		if (HasComponent<Particles>(entityToCopy)) {
			Particles& particlesCompSrc = GetComponent<Particles>(entityToCopy);
			Particles particlesCompDst(entityToCopy.GetEntityID());

			// RTTR to copy values
			{
				rttr::instance rttrInstDst(particlesCompDst);
				rttr::instance rttrInstSrc(particlesCompSrc);
				rttr::type rttrParticlesType = rttr::type::get(particlesCompSrc);

				// copy all properties' values from src to dst
				for (const auto& prop : rttrParticlesType.get_properties()) {
					prop.set_value(rttrInstDst, prop.get_value(rttrInstSrc));
				}
			}

			AddComponent<Particles>(duplicated_entity, particlesCompDst);
		}
		// Render
		if (HasComponent<Render>(entityToCopy))
		{
			const Render& renderCompSrc = GetComponent<Render>(entityToCopy.GetEntityID());
			Render renderCompDst("", duplicated_entity.GetEntityID());

			// RTTR to copy values
			{
				rttr::instance rttrInstDst(renderCompDst);
				rttr::instance rttrInstSrc(renderCompSrc);
				rttr::type rttrRenderType = rttr::type::get(renderCompSrc);

				// copy all properties' values from src to dst
				for (const auto& prop : rttrRenderType.get_properties()) {
					prop.set_value(rttrInstDst, prop.get_value(rttrInstSrc));
				}
			}

			AddComponent<Render>(duplicated_entity, renderCompDst);
		}
		// Transform
		if (HasComponent<Transform>(entityToCopy))
		{
			AddComponent<Transform>(duplicated_entity, GetComponent<Transform>(entityToCopy.GetEntityID()));
		}
		// UI 
		if (HasComponent<UI>(entityToCopy))
		{
			const UI& uiCompSrc = GetComponent<UI>(entityToCopy.GetEntityID());
			UI uiCompDst(duplicated_entity.GetEntityID());
						
			// RTTR to copy values
			{
				rttr::instance rttrInstDst(uiCompDst);
				rttr::instance rttrInstSrc(uiCompSrc);
				rttr::type rttrRenderType = rttr::type::get(uiCompSrc);
				
				// copy all properties' values from src to dst
				for (const auto& prop : rttrRenderType.get_properties()) {
					prop.set_value(rttrInstDst, prop.get_value(rttrInstSrc));
				}
			}

			AddComponent<UI>(duplicated_entity, uiCompDst);
		}

		// Collider
		if (HasComponent<Collider>(entityToCopy))
		{
			Collider& colliderCompSrc = GetComponent<Collider>(entityToCopy.GetEntityID());
			Collider colliderCompDst;

			// RTTR to copy values
			{
				rttr::instance rttrInstDst(colliderCompDst);
				rttr::instance rttrInstSrc(colliderCompSrc);
				rttr::type rttrRenderType = rttr::type::get(colliderCompSrc);
				
				// copy all properties' values from src to dst
				for (const auto& prop : rttrRenderType.get_properties()) {
					rttr::variant propValueSrc = prop.get_value(rttrInstSrc);
					
					// Adapted from Serialization::DeserializeComponentProperties
					rttr::type rttrPhysMatType = rttr::type::get<ACPhysics::ACPhysicsMaterial>();

					if (prop.get_type() == rttrPhysMatType) {
						ACPhysics::ACPhysicsMaterial mat;
						rttr::instance rttrPhysMat(mat);

						// Copy Phys Material's properties from propValueSrc into rttrPhysMat
						for (const auto& physMatProp : rttrPhysMatType.get_properties()) {
							physMatProp.set_value(rttrPhysMat, physMatProp.get_value(propValueSrc));
						}

						prop.set_value(rttrInstDst, mat);
						continue;
					}
					if (prop.get_type() == rttr::type::get<ACPhysics::Geometry*>()) {
						ACPhysics::Geometry* baseGeom = propValueSrc.get_value<ACPhysics::Geometry*>();

						switch(colliderCompSrc.GetColliderType()) {
							case Collider::ColliderType::CUBE:
								{
									ACPhysics::CubeGeometry* geomSrc = dynamic_cast<ACPhysics::CubeGeometry*>(baseGeom);
									ACPhysics::CubeGeometry* geomDst = new ACPhysics::CubeGeometry();
									rttr::instance rttrGeom(geomDst);

									rttr::type rttrCubeGeomType = rttr::type::get<ACPhysics::CubeGeometry*>();

									// Loop through CubeGeometry properties
									for (const auto& geomProp : rttrCubeGeomType.get_properties()) {
										rttr::variant geomPropSrc = geomProp.get_value(geomSrc);

										/*if (geomPropSrc.is_valid()) {
											AeonCore_INFO("DuplicateEntity - CubeGeometry Props: {}", geomProp.get_name());
										} else {
											AeonCore_INFO("DuplicateEntity - CubeGeometry invalid Props: {}", geomProp.get_name());
										}*/
										geomProp.set_value(rttrGeom, geomPropSrc);
									}

									baseGeom = geomDst;

									break;
								}
							case Collider::ColliderType::SPHERE:
								{
									ACPhysics::SphereGeometry* geomSrc = dynamic_cast<ACPhysics::SphereGeometry*>(baseGeom);
									ACPhysics::SphereGeometry* geomDst = new ACPhysics::SphereGeometry();
									rttr::instance rttrGeom(geomDst);

									rttr::type rttrSphereGeomType = rttr::type::get<ACPhysics::SphereGeometry*>();

									// Loop through SphereGeometry properties
									for (const auto& geomProp : rttrSphereGeomType.get_properties()) {
										geomProp.set_value(rttrGeom, geomProp.get_value(geomSrc));
									}

									baseGeom = geomDst;

									break;
								}
							case Collider::ColliderType::CAPSULE:
								{
									ACPhysics::CapsuleGeometry* geomSrc = dynamic_cast<ACPhysics::CapsuleGeometry*>(baseGeom);
									ACPhysics::CapsuleGeometry* geomDst = new ACPhysics::CapsuleGeometry();
									rttr::instance rttrGeom(geomDst);

									rttr::type rttrCapsuleGeomType = rttr::type::get<ACPhysics::CapsuleGeometry*>();

									// Loop through CapsuleGeometry properties
									for (const auto& geomProp : rttrCapsuleGeomType.get_properties()) {
										geomProp.set_value(rttrGeom, geomProp.get_value(geomSrc));
									}

									baseGeom = geomDst;

									break;
								}
							default: ;
						}

						// need to delete the old Geometry for the destination Collider
						if (colliderCompDst.GetGeometry())
							delete colliderCompDst.GetGeometry();

						prop.set_value(rttrInstDst, baseGeom);
						continue;
					}
					//if (prop.get_type() == rttr::type::get<Collider::ColliderType>()) continue;

					prop.set_value(rttrInstDst, propValueSrc);
				}
			}

			AddComponent<Collider>(duplicated_entity, colliderCompDst);
		}
		// RigidBody
		if (HasComponent<RigidBody>(entityToCopy))
		{
			const RigidBody& rbCompSrc = GetComponent<RigidBody>(entityToCopy.GetEntityID());
			RigidBody rbCompDst;

			// RTTR to copy values
			{
				rttr::instance rttrInstDst(rbCompDst);
				rttr::instance rttrInstSrc(rbCompSrc);
				rttr::type rttrRenderType = rttr::type::get(rbCompSrc);
				
				// copy all properties' values from src to dst
				for (const auto& prop : rttrRenderType.get_properties()) {
					prop.set_value(rttrInstDst, prop.get_value(rttrInstSrc));
				}
			}

			AddComponent<RigidBody>(duplicated_entity, rbCompDst);
		}
		// Audio
		if (HasComponent<AudioComponent>(entityToCopy))
		{
			AudioComponent& audioCompSrc = GetComponent<AudioComponent>(entityToCopy.GetEntityID());
			AudioComponent audioCompDst;

			// RTTR to copy values
			{
				rttr::instance rttrInstDst(audioCompDst);
				rttr::instance rttrInstSrc(audioCompSrc);
				rttr::type rttrRenderType = rttr::type::get(audioCompSrc);

				// copy all properties' values from src to dst
				for (const auto& prop : rttrRenderType.get_properties()) {
					rttr::variant propValueSrc = prop.get_value(rttrInstSrc);

					// AudioComponent only has 1 string property, the audio file name. So if it's empty, we don't call the setter
					if (prop.get_type() == rttr::type::get<std::string>() && propValueSrc.get_value<std::string>().empty()) {
						continue;
					}

					prop.set_value(rttrInstDst, propValueSrc);
				}
			}

			AddComponent<AudioComponent>(duplicated_entity, audioCompDst);

		}

		// These components cannot copy using C++ synthesized logic
		// Behaviour
		/*if (HasComponent<Behaviour>(entityToCopy.GetEntityID()))
		{
			Behaviour behaviourPtr = GetComponent<Behaviour>(entityToCopy.GetEntityID());
			Behaviour c = behaviourPtr;
			AddComponent<Behaviour>(duplicated_entity, c);
		}*/

		// Loop original entityToCopy children
		if ( scene->mParentChild.find(entityToCopy.GetEntityID()) != scene->mParentChild.end() )
		{
			for ( auto &child : scene->mParentChild[entityToCopy.GetEntityID()] )
			{
				DuplicateEntity(*GetEntity(child), duplicated_entity.GetEntityID());
			}
		}
	}

	Entity *Coordinator::GetEntity(EntityID id)
	{
		auto scene = SceneManager::GetInstance().GetSceneFromEntity(id);

		if (scene == nullptr){
			AeonCore_WARN("Failed to get scene in GetEntity");
			return nullptr;
		}

		return scene->GetEntity(id);
	}

	Entity *Coordinator::GetEntityByName(std::string name)
	{
		std::shared_ptr<Scene> scene = SceneManager::GetInstance().GetActiveScene();

		for ( int i = 0; i < scene->mEntities.size(); ++i )
		{
			if ( scene->mEntities[i].GetEntityName() == name )
			{
				return &scene->mEntities[i];
			}
		}
		return nullptr;
	}

	Entity *Coordinator::GetEntityByUUID(UUID _uuid)
	{
		std::shared_ptr<Scene> scene = SceneManager::GetInstance().GetActiveScene();

		for ( int i = 0; i < scene->mEntities.size(); ++i )
		{
			if ( scene->mEntities[i].GetUUID() == _uuid )
			{
				return &scene->mEntities[i];
			}
		}
		return nullptr;
	}

	Signature Coordinator::GetEntitySignature(EntityID _entityID)
	{
		auto scene = SceneManager::GetInstance().GetSceneFromEntity(_entityID);

		if (scene == nullptr){
			AeonCore_ASSERT(false, "Failed to get scene in GetEntitySignature")
			return Signature{};
		}

		return scene->GetSignature(_entityID);
	}

	Signature Coordinator::GetEntitySignature(Entity &_entity)
	{
		return GetEntitySignature(_entity.GetEntityID());
	}

	Entity& Coordinator::CreateChild(EntityID parent, const std::string& _name)
	{
		/*std::shared_ptr<Scene> scene = SceneManager::GetInstance().GetActiveScene();
		(*GetEntity(parent)).SetIsParent(true);

		Entity e = mEntityManager->CreateChild(parent, __name__);
		scene->mParentChild[parent].emplace_back(e.GetEntityID());
		
		return scene->GetEntities().emplace_back(e);*/
		return SceneManager::GetInstance().GetSceneFromEntity(parent)->CreateChild(parent, _name);
	}

	void Coordinator::ToChild(EntityID parent, EntityID child)
	{
		ToChild(*GetEntity(parent), *GetEntity(child));
	}

	void Coordinator::ToChild(Entity& parent, Entity& child) {
		/*Scene* scene = SceneManager::GetInstance().GetActiveScene();

		parent.SetIsParent(true);

		// if child entity was a child of another entity, modify the previous parent to remove the child
		if ( child.IsChild() && child.GetParent() != parent.GetEntityID() )
		{
			auto childrenIt = scene->mParentChild.find(child.GetParent());

			if (childrenIt != scene->mParentChild.end()) {
				// Remove child from the vector in mParentChild
				std::vector<EntityID> &childs = childrenIt->second;

				childs.erase(std::find(childs.begin(), childs.end(), child.GetEntityID()));

				if (childs.empty())
				{
					GetEntity(child.GetParent())->SetIsParent(false);
					scene->mParentChild.erase(child.GetParent());
				}				
			}
		}
		child.SetIsChild(true);
		child.SetParentID(parent.GetParent());

		scene->mParentChild[parent.GetEntityID()].emplace_back(child.GetEntityID());*/
		SceneManager::GetInstance().GetSceneFromEntity(parent.GetEntityID())->ToChild(parent, child);
	}

	void Coordinator::UnChild(EntityID parent, EntityID child)
	{
		/*Scene* scene = SceneManager::GetInstance().GetActiveScene();

		Entity *entity_child = GetEntity(child);
		entity_child->SetIsChild(false);
		entity_child->SetParentID(INVALID_ENTITY_ID);

		std::vector<EntityID> &children = scene->mParentChild[parent];
		auto itr = std::find(children.begin(), children.end(), child);
		if ( itr != children.end() )
		{
			scene->mParentChild[parent].erase(itr);
		}

		if ( scene->mParentChild[parent].size() == 0 )
		{
			Entity *entity_parent = GetEntity(parent);
			if ( entity_parent )
			{
				entity_parent->SetIsParent(false);
			}
			scene->mParentChild.erase(parent);
		}*/
		SceneManager::GetInstance().GetSceneFromEntity(parent)->RemoveParent(*GetEntity(child));
	}

	std::vector<EntityID> Coordinator::GetChildObjects(EntityID id)
	{
		std::shared_ptr<Scene> scene = SceneManager::GetInstance().GetSceneFromEntity(id);

		auto it = scene->mParentChild.find(id);
		if ( it != scene->mParentChild.end() )
		{
			return scene->mParentChild[id];
		}
		// Else return empty vector
		return std::vector<EntityID>();
	}

	Entity *Coordinator::GetChildObject(EntityID parent, uint32_t index)
	{
		auto children = GetChildObjects(parent);
		AeonCore_ASSERT(!children.empty() , "Calling GetChildObject on entity without a child!");

		return GetEntity(children[index]);
	}

	void Coordinator::GetAllChildren(std::vector<EntityID> &container, EntityID id)
	{
		container.emplace_back(id);

		for ( auto &i : GetChildObjects(id) )
		{
			GetAllChildren(container, i);
		}
	}

	void Coordinator::DestroyEntity(Entity &e, SceneRef scene)
	{
		DestroyEntity(e.GetEntityID(), scene);
	}

	void Coordinator::DestroyEntity(EntityID e, SceneRef scene)
	{
		if (scene == nullptr)
			scene = SceneManager::GetInstance().GetSceneFromEntity(e);

		// Insert children before parent, so that in the request fulfill order, children gets destroyed before the parent does
		for ( auto &child : GetChildObjects(e) )
		{
			DestroyEntity(child, scene);
		}

		scene->mDeleteEntitiesRequest.insert(e);
#if 0
		Entity* entityPtr = GetEntity(e);
		if (entityPtr == nullptr) return;
		Entity &entity = *entityPtr;
		std::string prefabName = entity.GetPrefab();
		RemoveFromPrefabMap(prefabName, e);
		EntityID parentID = entity.GetParent();
		if ( parentID < MAX_ENTITIES )
		{
			std::vector<EntityID> &children = mParentChild[parentID];
			auto itr = std::find(children.begin(), children.end(), e);
			if ( itr != children.end() )
			{
				mParentChild[parentID].erase(itr);
			}

			if ( mParentChild[parentID].size() == 0 )
			{
				Entity *entity_parent = GetEntity(parentID);
				if ( entity_parent )
				{
					entity_parent->SetIsParent(false);
				}
				mParentChild.erase(parentID);
			}
		}

		// Remove entity from mEntities container
		int index = 0;
		for ( int i = 0; i < mEntities.size(); ++i )
		{
			if ( e == mEntities[i].GetEntityID() )
			{
				index = i;
				break;
			}
		}
		mEntities.erase(mEntities.begin() + index);

		mEntityManager->DestroyEntity(e);
		mComponentManager->DestroyEntity(e);
		mSystemManager->DestroyEntity(e);
#endif
	}

	std::vector<Entity> &Coordinator::GetEntities()
	{
		return SceneManager::GetInstance().GetActiveScene()->GetEntities();
	}

	std::vector<EntityID> &Coordinator::GetPrefabContainer(std::string prefabName)
	{
		return mPrefabReloading[prefabName];
	}

	void Coordinator::UntagPrefab(EntityID id)
	{
		std::string prefabName = GetEntity(id)->GetPrefab();
		GetEntity(id)->SetPrefab("");

		RemoveFromPrefabMap(prefabName, id);
	}

	void Coordinator::AddToPrefabMap(std::string prefabName, EntityID id)
	{
		std::vector<EntityID> &value = mPrefabReloading[prefabName];
		for ( int i = 0; i < value.size(); ++i )
		{
			if ( id == value[i] )
			{
				return;
			}
		}

		value.emplace_back(id);
	}

	void Coordinator::RemoveFromPrefabMap(std::string prefabName, EntityID id)
	{
		std::vector<EntityID> &value = mPrefabReloading[prefabName];

		int index = INVALID_ENTITY_ID;
		for ( int i = 0; i < value.size(); ++i )
		{
			if ( id == value[i] )
			{
				index = i;
				break;
			}
		}

		if ( index < MAX_ENTITIES )
		{
			value.erase(value.begin() + index);
		}
	}

	void Coordinator::RefreshSystems() {
		mSystemManager->DestroyAllEntities(); // Clears the mEntities std::set in all the systems

		// iterating through all the entities, we add all of them to the systems
		for (auto& entity : SceneManager::GetInstance().GetActiveScene()->mEntities) {
			mSystemManager->EntitySignatureChanged(entity.GetEntityID(), GetEntitySignature(entity));
		}
	}

	bool Coordinator::EntityExists(EntityID id)
	{
		/*for ( int i = 0; i < mEntities.size(); ++i )
		{
			if ( id == mEntities[i].GetEntityID() )
			{
				return true;
			}
		}*/

		if (id == INVALID_ENTITY_ID) return false;
		
		auto scene = SceneManager::GetInstance().GetSceneFromEntity(id);

		return scene != nullptr && scene->EntityExists(id);
	}

	bool Coordinator::CheckParentIsActive(Entity &e)
	{
		if ( e.GetIsActive() )
		{
			EntityID parent_id = e.GetParent();
			EntityID act_parentEntId = ECSUtils::ExtractRealEntityId(parent_id);

			if (act_parentEntId > 0 && act_parentEntId <= MAX_ENTITIES)
			{
				Entity &parent = *GetEntity(parent_id);
				return CheckParentIsActive(parent);
			}
			else
			{
				return e.GetIsActive();
			}
		}
		else
		{
			return false;
		}
	}

	bool Coordinator::CheckParentIsActive(EntityID id)
	{
		Entity &e = *GetEntity(id);
		return CheckParentIsActive(e);
	}

	void Coordinator::DeleteRequestedEntities() {
		std::shared_ptr<Scene> scene = SceneManager::GetInstance().GetActiveScene();

		for (auto& entID : scene->mDeleteEntitiesRequest) {
			Entity* entityPtr = GetEntity(entID);
			if (entityPtr == nullptr) return;
			Entity &entity = *entityPtr;
			std::string prefabName = entity.GetPrefab();
			RemoveFromPrefabMap(prefabName, entID);
			EntityID parentID = entity.GetParent();
			EntityID act_parentEntId = ECSUtils::ExtractRealEntityId(parentID);
			if (act_parentEntId > 0 && act_parentEntId <= MAX_ENTITIES)
			{
				std::vector<EntityID> &children = scene->mParentChild[parentID];
				auto itr = std::find(children.begin(), children.end(), entID);
				if ( itr != children.end() )
				{
					scene->mParentChild[parentID].erase(itr);
				}

				if ( scene->mParentChild[parentID].size() == 0 )
				{
					Entity *entity_parent = GetEntity(parentID);
					if ( entity_parent )
					{
						entity_parent->SetIsParent(false);
					}
					scene->mParentChild.erase(parentID);
				}
			}

			// Remove entity from mEntities container
			int index = 0;
			for ( int i = 0; i < scene->mEntities.size(); ++i )
			{
				if ( entID == scene->mEntities[i].GetEntityID() )
				{
					index = i;
					break;
				}
			}
			//mEntities[index] = mEntities[mEntities.size() - 1]; // move the last element into the removed entity's index
			//mEntities.resize(mEntities.size() - 1); // shrink the mEntities container by 1, to eliminate that last entity from the container
			scene->mEntities.erase(scene->mEntities.begin() + index);

			scene->DestroyEntity(entID);
			mComponentManager->DestroyEntity(entID);
			mSystemManager->DestroyEntity(entID);
		}

		scene->mDeleteEntitiesRequest.clear();
	}
}
