#pragma once
/*!***************************************************************************
	\file			ACPhysicsInternal.hpp
	\author			Chua Yip Xuan & Felicia Ng Yuwen
	\par			yipxuan.chua\@digipen.edu & yuwenfelicia.ng\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the PhysicsInternal class and wraps the 
					setup functions from physx.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <PxPhysicsAPI.h>

#include "ACPhysicsCallback.h"
#include "ACPhysicsMaterial.hpp"
#include "Core/AeonAPI.h"
#include "Core/Singleton.hpp"

namespace AeonCore::ACPhysics
{
	class AEON_API PhysicsInternal : public Singleton<PhysicsInternal>
	{
	public:
		void Init();
		void Shutdown();
		void CreatePVD();
		void InitializePVD();

		physx::PxPhysics& GetPhysics();
		physx::PxAllocatorCallback& GetAllocator();
		physx::PxDefaultCpuDispatcher& GetCpuDispatcher();
		physx::PxScene  &GetPhysicsScene();

		physx::PxMaterial *GetDefaultMaterial();
		physx::PxMaterial *CreateMaterial(const ACPhysicsMaterial &_mat);
		ACPhysicsCallback &GetACPhysicsCallback();
	private:
		friend class Singleton<PhysicsInternal>;

		physx::PxDefaultAllocator		mDefaultAllocatorCallback;
		physx::PxDefaultErrorCallback	mDefaultErrorCallback;
		physx::PxDefaultCpuDispatcher	*mDefaultCpuDispatcher;
		physx::PxFoundation				*mFoundation ;
		physx::PxPhysics				*mPhysics ;
		physx::PxScene					*mPhysicsScene;
		physx::PxPvd					*mPVD ;
		physx::PxPvdTransport			*mTransport;
		physx::PxPvdSceneClient			*mPvdClient ;
		physx::PxTolerancesScale		mToleranceScale;
		physx::PxMaterial				*gMaterial;
		ACPhysicsCallback				gACPhysicsCallback;
	};

}