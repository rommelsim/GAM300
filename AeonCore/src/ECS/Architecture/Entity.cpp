#include "acpch.h"
/*!***************************************************************************
	\file			Entity.cpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage the entities in the ECS architecture.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Architecture/Entity.hpp"

#include "ECS/Components/Transform.h"
#include "ECS/Architecture/Coordinator.hpp"

#include "Core/Log.h"
#include "Core/SceneManager.h"
#include "ECS/Architecture/ECSUtils.hpp"

#define DEFAULT_ENTITY_NAME "Entity "

namespace AeonCore
{
	Entity::Entity(EntityID _id, std::string _name) : id{ _id }, mUUID(UUID()), mName{ _name }, mParent{ INVALID_ENTITY_ID }, mIsChild{ false }, mIsParent{ false }, mIsActive{ true } {}

	Entity::Entity(EntityID _id, EntityID _parent, std::string _name) : id{ _id }, mUUID(UUID()), mName{ _name }, mParent{ _parent }, mIsChild{ true }, mIsParent{ false }, mIsActive{ true } {}

	EntityID Entity::GetEntityID() const
	{
		return id;
	}

	void Entity::SetEntityName(std::string _name)
	{
		mName = _name;
	}

	std::string Entity::GetEntityName() const
	{
		return mName;
	}

	UUID Entity::GetUUID() const
	{
		return mUUID;
	}

	void Entity::Copy(const Entity &_rhs)
	{
		mIsChild = _rhs.mIsChild;
		mIsParent = _rhs.mIsParent;
		mIsActive = _rhs.mIsActive;

		mPrefab = _rhs.GetPrefab();
	}

	bool Entity::IsChild() const
	{
		return mIsChild;
	}

	void Entity::SetIsChild(bool _rhs)
	{
		mIsChild = _rhs;
	}

	bool Entity::IsParent() const
	{
		return mIsParent;
	}

	void Entity::SetIsParent(bool _rhs)
	{
		mIsParent = _rhs;
	}

	void Entity::SetParentID(EntityID _rhs)
	{
		mParent = _rhs;
	}

	void Entity::SetIsActive(bool _rhs)
	{
		mIsActive = _rhs;
	}

	bool Entity::GetIsActive() const
	{
		return mIsActive;
	}

	EntityID Entity::GetParent() const
	{
		return mParent;
	}

	void Entity::SetPrefab(std::string _prefab)
	{
		mPrefab = _prefab;
	}

	std::string Entity::GetPrefab() const
	{
		return mPrefab;
	}
	/*void EntityManager::UpdateEntityTransform(EntityID parentID)
	{
		Coordinator& gCoordinator = Coordinator::GetInstance();
		Transform& parentTransform = gCoordinator.GetComponent<Transform>(parentID);
		auto allChildren = gCoordinator.GetChildObjects(parentID);
		
		for (EntityID child : allChildren) {
			Transform& childTransform = gCoordinator.GetComponent<Transform>(child);
			childTransform.UpdateMatrixWithOffset(parentTransform);
		}
	}*/
}

RTTR_REGISTRATION
{
	using namespace AeonCore;

	rttr::registration::class_<Entity>("Entity")
		.constructor<>()
		.property("id", &Entity::id)
		.property_readonly("uuid", &Entity::GetUUID)
		.property("mName", &Entity::GetEntityName, &Entity::SetEntityName)
		.property("mPrefab", &Entity::GetPrefab, &Entity::SetPrefab)
		.property("mParent", &Entity::GetParent, &Entity::SetParentID)
		.property("mIsChild", &Entity::IsChild, &Entity::SetIsChild)
		.property("mIsParent", &Entity::IsParent, &Entity::SetIsParent)
		.property("mIsActive", &Entity::GetIsActive, &Entity::SetIsActive)
		;
}