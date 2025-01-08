#include "acpch.h"
/*!***************************************************************************
	\file			RigidBody.cpp
	\author			Ng Yuwen Felicia & Chua Yip Xuan
	\par			yuwenfelicia.ng\@digipen.edu & yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the Rigidbody component and
					its functions implementations
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Components/RigidBody.h"

#include "ECS/Systems/PhysicsSystem.h"
#include "Physics/ACPhysicsInternal.hpp"
#include "Physics/ACPhysicsUtility.hpp"

#include "Serialization/RegisterManager.hpp"

namespace AeonCore
{
	RigidBody::RigidBody() : mMass(1.0f), mLinearDrag(0.5f), mAngularDrag(0.5f), mUseGravity(false),
		mIsAutoCenterOfMass(true), mCenterOfMass(0.0f, 0.0f, 0.0f),
		mIsAutoTensor(true), mInertiaTensor(0.5f, 0.5f, 0.5f),
		mInertiaTensorRotation(0.0f, 0.0f, 0.0f), mIsKinematic(false),
		mBodyType(RigidBodyType::STATIC)
	{
	}

	RigidBody::RigidBody(float _mass, float _linearDrag, float _angularDrag, bool _useGravity, bool _isAutoCenterOfMass,
						 bool _isAutoTensor, bool _isKinematic, RigidBodyType _type)
		: mMass(_mass), mLinearDrag(_linearDrag), mAngularDrag(_angularDrag),
		mUseGravity(_useGravity), mIsAutoCenterOfMass(_isAutoCenterOfMass),
		mCenterOfMass(0.0f, 0.0f, 0.0f), mIsAutoTensor(_isAutoTensor),
		mInertiaTensor(0.5f, 0.5f, 0.5f),
		mInertiaTensorRotation(0.0f, 0.0f, 0.0f),
		mIsKinematic(_isKinematic), mBodyType(_type)
	{
	}

	RigidBody::~RigidBody()
	{
		/*if ( mPhysxActor)
		{
			ACPhysics::PhysicsInternal::GetInstance().GetPhysicsScene().removeActor(*mPhysxActor);
			mPhysxActor->release();
			mPhysxActor = nullptr;
		}*/
	}

	void RigidBody::Destroy()
	{
		if ( mPhysxActor )
		{
			ACPhysics::PhysicsInternal::GetInstance().GetPhysicsScene().removeActor(*mPhysxActor);
			mPhysxActor->release();
			mPhysxActor = nullptr;
		}
	}

	float RigidBody::GetMass() const
	{
		return mMass;
	}

	float RigidBody::GetLinearDrag() const
	{
		return mLinearDrag;
	}

	float RigidBody::GetAngularDrag() const
	{
		return mAngularDrag;
	}

	bool RigidBody::GetUseGravity() const
	{
		return mUseGravity;
	}

	bool RigidBody::GetIsAutoCenterOfMass() const
	{
		return mIsAutoCenterOfMass;
	}

	glm::vec3 RigidBody::GetCenterOfMass() const
	{
		return mCenterOfMass;
	}

	bool RigidBody::GetIsAutoTensor() const
	{
		return mIsAutoTensor;
	}

	glm::vec3 RigidBody::GetInertiaTensor() const
	{
		return mInertiaTensor;
	}

	glm::vec3 RigidBody::GetInertiaTensorRotation() const
	{
		return mInertiaTensorRotation;
	}

	bool RigidBody::GetIsKinematic() const
	{
		return mIsKinematic;
	}

	RigidBody::RigidBodyType RigidBody::GetRigidBodyType() const
	{
		return mBodyType;
	}

	physx::PxRigidActor* RigidBody::GetPhysxActor()
	{
		return mPhysxActor;
	}

	void RigidBody::SetPhysxActor(physx::PxRigidActor* _actor)
	{
		mPhysxActor = _actor;
	}

	bool RigidBody::GetHasChanged() const
	{
		return mHasChanged;
	}

	void RigidBody::SetMass(const float _mass)
	{
		mMass = _mass;
		mHasChanged = true;
	}

	void RigidBody::SetLinearDrag(const float _ld)
	{
		mLinearDrag = _ld;
		mHasChanged = true;
	}

	void RigidBody::SetAngularDrag(const float _ad)
	{
		mAngularDrag = _ad;
		mHasChanged = true;
	}

	void RigidBody::SetUseGravity(const bool _useGravity)
	{
		mUseGravity = _useGravity;
		mHasChanged = true;
	}

	void RigidBody::SetIsAutoCenterOfMass(const bool _isAutoCenterOfMass)
	{
		mIsAutoCenterOfMass = _isAutoCenterOfMass;
		mHasChanged = true;
	}

	void RigidBody::SetCenterOfMass(const glm::vec3 _centerOfMass)
	{
		mCenterOfMass = _centerOfMass;
	}

	void RigidBody::SetIsAutoTensor(const bool _isAutoTensor)
	{
		mIsAutoTensor = _isAutoTensor;
		mHasChanged = true;
	}

	void RigidBody::SetInertiaTensor(const glm::vec3 _inertiaTensor)
	{
		mInertiaTensor = _inertiaTensor;
		mHasChanged = true;
	}

	void RigidBody::SetInertiaTensorRotation(const glm::vec3 _inertiaTensorRot)
	{
		mInertiaTensorRotation = _inertiaTensorRot;
		mHasChanged = true;
	}

	void RigidBody::SetIsKinematic(const bool _isKinematic)
	{
		mIsKinematic = _isKinematic;
		mHasChanged = true;
	}

	void RigidBody::SetRigidBodyType(const RigidBodyType _bodyType)
	{
		mBodyType = _bodyType;
		mHasChanged = true;
	}

	void RigidBody::SetHasChanged(const bool _value)
	{
		mHasChanged = _value;
	}

	void RigidBody::AddForce(glm::vec3 _force)
	{
		physx::PxRigidDynamic *actor = mPhysxActor->is<physx::PxRigidDynamic>();
		if ( actor )
			actor->addForce(ACPhysicsUtility::ToPhysXVector(_force), physx::PxForceMode::eFORCE);

	}

	void RigidBody::AddImpulse(glm::vec3 _force)
	{
		physx::PxRigidDynamic *actor = mPhysxActor->is<physx::PxRigidDynamic>();
		if ( actor )
			actor->addForce(ACPhysicsUtility::ToPhysXVector(_force), physx::PxForceMode::eIMPULSE);
	}

	glm::vec3 RigidBody::GetVelocity()
	{
		physx::PxRigidDynamic *actor = mPhysxActor->is<physx::PxRigidDynamic>();
		if ( actor )
			return ACPhysicsUtility::FromPhysXVector(actor->getLinearVelocity());
		else
			return glm::vec3(0.0f);

	}
	void RigidBody::SetVelocity(glm::vec3 _vel)
	{
		physx::PxRigidDynamic *actor = mPhysxActor->is<physx::PxRigidDynamic>();
		if ( actor )
			return actor->setLinearVelocity(ACPhysicsUtility::ToPhysXVector(_vel));
	}

	bool RigidBody::Raycast(const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance, RaycastResult &result)
	{
		physx::PxRaycastBuffer hitInfo;
		physx::PxRigidDynamic *actor = mPhysxActor->is<physx::PxRigidDynamic>();
		if ( !actor )
		{
			return false;
		}

		physx::PxScene *scene = actor->getScene();
		if ( !scene )
		{
			return false;
		}

		/*bool status =*/ scene->raycast(ACPhysicsUtility::ToPhysXVector(origin),      
			ACPhysicsUtility::ToPhysXVector(direction), maxDistance,hitInfo
		);

		if ( hitInfo.hasBlock )
		{
			result.hitPosition = ACPhysicsUtility::FromPhysXVector(hitInfo.block.position);
			result.hitNormal = ACPhysicsUtility::FromPhysXVector(hitInfo.block.normal);
			result.distance = hitInfo.block.distance;
			physx::PxRigidActor *hitActor = hitInfo.block.actor;
			if ( hitActor )
			{
				auto phySystem = std::dynamic_pointer_cast<PhysicsSystem>(Coordinator::GetInstance().GetSystem<PhysicsSystem>());

				if (phySystem)
				{
					result.id = phySystem->GetEntityIdFromActor(hitActor);
				}
			}
			else
			{
				result.id = 0; 
			}

			return true;
		}

		return false;
	}

	RTTR_REGISTRATION
	{
		using namespace AeonCore;
	RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<RigidBody>("RigidBody")
			.constructor()()
			.constructor<float, float, float, bool, bool, bool, bool, RigidBody::RigidBodyType>()()
			.property_readonly("id", &RigidBody::GetComponentID)
			.property("mass", &RigidBody::GetMass, &RigidBody::SetMass)
			.property("linear drag", &RigidBody::GetLinearDrag, &RigidBody::SetLinearDrag)
			.property("angular drag", &RigidBody::GetAngularDrag, &RigidBody::SetAngularDrag)
			.property("use gravity", &RigidBody::GetUseGravity, &RigidBody::SetUseGravity)
			.property("auto center of mass", &RigidBody::GetIsAutoCenterOfMass, &RigidBody::SetIsAutoCenterOfMass)
			.property("center of mass", &RigidBody::GetCenterOfMass, &RigidBody::SetCenterOfMass)
			.property("auto tensor", &RigidBody::GetIsAutoTensor, &RigidBody::SetIsAutoTensor)
			.property("inertia tensor", &RigidBody::GetInertiaTensor, &RigidBody::SetInertiaTensor)
			.property("inertia tensor rotation", &RigidBody::GetInertiaTensorRotation, &RigidBody::SetInertiaTensorRotation)
			.property("kinematic", &RigidBody::GetIsKinematic, &RigidBody::SetIsKinematic)
			.property("type", &RigidBody::GetRigidBodyType, &RigidBody::SetRigidBodyType);
		}, 10);
	}
}
