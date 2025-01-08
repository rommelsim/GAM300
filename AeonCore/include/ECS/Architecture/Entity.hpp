#pragma once
/*!***************************************************************************
	\file			Entity.hpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file declares the Entity class
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "ECS.hpp"
#include "UUID.hpp"

#include <string>
#include <rttr/registration_friend.h>
#include <rttr/rttr_enable.h>

namespace AeonCore {
	class AEON_API Entity
	{
	public:
		Entity(EntityID _id = INVALID_ENTITY_ID, std::string _name = "");
		Entity(EntityID _id, EntityID _parent, std::string _name = "");

		// Getter, Setter
		EntityID GetEntityID() const;

		void SetEntityName(std::string _name);
		std::string GetEntityName() const;

		UUID GetUUID() const;

		// For duplication
		void Copy(const Entity &_rhs);

		bool IsChild() const;
		void SetIsChild(bool);

		bool IsParent() const;
		void SetIsParent(bool);
		void SetParentID(EntityID);

		void SetIsActive(bool);
		bool GetIsActive() const;
		EntityID GetParent() const;

		void SetPrefab(std::string _prefab);
		std::string GetPrefab() const;

		RTTR_ENABLE();
	private:
		RTTR_REGISTRATION_FRIEND
		EntityID	id;
		UUID		mUUID;
		std::string mName;
		std::string mPrefab{};
		EntityID	mParent; // parent id given that is_child is true
		bool		mIsChild;
		bool		mIsParent;
		bool		mIsActive;
	};
}
