#pragma once
/*!***************************************************************************
	\file			ACPhysicsCallback.h
	\author			Ng Yuwen Felicia & Chua Yip Xuan
	\par			yuwenfelicia.ng\@digipen.edu & yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the Contact Listener overriding
					functions implementations
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <PxSimulationEventCallback.h>

namespace AeonCore::ACPhysics
{
	class ACPhysicsCallback : public physx::PxSimulationEventCallback
	{
	public:
		void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count) override;
		void onWake(physx::PxActor **actors, physx::PxU32 count) override;
		void onSleep(physx::PxActor **actors, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs) override;
		void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count) override;
		void onAdvance(const physx::PxRigidBody *const *bodyBuffer, const physx::PxTransform *poseBuffer, const physx::PxU32 count) override;
	};
}
