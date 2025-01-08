#include "acpch.h"
/*!***************************************************************************
	\file            Behaviour.cpp
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file defines the Behaviour component
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Components/Behaviour.h"

#include "Core/Log.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "Scripting/Scripting.h"
#include "Serialization/RegisterManager.hpp"

namespace AeonCore {
	Behaviour::Behaviour() : m_OwnerID{ 0 }
	{
		id = Coordinator::GetInstance().GetComponentType<Behaviour>();
	}

	Behaviour::Behaviour(EntityID ownerID) : m_OwnerID{ ownerID }
	{
		id = Coordinator::GetInstance().GetComponentType<Behaviour>();
	}

	void Behaviour::Destroy() {
		if (!mBehaviourNames.empty()) {
			//AeonCore_ERROR("Behaviour destructor from owner {0}", m_OwnerID);
			Scripting::DestroyEntityBehaviours(this);
		}
	}

	EntityID Behaviour::GetOwnerID() const {
		return this->m_OwnerID;
	}

	int Behaviour::AddBehaviourScript(const std::string& behaviourClassName) {
		if (HasBehaviourScript(behaviourClassName)) {
			AeonCore_INFO("{} has already been added into Behaviour component", behaviourClassName);
			return -1;
		}

		int idResult = Scripting::CreateBehaviourInstance(this->m_OwnerID, behaviourClassName);
		AeonCore_ASSERT(idResult > -1, "Failed to add " + behaviourClassName + " into Behaviour");
		mBehaviourNames.insert({ behaviourClassName, idResult });

		return idResult;
	}

	bool Behaviour::HasBehaviourScript(const std::string& behaviourClassName) {
		return mBehaviourNames.find(behaviourClassName) != mBehaviourNames.end();
	}

	std::shared_ptr<ScriptInstance> Behaviour::GetBehaviourScript(int bvrIdx) {
		if (bvrScriptInstances == nullptr)
			bvrScriptInstances = Scripting::GetBehaviourInstances(m_OwnerID);
		AeonCore_ASSERT(bvrScriptInstances != nullptr, "Behaviour owned by " + std::to_string(m_OwnerID) + " has nullptr for bvrScriptInstances")
		AeonCore_ASSERT(static_cast<unsigned int>(bvrIdx) < bvrScriptInstances->size(), "GetBehaviourScript");

		return (*bvrScriptInstances)[bvrIdx];
	}

	void Behaviour::InvokeUpdate() {
		if (bvrScriptInstances == nullptr)
			bvrScriptInstances = Scripting::GetBehaviourInstances(m_OwnerID);

		if (bvrScriptInstances == nullptr) return;
		for (auto& scriptInst : (*bvrScriptInstances)) {
			scriptInst->InvokeUpdate();
		}
	}

	void Behaviour::InvokeOnCollisionEnter(EntityID otherCollidingEntity) {
		if (bvrScriptInstances == nullptr)
			bvrScriptInstances = Scripting::GetBehaviourInstances(m_OwnerID);

		if (bvrScriptInstances == nullptr) return;

		for (auto& scriptInst : (*bvrScriptInstances)) {
			scriptInst->InvokeOnCollisionEnter(otherCollidingEntity);
		}
	}
	void Behaviour::InvokeOnCollisionUpdate(EntityID otherCollidingEntity) {
		if (bvrScriptInstances == nullptr)
			bvrScriptInstances = Scripting::GetBehaviourInstances(m_OwnerID);

		if (bvrScriptInstances == nullptr) return;

		for (auto& scriptInst : (*bvrScriptInstances)) {
			scriptInst->InvokeOnCollisionUpdate(otherCollidingEntity);
		}
	}
	void Behaviour::InvokeOnCollisionExit(EntityID otherCollidingEntity) {
		if (bvrScriptInstances == nullptr)
			bvrScriptInstances = Scripting::GetBehaviourInstances(m_OwnerID);

		if (bvrScriptInstances == nullptr) return;

		for (auto& scriptInst : (*bvrScriptInstances)) {
			scriptInst->InvokeOnCollisionExit(otherCollidingEntity);
		}
	}
	void Behaviour::InvokeOnTriggerEnter(EntityID otherCollidingEntity) {
		if (bvrScriptInstances == nullptr)
			bvrScriptInstances = Scripting::GetBehaviourInstances(m_OwnerID);

		if (bvrScriptInstances == nullptr) return;

		for (auto& scriptInst : (*bvrScriptInstances)) {
			scriptInst->InvokeOnTriggerEnter(otherCollidingEntity);
		}
	}
	void Behaviour::InvokeOnTriggerUpdate(EntityID otherCollidingEntity) {
		if (bvrScriptInstances == nullptr)
			bvrScriptInstances = Scripting::GetBehaviourInstances(m_OwnerID);

		if (bvrScriptInstances == nullptr) return;

		for (auto& scriptInst : (*bvrScriptInstances)) {
			scriptInst->InvokeOnTriggerUpdate(otherCollidingEntity);
		}
	}
	void Behaviour::InvokeOnTriggerExit(EntityID otherCollidingEntity) {
		if (bvrScriptInstances == nullptr)
			bvrScriptInstances = Scripting::GetBehaviourInstances(m_OwnerID);

		if (bvrScriptInstances == nullptr) return;

		for (auto& scriptInst : (*bvrScriptInstances)) {
			scriptInst->InvokeOnTriggerExit(otherCollidingEntity);
		}
	}

	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<Behaviour>("Behaviour")
			.constructor<EntityID>()
			.method("AddBehaviourScript", &Behaviour::AddBehaviourScript)
			.property_readonly("id", &Behaviour::GetComponentID)
			.property_readonly("ownerID", &Behaviour::GetOwnerID)
			.property("scripts", &Behaviour::mBehaviourNames);
		}, 7);
	}
}