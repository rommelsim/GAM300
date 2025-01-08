#pragma once
/*!***************************************************************************
	\file			ACPhysicsGeom.hpp
	\author			Chua Yip Xuan & Felicia Ng Yuwen
	\par			yipxuan.chua\@digipen.edu & yuwenfelicia.ng\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the P
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <PxPhysics.h>
#include <glm/glm.hpp>
#include "Core/AeonAPI.h"
#include "rttr/registration"
#include "rttr/registration_friend.h"

namespace AeonCore::ACPhysics
{
	class AEON_API Geometry
	{
	public:
		virtual ~Geometry() = default;
		bool mHasChanged = false;
		RTTR_ENABLE();
	};

	class AEON_API CubeGeometry : public Geometry
	{
	public:
		CubeGeometry(float _halfExtentX = 0.5f, float _halfExtentY = 0.5f, float _halfExtentZ = 0.5f);

		CubeGeometry(glm::vec3 _halfExtent);

		~CubeGeometry() = default;

		physx::PxBoxGeometry &GetPhysxGeometry();

		glm::vec3					GetHalfExtents();
		void						SetHalfExtents(const glm::vec3 _halfExtents);
		void						SetHalfExtentsProxy(glm::vec3 _halfExtents);
		void						SetHalfExtents(const float _halfExtentsX, const float _halfExtentsY, const float _halfExtentsZ);
	private:
		RTTR_ENABLE(Geometry);
		RTTR_REGISTRATION_FRIEND;
		glm::vec3					mHalfExtent;
		physx::PxBoxGeometry		mGeometry;
	};

	class AEON_API SphereGeometry : public Geometry
	{
	public:
		SphereGeometry();
		SphereGeometry(float _radius );
		~SphereGeometry() = default;

		physx::PxSphereGeometry &GetPhysxGeometry();

		float						GetRadius() const;
		void						SetRadius(const float _rad);
	private:
		RTTR_ENABLE(Geometry);
		RTTR_REGISTRATION_FRIEND;
		float						mRadius;
		physx::PxSphereGeometry		mGeometry;
	};

	class AEON_API CapsuleGeometry : public Geometry
	{
	public:
		CapsuleGeometry();
		CapsuleGeometry(float _radius, float _halfHeight);
		~CapsuleGeometry() = default;

		physx::PxCapsuleGeometry &GetPhysxGeometry();

		float						GetRadius() const;
		void						SetRadius(const float _rad);

		float						GetHalfHeight() const;
		void						SetHalfHeight(const float _hh);
	private:
		RTTR_ENABLE(Geometry);
		RTTR_REGISTRATION_FRIEND;
		float						mRadius;
		float						mHalfHeight;
		physx::PxCapsuleGeometry	mGeometry;
	};

	class PlaneGeometry : public Geometry
	{
	public:
		PlaneGeometry() = default;
		PlaneGeometry(float _normalX, float _normalY, float _normalZ, float _distance);

		PlaneGeometry(glm::vec3 _normal, float _distance);

		PlaneGeometry(glm::vec3 _point, glm::vec3 _normal);

		PlaneGeometry(glm::vec3 _point0, glm::vec3 _point1, glm::vec3 _point2);
		~PlaneGeometry() = default;

		glm::vec3					GetNormal() const;
		void						SetNormal(const float _normalX, const float _normalY, const float _normalZ);
		void						SetNormal(glm::vec3 _normal);
		void						SetNormalProxy(const glm::vec3 _normal);

		float						GetDistance() const;
		void						SetDistance(const float _distance);

	private:
		RTTR_ENABLE(Geometry);
		RTTR_REGISTRATION_FRIEND;
		glm::vec3					mNormal;
		float						mDistance;
	};

}
