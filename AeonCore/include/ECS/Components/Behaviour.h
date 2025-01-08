#pragma once
/*!***************************************************************************
	\file            Behaviour.h
	\author          Lam En Qing
	\par             enqing.lam\@digipen.edu
	\brief           This file declares the Behaviour component
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "IComponent.h"

#include <string>
#include <unordered_map>

#include "Core/AeonAPI.h"
#include "ECS/Architecture/ECS.hpp"

namespace AeonCore {

	class ScriptInstance;

	class AEON_API Behaviour : public IComponent {
	public:
		using BehaviourNamesMap = std::unordered_map<std::string, int>;

		// An integer is paired to a string representing the behaviour name. The integer is an index of the C# class instance, it points to a specific ScriptInstance
		BehaviourNamesMap mBehaviourNames;

		Behaviour();
		Behaviour(EntityID ownerID);

		void Destroy() override;
		EntityID GetOwnerID() const;

		int AddBehaviourScript(const std::string& behaviourClassName);
		bool HasBehaviourScript(const std::string& behaviourClassName);
		std::shared_ptr<ScriptInstance> GetBehaviourScript(int bvrIdx);

		/// <summary>
		/// Invokes the Update function for all scripts owned by this Behaviour component
		/// </summary>
		void InvokeUpdate();

		/// <summary>
		/// Invokes the OnCollisionEnter function for all scripts owned by this Behaviour component
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnCollisionEnter(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the InvokeOnCollisionUpdate function for all scripts owned by this Behaviour component
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnCollisionUpdate(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the InvokeOnCollisionExit function for all scripts owned by this Behaviour component
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnCollisionExit(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the InvokeOnTriggerEnter function for all scripts owned by this Behaviour component
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnTriggerEnter(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the InvokeOnTriggerUpdate function for all scripts owned by this Behaviour component
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnTriggerUpdate(EntityID otherCollidingEntity);
		
		/// <summary>
		/// Invokes the InvokeOnTriggerExit function for all scripts owned by this Behaviour component
		/// </summary>
		/// <param name="otherCollidingEntity">The EntityID of the other colliding entity</param>
		void InvokeOnTriggerExit(EntityID otherCollidingEntity);
	private:
		RTTR_ENABLE(IComponent);

		EntityID m_OwnerID;
		std::vector<std::shared_ptr<ScriptInstance>>* bvrScriptInstances{nullptr};
	};
}