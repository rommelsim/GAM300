#pragma once
/*!***************************************************************************
	\file			PhysicsSystem.h
	\author			Ng Yuwen Felicia & Chua Yip Xuan 
	\par			yuwenfelicia.ng\@digipen.edu & yipxuan.chua\@digipen.edu 		
	\date			Oct 1, 2023
	\brief			This header file consists of the Physics System
					and its functions declarations, to initialise, update and 
					destroy the system.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Architecture/System.hpp"
#include "Core/Singleton.hpp"

#include <memory>
#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>

#include "Core/AeonAPI.h"
#include "ECS/Architecture/Entity.hpp"
#include "ECS/Components/Collider.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Transform.h"

namespace AeonCore
{
	class PhysicsSystem : public System
	{
	public:
		void			Init() override;

		void			Update() override;

		void			Destroy() override;
		

		EntityID		GetEntityIdFromActor(physx::PxRigidActor *_actor);

		void			CreateAllActors();

		void AEON_API	AfterSceneLoaded();

		void AEON_API	DeleteActor(Entity _id);
	private:
		void CreateActor(Entity _id, Transform *_trans, RigidBody *_rb, Collider *_col);

		void PreSimulationUpdate();
		void PostSimulationUpdate();

		void UpdatePhysxRb(Entity _entity, Transform *_trans, RigidBody *_rb, Collider *_col);
		void UpdatePhysxCol(RigidBody *_rb, Collider *_col);
		void UpdatePhysxTrans(Entity _entity, Transform* _trans, RigidBody* _rb);

		void UpdateTrans(Transform* _trans, RigidBody* _rb);

		physx::PxScene	*mPhysicsScene = nullptr;
		bool			mAreActorsCreated = false;
		std::unordered_map<EntityID, RigidBody*> mActorsMap;
		bool mNeedToCreateActor = false;
		bool mNeedToUpdateShape = false;
	};
}
