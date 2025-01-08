#include "acpch.h"
/*!***************************************************************************
	\file			ACPhysicsCallback.cpp
	\author			Ng Yuwen Felicia & Chua Yip Xuan
	\par			yuwenfelicia.ng\@digipen.edu & yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the Contact Listener overriding
					functions implementations
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Physics/ACPhysicsCallback.h"

#include <PxActor.h>

#include "Core/Log.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Architecture/Entity.hpp"
#include "ECS/Components/Behaviour.h"

namespace AeonCore::ACPhysics
{
	void ACPhysicsCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		PX_UNUSED(constraints);
		PX_UNUSED(count);
	}

	void ACPhysicsCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		PX_UNUSED(actors);
		PX_UNUSED(count);
	}

	void ACPhysicsCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		PX_UNUSED(actors);
		PX_UNUSED(count);
	}

	void ACPhysicsCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs,
		physx::PxU32 nbPairs)
	{
		EntityID ent1 = reinterpret_cast<EntityID>(pairHeader.actors[0]->userData);
		EntityID ent2 = reinterpret_cast<EntityID>(pairHeader.actors[1]->userData);
		auto coord = &Coordinator::GetInstance();

		if (coord->GetEntity(ent1) && coord->GetEntity(ent2))
		{
			for (physx::PxU32 i = 0; i < nbPairs; i++)
			{
				const physx::PxContactPair& pair = pairs[i];

				if ((pair.flags & physx::PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH) == physx::PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
				{
					//AeonCore_INFO("On Contact first touch Entity 1: {0}, Entity 2: {1}", ent1->GetEntityName(), ent2->GetEntityName());
					if (coord->HasComponent<Behaviour>(ent1))
					{
						auto behaviour = coord->GetComponent<Behaviour>(ent1);
						behaviour.InvokeOnCollisionEnter(ent2);
					}
					if (coord->HasComponent<Behaviour>(ent2))
					{
						auto behaviour = coord->GetComponent<Behaviour>(ent2);
						behaviour.InvokeOnCollisionEnter(ent1);
					}
				}
				else if ((pair.flags & physx::PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH) == physx::PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH)
				{
					//AeonCore_INFO("On Contact lost touch Entity 1: {0}, Entity 2: {1}", ent1->GetEntityName(), ent2->GetEntityName());
					if (coord->HasComponent<Behaviour>(ent1))
					{
						auto behaviour = coord->GetComponent<Behaviour>(ent1);
						behaviour.InvokeOnCollisionExit(ent2);
					}
					if (coord->HasComponent<Behaviour>(ent2))
					{
						auto behaviour = coord->GetComponent<Behaviour>(ent2);
						behaviour.InvokeOnCollisionExit(ent1);
					}
				}
				else
				{
					//AeonCore_INFO("On Contact update Entity 1: {0}, Entity 2: {1}", ent1->GetEntityName(), ent2->GetEntityName());
					if (coord->HasComponent<Behaviour>(ent1))
					{
						auto behaviour = coord->GetComponent<Behaviour>(ent1);
						behaviour.InvokeOnCollisionUpdate(ent2);
					}
					if (coord->HasComponent<Behaviour>(ent2))
					{
						auto behaviour = coord->GetComponent<Behaviour>(ent2);
						behaviour.InvokeOnCollisionUpdate(ent1);
					}
				}
				//AeonCore_INFO("Contact points{0}", nbPairs);
			}
		}

		if (ent1 && ent2)
		{
			
			
		}
	}

	void ACPhysicsCallback::onTrigger(physx::PxTriggerPair * pairs, physx::PxU32 count)
	{
		auto coord = &Coordinator::GetInstance();

		for (physx::PxU32 i = 0; i < count; ++i)
		{
		/*	EntityID ent1;
			EntityID ent2;*/
		/*	ent1 = reinterpret_cast<EntityID>(pairHeader.actors[0]->userData);
			ent2 = reinterpret_cast<EntityID>(pairHeader.actors[1]->userData);*/
			const physx::PxTriggerPair& pair = pairs[i];
			/*Entity* ent1 = static_cast<Entity*>(pairs->triggerActor->userData);
			Entity* ent2 = static_cast<Entity*>(pairs->otherActor->userData);*/
			EntityID ent1 = reinterpret_cast<EntityID>(pairs->triggerActor->userData);
			EntityID ent2 = reinterpret_cast<EntityID>(pairs->otherActor->userData);

			Entity* entity1 = coord->GetEntity(ent1);
			Entity* entity2 = coord->GetEntity(ent2);

			/*if (entity1 != nullptr) {
				AeonCore_INFO("onTrigger, entity 1 {}", entity1->GetEntityName());
			}
			if (entity2 != nullptr) {
				AeonCore_INFO("onTrigger, entity 2 {}", entity2->GetEntityName());
			}*/

			//AeonCore_INFO("onTrigger, ids are {} {}", ent1, ent2);
			if (entity1 == nullptr || entity2 == nullptr) continue;
			//AeonCore_INFO("onTrigger, got past null check");

			if ((pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				//AeonCore_INFO("On trigger first touch Entity 1: {0}, Entity 2: {1}", entity1->GetEntityName(), entity2->GetEntityName());
				if (coord->HasComponent<Behaviour>(ent1))
				{
					auto behaviour = coord->GetComponent<Behaviour>(ent1);
					behaviour.InvokeOnTriggerEnter(ent2);
				}
				if (coord->HasComponent<Behaviour>(ent2))
				{
					auto behaviour = coord->GetComponent<Behaviour>(ent2);
					behaviour.InvokeOnTriggerEnter(ent1);
				}
			}

			else if ((pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//AeonCore_INFO("On trigger lost touch Entity 1: {0}, Entity 2: {1}", entity1->GetEntityName(), entity2->GetEntityName());
				if (coord->HasComponent<Behaviour>(ent1))
				{
					auto behaviour = coord->GetComponent<Behaviour>(ent1);
					behaviour.InvokeOnTriggerExit(ent2);
				}
				if (coord->HasComponent<Behaviour>(ent2))
				{
					auto behaviour = coord->GetComponent<Behaviour>(ent2);
					behaviour.InvokeOnTriggerExit(ent1);
				}
			}
			else
			{
				AeonCore_INFO("On trigger update Entity 1: {0}, Entity 2: {1}", entity1->GetEntityName(), entity2->GetEntityName());
				if (coord->HasComponent<Behaviour>(ent1))
				{
					auto behaviour = coord->GetComponent<Behaviour>(ent1);
					behaviour.InvokeOnTriggerUpdate(ent2);
				}
				if (coord->HasComponent<Behaviour>(ent2))
				{
					auto behaviour = coord->GetComponent<Behaviour>(ent2);
					behaviour.InvokeOnTriggerUpdate(ent1);
				}
			}
		}
		//AeonCore_INFO("count {}:", count);
	}

	void ACPhysicsCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform * poseBuffer,
		const physx::PxU32 count)
	{
		PX_UNUSED(bodyBuffer);
		PX_UNUSED(poseBuffer);
		PX_UNUSED(count);
	}
}

