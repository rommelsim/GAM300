#pragma once
/*!***************************************************************************
	\file			Transform.h
	\author			Leong Wai Kit, Rommel Sim 
	\par			l.waikit\@digipen.edu, zhenpengrommel.sim\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declarations for the
					Transform class, which represents the transformation
					properties of an entity.
	
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Components/Camera.h"
#include "IComponent.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Core/Log.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "Core/AeonAPI.h"


namespace AeonCore
{
	// Forward declaration
	class Model;
	class Transform;

	struct AEON_API Plane
	{
	public:
		glm::vec3 normal = { 0.f, 1.f, 0.f }; // unit vector
		float     distance = 0.f;        // Distance with origin

		Plane() = default;

		Plane(const glm::vec3& p1, const glm::vec3& norm);

		float getSignedDistanceToPlane(const glm::vec3& point) const;
	};

	struct AEON_API Frustum
	{
		Plane topFace;
		Plane bottomFace;

		Plane rightFace;
		Plane leftFace;

		Plane farFace;
		Plane nearFace;
	};

	struct BoundingVolume
	{
		virtual bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const = 0;

		virtual bool isOnOrForwardPlane(const Plane& plane) const = 0;

		bool isOnFrustum(const Frustum& camFrustum) const;
	};

	struct AABB : public BoundingVolume
	{
		glm::vec3 center{ 0.f, 0.f, 0.f };
		glm::vec3 extents{ 0.f, 0.f, 0.f };

		AABB() = default;

		AABB(const glm::vec3& min, const glm::vec3& max);

		AABB(const glm::vec3& inCenter, float iI, float iJ, float iK);

		std::array<glm::vec3, 8> getVertice() const;

		//see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
		bool isOnOrForwardPlane(const Plane& plane) const final;

		bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final;
	};

	class AEON_API Transform : public IComponent
	{
	public:
		
		Transform(glm::vec3 const &_position = { 0.f, 0.f, 0.f },
				  glm::quat const &_rotation = { 1.f, 0.f, 0.f, 0.f },
				  glm::vec3 const &_scale = { 1.f, 1.f, 1.f });

		void operator+=(Transform const &_rhs);

		glm::vec3& GetPosition();
		glm::quat& GetRotation();
		glm::vec3& GetScale();
		glm::vec3& GetEulerAngle();
		glm::mat4& GetWorldMatrix();
		glm::mat4 GetTransformMatrix() const;

		inline bool GetOnRotation() { return mOnRotation; }

		void SetPosition(glm::vec3& _pos);
		void SetRotation(glm::quat& _rot);
		void SetScale(glm::vec3& _scale);
		void SetEulerAngle(glm::vec3& _eul);
		void SetWorldMatrix (glm::mat4& _wm);
		glm::mat4 UpdateMatrix();
		glm::mat4 getLocalModelMatrix()
		{
			const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(this->meulerAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));
			const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(this->meulerAngle.y), glm::vec3(0.0f, 1.0f, 0.0f));
			const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(this->meulerAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));

			// Y * X * Z
			const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

			// translation * rotation * scale (also know as TRS matrix)
			return glm::translate(glm::mat4(1.0f), this->mPosition) * rotationMatrix * glm::scale(glm::mat4(1.0f), this->mScale);
		}
		glm::vec3 getRight() const
		{
			return this->mWorldMatrix[0];
		}

		glm::vec3 getUp() const
		{
			return this->mWorldMatrix[1];
		}

		glm::vec3 getBackward() const
		{
			return this->mWorldMatrix[2];
		}

		glm::vec3 getForward() const
		{
			return -this->mWorldMatrix[2];
		}
		inline void SetOnRotation(bool _onRotate) { mOnRotation = _onRotate; }


		//for physics, do not delete
		void PhysicsSyncSetPosition(glm::vec3 &_pos);
		void PhysicsSyncSetRotation(glm::quat &_rot);
		void PhysicsSyncSetScale(glm::vec3 &_scale);
		void PhysicsSyncSetEulerAngle(glm::vec3 &_eul);
		bool GetHasChangedFlag() const;
		void SetHasChangedFlag(bool _boolean);

		void UpdateMatrixWithOffset(Transform& parentTransform);



	private:
		RTTR_ENABLE(IComponent);
		void updateEulerAngles();
		glm::vec3 mPosition;
		glm::quat mRotation;
		glm::vec3 mScale{ 1.0f,1.0f,1.0f };
		glm::vec3 meulerAngle;
		glm::mat4 mWorldMatrix;
		bool mHasChanged = false;

	public:
		AABB boundingVolume;
		bool mOnRotation = true;
	};


	AABB generateAABB(Model* model);
}
