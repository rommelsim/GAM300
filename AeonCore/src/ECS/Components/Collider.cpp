#include "acpch.h"

/*!***************************************************************************
	\file			Collider.cpp
	\author			Ng Yuwen Felicia & Chua Yip Xuan
	\par			yuwenfelicia.ng\@digipen.edu & yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the Collider component and
					its functions
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Components/Collider.h"
#include "Physics/ACPhysicsInternal.hpp"
#include "Serialization/RegisterManager.hpp"

#include <PxPhysics.h>

#include "ECS/Systems/PhysicsSystem.h"


namespace AeonCore
{
	Collider::Collider(): mColliderType(ColliderType::CUBE)
	{
		mGeometry = new ACPhysics::CubeGeometry();
	}

	Collider::Collider(bool _showColliderBounds, bool _isTrigger, glm::vec3 _offset, ACPhysics::ACPhysicsMaterial _material, ColliderType _colType)
		: mShowColliderBounds(_showColliderBounds), mIsTrigger(_isTrigger), mOffset(_offset), mMaterial(_material), mColliderType(_colType) 
	{
		switch ( mColliderType )
		{
			case ColliderType::CUBE:
				mGeometry = new ACPhysics::CubeGeometry();
				break;
			case ColliderType::SPHERE:
				mGeometry = new ACPhysics::SphereGeometry();
				break;
			case ColliderType::CAPSULE:
				mGeometry = new ACPhysics::CapsuleGeometry();
				break;
			default:
				AeonCore_WARN("No Geometry!");
		}
	}

	Collider::Collider(const Collider& _rhs)
	: mShowColliderBounds(_rhs.mShowColliderBounds), mIsTrigger(_rhs.IsTrigger()), mOffset(_rhs.mOffset), mMaterial(_rhs.mMaterial),
	  mColliderType(_rhs.mColliderType), mShape(_rhs.mShape)
{
		switch (mColliderType )
		{
		case ColliderType::CUBE:
			mGeometry = new ACPhysics::CubeGeometry(*dynamic_cast<ACPhysics::CubeGeometry*>(_rhs.mGeometry));
			break;
		case ColliderType::SPHERE:
			mGeometry = new ACPhysics::SphereGeometry(*dynamic_cast<ACPhysics::SphereGeometry *>(_rhs.mGeometry));
			break;
		case ColliderType::CAPSULE:
			mGeometry = new ACPhysics::CapsuleGeometry(*dynamic_cast<ACPhysics::CapsuleGeometry *>(_rhs.mGeometry));
			break;
		default:
			AeonCore_WARN("No Geometry!");
		}
	}

	Collider& Collider::operator=(Collider const& _rhs)
	{
		if ( this != &_rhs )
		{
			Collider tmp(_rhs);
			Swap(*this, tmp);
		}
		return *this;
	}

	void Collider::Swap(Collider& col1, Collider& col2)
	{
		std::swap(col1.mShape, col2.mShape);
		std::swap(col1.mGeometry, col2.mGeometry);
		std::swap(col1.mPhysxMaterial, col2.mPhysxMaterial);
		std::swap(col1.mShowColliderBounds, col2.mShowColliderBounds);
		std::swap(col1.mIsTrigger, col2.mIsTrigger);
		std::swap(col1.mColliderType, col2.mColliderType);
		std::swap(col1.mHasChanged, col2.mHasChanged);
		std::swap(col1.mMaterial, col2.mMaterial);
		std::swap(col1.mOffset, col2.mOffset);
	}

	Collider::~Collider()
	{
		if( mGeometry )
		{
			delete mGeometry;
			mGeometry = nullptr;
		}
	}

	void Collider::Destroy()
	{
	}

	bool Collider::GetIsShowingBounds() const
	{
		return mShowColliderBounds;
	}

	bool Collider::IsTrigger() const
	{
		return mIsTrigger;
	}

	glm::vec3 Collider::GetOffset() const
	{
		return mOffset;	
	}

	ACPhysics::ACPhysicsMaterial Collider::GetMaterial() const
	{
		return mMaterial;
	}

	physx::PxMaterial* Collider::GetPhysxMaterial()
	{
		return mPhysxMaterial;
	}

	physx::PxShape* Collider::GetPhysxShape()
	{
		return mShape;
	}

	ACPhysics::Geometry* Collider::GetGeometry()
	{
		return mGeometry;
	}

	void Collider::SetIsShowingBounds(bool _show)
	{
		mShowColliderBounds = _show;
		mHasChanged = true;
	}

	void Collider::SetTrigger(bool _trigger)
	{
		mIsTrigger = _trigger;
		mHasChanged = true;
	}

	void Collider::SetOffset(const glm::vec3 &_offset)
	{
		mOffset = _offset;
		mHasChanged = true;
	}

	void Collider::SetOffsetProxy(const glm::vec3 _offset)
	{
		SetOffset(_offset);
	}

	void Collider::SetMaterial(const ACPhysics::ACPhysicsMaterial &_material)
	{
		mMaterial = _material;
		mHasChanged = true;
	}

	void Collider::SetMaterialProxy(const ACPhysics::ACPhysicsMaterial _material)
	{
		SetMaterial(_material);
	}

	void Collider::SetPhysxMaterial(physx::PxMaterial* _mat)
	{
		mPhysxMaterial = _mat;
	}

	void Collider::SetPhysxShape(physx::PxShape* _shape)
	{
		mShape = _shape;
	}

	bool Collider::GetHasChanged() const
	{
		return mHasChanged;
	}


	void Collider::SetHasChanged(const bool _value)
	{
		mHasChanged = _value;
	}


	void Collider::SetGeometry(ACPhysics::Geometry* _geom)
	{
		mGeometry = _geom;
		mHasChanged = true;
	}

	Collider::ColliderType Collider::GetColliderType()
	{
		return mColliderType;
	}

	void Collider::SetColliderType(ColliderType _type)
	{
		mColliderType = _type;
		mHasChanged = true;
	}

	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<Collider>("Collider")
			.constructor<bool, bool, glm::vec3, ACPhysics::ACPhysicsMaterial, Collider::ColliderType>()()
			.constructor<Collider&>()()
			.property_readonly("id", &Collider::GetComponentID)
			.property("show bounds", &Collider::GetIsShowingBounds, &Collider::SetIsShowingBounds)
			.property("trigger", &Collider::IsTrigger, &Collider::SetTrigger)
			.property("offset", &Collider::GetOffset, &Collider::SetOffsetProxy)
			.property("collider type", &Collider::GetColliderType, &Collider::SetColliderType)
			.property("material", &Collider::GetMaterial, &Collider::SetMaterialProxy)
			.property("geometry", &Collider::GetGeometry, &Collider::SetGeometry);
		 }, 13);
	}
}
