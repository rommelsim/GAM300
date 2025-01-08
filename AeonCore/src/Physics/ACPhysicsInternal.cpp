#include "acpch.h"
/*!***************************************************************************
	\file			ACPhysicsInternal.cpp
	\author			Chua Yip Xuan & Felicia Ng Yuwen
	\par			yipxuan.chua\@digipen.edu & yuwenfelicia.ng\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the PhysicsInternal class and wraps the
					setup functions from physx.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Physics/ACPhysicsInternal.hpp"

#include <PxPhysics.h>
#include <foundation/PxPhysicsVersion.h>

#include "Physics/ACPhysicsMaterial.hpp"

#include "Core/Log.h"

namespace AeonCore::ACPhysics
{
	physx::PxFilterFlags FilterShaderExample(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags &pairFlags, const void *constantBlock, physx::PxU32 constantBlockSize)
	{
		if ( physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1) )
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT |
			physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS |
			physx::PxPairFlag::eNOTIFY_CONTACT_POINTS | physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
		pairFlags |= physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

		if ( (filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1) )
		{
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
			return physx::PxFilterFlag::eDEFAULT;
		}

		return physx::PxFilterFlag::eDEFAULT;
	}


	void PhysicsInternal::Init()
	{
		if ( !mFoundation ) mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
		if ( !mFoundation ) AeonCore_ERROR("PxCreateFoundation failed!");

#if _DEBUG
		CreatePVD();
#endif

		mToleranceScale.length = 1;
		mToleranceScale.speed = 10; 
#if _DEBUG
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPVD);
#else
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale);
#endif
		mDefaultCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		physx::PxSceneDesc sceneDesc = GetInstance().GetPhysics().getTolerancesScale();
		sceneDesc.gravity = physx::PxVec3(0.0f, -90.0f, 0.0f);
		sceneDesc.cpuDispatcher = &GetInstance().GetCpuDispatcher();
		sceneDesc.filterShader = FilterShaderExample;
		sceneDesc.bounceThresholdVelocity = 1.0f;
		sceneDesc.simulationEventCallback = &GetACPhysicsCallback();
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD; // Enable continuous collision detection
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
		mPhysicsScene = GetInstance().GetPhysics().createScene(sceneDesc);
		mPhysicsScene->setSimulationEventCallback(&gACPhysicsCallback);
#if _DEBUG
		InitializePVD();
#endif
		ACPhysicsMaterial defaultMat(0.6f, 0.6f, 0.5f);
		gMaterial = CreateMaterial(defaultMat);
		AeonCore_INFO("Succesfully initialized Physics Internal.");
	}

	physx::PxMaterial *PhysicsInternal::CreateMaterial(const ACPhysicsMaterial &_mat)
	{
		physx::PxMaterial *material = mPhysics->createMaterial(_mat.GetStaticFriction(), _mat.GetDynamicFriction(), _mat.GetRestitution());
		return material;
	}

	ACPhysicsCallback& PhysicsInternal::GetACPhysicsCallback()
	{
		return gACPhysicsCallback;
	}

	void PhysicsInternal::CreatePVD()
	{
		mPVD = PxCreatePvd(*mFoundation);
		if ( mPVD )
		{
			mTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
			bool isConnected = mPVD->connect(*mTransport, physx::PxPvdInstrumentationFlag::eALL);
			if (isConnected )
			{
				AeonCore_INFO("Successfully connected to Physx Visual Debugger.");
			}
			else
			{
				AeonCore_INFO("Failedto connect to Physx Visual Debugger.");
			}
		}
	}

	void PhysicsInternal::InitializePVD()
	{
		mPvdClient = mPhysicsScene->getScenePvdClient();
		if ( mPvdClient )
		{
			mPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			mPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			mPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void PhysicsInternal::Shutdown()
	{
		if ( mPhysicsScene )
		{
			mPhysicsScene->release();
			mPhysicsScene = nullptr;
		}
		if ( mPhysics )
		{
			mPhysics->release();
			mPhysics = nullptr;
		}
		if ( mDefaultCpuDispatcher )
		{
			mDefaultCpuDispatcher->release();
			mDefaultCpuDispatcher = nullptr;
		}
		if ( mPhysics )
		{
			mPhysics->release();
			mPhysics = nullptr;
		}
		if ( mPVD )
		{
			mPVD->release();
			mPVD = nullptr;
		}
		if ( mTransport )
		{
			mTransport->release();
			mTransport = nullptr;
		}
		if ( mFoundation )
		{
			mFoundation->release();
			mFoundation = nullptr;
		}
	}

	physx::PxPhysics& PhysicsInternal::GetPhysics()
	{
		return *mPhysics;
	}

	physx::PxAllocatorCallback& PhysicsInternal::GetAllocator()
	{
		return mDefaultAllocatorCallback;
	}

	physx::PxDefaultCpuDispatcher& PhysicsInternal::GetCpuDispatcher()
	{
		return *mDefaultCpuDispatcher;
	}

	physx::PxScene &PhysicsInternal::GetPhysicsScene()
	{
		return *mPhysicsScene;
	}

	physx::PxMaterial* PhysicsInternal::GetDefaultMaterial()
	{
		return gMaterial;
	}

}
