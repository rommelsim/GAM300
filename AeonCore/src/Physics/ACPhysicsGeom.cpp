#include "acpch.h"
/*!***************************************************************************
	\file			ACPhysicsGeom.cpp
	\author			Chua Yip Xuan & Felicia Ng Yuwen
	\par			yipxuan.chua\@digipen.edu & yuwenfelicia.ng\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the P
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Physics/ACPhysicsGeom.hpp"
#include "Physics/ACPhysicsUtility.hpp"
#include "Serialization/RegisterManager.hpp"

namespace AeonCore::ACPhysics
{
	CubeGeometry::CubeGeometry(float _halfExtentX, float _halfExtentY, float _halfExtentZ) : mHalfExtent(glm::vec3(_halfExtentX, _halfExtentY, _halfExtentZ))
	{
		mGeometry = physx::PxBoxGeometry(mHalfExtent.x, mHalfExtent.y, mHalfExtent.z);
	}

	CubeGeometry::CubeGeometry(glm::vec3 _halfExtent) : mHalfExtent(_halfExtent)
	{
		mGeometry = physx::PxBoxGeometry(mHalfExtent.x, mHalfExtent.y, mHalfExtent.z);
	}

	physx::PxBoxGeometry &CubeGeometry::GetPhysxGeometry()
	{
		return mGeometry;
	}

	glm::vec3 CubeGeometry::GetHalfExtents()
	{
		return mHalfExtent;
	}

	void CubeGeometry::SetHalfExtents(const glm::vec3 _halfExtents)
	{
		mHalfExtent = _halfExtents;
		mGeometry.halfExtents = ACPhysicsUtility::ToPhysXVector(mHalfExtent);
		mHasChanged = true;
	}

	void CubeGeometry::SetHalfExtentsProxy(glm::vec3 _halfExtents)
	{
		SetHalfExtents(_halfExtents);
	}

	void CubeGeometry::SetHalfExtents(const float _halfExtentsX, const float _halfExtentsY, const float _halfExtentsZ)
	{
		mHalfExtent.x = _halfExtentsX;
		mHalfExtent.y = _halfExtentsY;
		mHalfExtent.z = _halfExtentsZ;
		mGeometry.halfExtents = ACPhysicsUtility::ToPhysXVector(mHalfExtent);
		mHasChanged = true;
	}

	SphereGeometry::SphereGeometry(): mRadius(1.0f)
	{
		mGeometry = physx::PxSphereGeometry(mRadius);
	}

	SphereGeometry::SphereGeometry(float _radius) : mRadius(_radius)
	{
		mGeometry = physx::PxSphereGeometry(mRadius);
	}

	physx::PxSphereGeometry &SphereGeometry::GetPhysxGeometry()
	{
		return mGeometry;
	}

	float SphereGeometry::GetRadius() const
	{
		return mRadius;
	}

	void SphereGeometry::SetRadius(const float _rad)
	{
		mRadius = _rad;
		mGeometry.radius = mRadius;
		mHasChanged = true;
	}

	CapsuleGeometry::CapsuleGeometry(): mRadius(1.0f), mHalfHeight(1.0f)
	{
		mGeometry = physx::PxCapsuleGeometry(mRadius, mHalfHeight);
	}

	CapsuleGeometry::CapsuleGeometry(float _radius, float _halfHeight) : mRadius(_radius), mHalfHeight(_halfHeight)
	{
		mGeometry = physx::PxCapsuleGeometry(mRadius, mHalfHeight);
	}

	physx::PxCapsuleGeometry &CapsuleGeometry::GetPhysxGeometry()
	{
		return mGeometry;
	}


	float CapsuleGeometry::GetRadius() const
	{
		return mRadius;
	}

	void CapsuleGeometry::SetRadius(const float _rad)
	{
		mRadius = _rad;
		mGeometry.radius = _rad;
		mHasChanged = true;
	}

	float CapsuleGeometry::GetHalfHeight() const
	{
		return mHalfHeight;
	}

	void CapsuleGeometry::SetHalfHeight(const float _hh)
	{
		mHalfHeight = _hh;
		mGeometry.halfHeight = _hh;
		mHasChanged = true;
	}

	PlaneGeometry::PlaneGeometry(float _normalX, float _normalY, float _normalZ, float _distance) : mNormal(glm::vec3(_normalX, _normalY, _normalX)), mDistance(_distance)
	{
	}

	PlaneGeometry::PlaneGeometry(glm::vec3 _normal, float _distance) : mNormal(_normal), mDistance(_distance)
	{
	}

	PlaneGeometry::PlaneGeometry(glm::vec3 _point, glm::vec3 _normal)
	{
		mNormal = _normal;
		mDistance = -glm::dot(_normal, _point);  
	}

	PlaneGeometry::PlaneGeometry(glm::vec3 _point0, glm::vec3 _point1, glm::vec3 _point2)
	{
		mNormal = glm::normalize(glm::cross(_point1 - _point0, _point2 - _point0));
		mDistance = -glm::dot(mNormal, _point0);
	}

	glm::vec3 PlaneGeometry::GetNormal() const
	{
		return mNormal;
	}

	void PlaneGeometry::SetNormal(const float _normalX, const float _normalY, const float _normalZ)
	{
		mNormal = glm::vec3(_normalX, _normalY, _normalZ);
	}

	void PlaneGeometry::SetNormal(glm::vec3 _normal)
	{
		mNormal = _normal;
	}

	void PlaneGeometry::SetNormalProxy(const glm::vec3 _normal)
	{
		SetNormal(_normal);
	}

	float PlaneGeometry::GetDistance() const
	{
		return mDistance;
	}

	void PlaneGeometry::SetDistance(const float _distance)
	{
		mDistance = _distance;
	}

	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<Geometry>("Geometry");
		rttr::registration::class_<CubeGeometry>("CubeGeometry")
			.constructor<float, float, float>()()
			.constructor<glm::vec3>()()
			.property("half extents", &CubeGeometry::GetHalfExtents, &CubeGeometry::SetHalfExtentsProxy);

		rttr::registration::class_<SphereGeometry>("SphereGeometry")
			.constructor<float>()()
			.property("radius", &SphereGeometry::GetRadius, &SphereGeometry::SetRadius);

		rttr::registration::class_<CapsuleGeometry>("CapsuleGeometry")
			.constructor<float, float>()()
			.property("radius", &CapsuleGeometry::GetRadius, &CapsuleGeometry::SetRadius)
			.property("half height", &CapsuleGeometry::GetHalfHeight, &CapsuleGeometry::SetHalfHeight);

		rttr::registration::class_<PlaneGeometry>("PlaneGeometry")
			.constructor<glm::vec3, float>()()
			.constructor<glm::vec3, glm::vec3>()()
			.constructor<glm::vec3, glm::vec3, glm::vec3>()()
			.property("normal", &PlaneGeometry::GetNormal, &PlaneGeometry::SetNormalProxy)
			.property("distance", &PlaneGeometry::GetDistance, &PlaneGeometry::SetDistance);
		}, 12);
	}
}
