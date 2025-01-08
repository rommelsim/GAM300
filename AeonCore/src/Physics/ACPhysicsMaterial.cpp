#include "acpch.h"
/*!***************************************************************************
	\file			ACPhysicsMaterial.cpp
	\author			Chua Yip Xuan & Felicia Ng Yuwen
	\par			yipxuan.chua\@digipen.edu & yuwenfelicia.ng\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the P
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Physics/ACPhysicsMaterial.hpp"
#include "Physics/ACPhysicsInternal.hpp"
#include "Serialization/RegisterManager.hpp"

namespace AeonCore::ACPhysics
{
	ACPhysicsMaterial::ACPhysicsMaterial(float _staticFriction, float _dynamicFriction, float _restitution) : mStaticFriction(_staticFriction), mDynamicFriction(_dynamicFriction), mRestitution(_restitution)
	{
	}

	float ACPhysicsMaterial::GetStaticFriction() const
	{
		return mStaticFriction;
	}

	float ACPhysicsMaterial::GetDynamicFriction() const
	{
		return mDynamicFriction;
	}

	float ACPhysicsMaterial::GetRestitution() const
	{
		return mRestitution;
	}


	void ACPhysicsMaterial::SetStaticFriction(const float _sf)
	{
		mStaticFriction = _sf;
	}

	void ACPhysicsMaterial::SetDynamicFriction(const float _df)
	{
		mDynamicFriction = _df;
	}

	void ACPhysicsMaterial::SetRestitution(const float _res)
	{
		mRestitution = _res;
	}

	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<ACPhysicsMaterial>("ACPhysicsMaterial")
			.constructor<float, float, float>()()
			.property("static friction", &ACPhysicsMaterial::GetStaticFriction, &ACPhysicsMaterial::SetStaticFriction)
			.property("dynamic friction", &ACPhysicsMaterial::GetDynamicFriction, &ACPhysicsMaterial::SetDynamicFriction)
			.property("restitution", &ACPhysicsMaterial::GetRestitution, &ACPhysicsMaterial::SetRestitution);
		}, 11);
	}
}
