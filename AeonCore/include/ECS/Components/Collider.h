#pragma once
/*!***************************************************************************
	\file			Collider.h
	\author			Ng Yuwen Felicia & Chua Yip Xuan
	\par			yuwenfelicia.ng\@digipen.edu & yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the Collider component and
					its functions
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <PxRigidActor.h>
#include <PxShape.h>
#include <glm/glm.hpp>

#include "IComponent.h"
#include "Physics/ACPhysicsGeom.hpp"
#include "Physics/ACPhysicsMaterial.hpp"
#include "Core/AeonAPI.h"
#include "Physics/ACPhysicsUtility.hpp"


namespace AeonCore 
{
	class AEON_API Collider : public IComponent
	{
	public:
		enum class ColliderType
		{
			CUBE,
			SPHERE,
			CAPSULE,
			NONE
		};

		Collider();

		Collider(bool _showColliderBounds, bool _isTrigger, glm::vec3 _offset, ACPhysics::ACPhysicsMaterial _material, ColliderType _colType);

		Collider(const Collider& _rhs);

		Collider &operator=(Collider const &_rhs);
		void Swap(Collider& col1, Collider& col2);

		~Collider() override;

		void Destroy() override;

		bool							GetIsShowingBounds() const;
		bool							IsTrigger() const;
		glm::vec3						GetOffset() const;
		ACPhysics::ACPhysicsMaterial	GetMaterial() const;
		physx::PxMaterial				*GetPhysxMaterial();
		physx::PxShape					*GetPhysxShape();
		ACPhysics::Geometry				*GetGeometry();
		bool							GetHasChanged() const;
		ColliderType					GetColliderType();
		//CollisionInfo							GetCollisionInfo();

		void							SetIsShowingBounds(bool _show);
		void							SetTrigger(bool _trigger);
		void							SetOffset(const glm::vec3 &_offset);
		void							SetOffsetProxy(const glm::vec3 _offset);
		void							SetMaterial(const ACPhysics::ACPhysicsMaterial &_material);
		void							SetMaterialProxy(const ACPhysics::ACPhysicsMaterial _material);
		void							SetPhysxMaterial(physx::PxMaterial* _mat);
		void							SetPhysxShape(physx::PxShape* _shape);
		void							SetHasChanged(const bool _value);
		void							SetGeometry(ACPhysics::Geometry* _geom);
		void							SetColliderType(ColliderType _type);

	private:
		RTTR_ENABLE(IComponent);
		bool							mShowColliderBounds = true;			//serialize
		bool							mIsTrigger = false;					//serialize
		glm::vec3						mOffset = glm::vec3(0.0f);	//serialize
		ACPhysics::ACPhysicsMaterial	mMaterial;							//serialize
		ACPhysics::Geometry				*mGeometry = nullptr;				//serialize
		ColliderType					mColliderType;						//serialize

		bool							mHasChanged = false;				//dont need serialize
		physx::PxMaterial				*mPhysxMaterial = nullptr;			//dont need serialize
		physx::PxShape					*mShape = nullptr;					//dont need serialize
		//CollisionInfo					mCollisionInfo;
	};

}
