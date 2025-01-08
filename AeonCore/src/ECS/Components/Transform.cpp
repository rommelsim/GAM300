#include "acpch.h"
/*!***************************************************************************
	\file			Transform.cpp
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
#include "ECS/Components/Transform.h"
#include "Serialization/RegisterManager.hpp"
#include "Resource/Model.h"

namespace AeonCore
{
	Transform::Transform(glm::vec3 const& _position, glm::quat const& _rotation, glm::vec3 const& _scale)
		: mPosition{ _position }, mRotation{ _rotation }, mScale{ _scale }
	{
		updateEulerAngles();
		
		id = Coordinator::GetInstance().GetComponentType<Transform>();

		// how to get the model data into here?
		//boundingVolume = new AABB(generateAABB(Model()));
	}

	void Transform::operator+=(Transform const& _rhs)
	{
		mPosition += _rhs.mPosition;
		meulerAngle += _rhs.meulerAngle;
		//mRotation = glm::quat(meulerAngle);
		mScale *= _rhs.mScale;
	}

	glm::vec3& Transform::GetPosition()
	{
		return mPosition;
	}

	glm::quat& Transform::GetRotation()
	{
		return mRotation;
	}

	glm::vec3& Transform::GetScale()
	{
		return mScale;
	}

	glm::vec3& Transform::GetEulerAngle()
	{
		return meulerAngle;
	}

	glm::mat4& Transform::GetWorldMatrix()
	{
		return mWorldMatrix;
	}

	void Transform::SetPosition(glm::vec3& _pos)
	{
		mPosition = _pos;
		mHasChanged = true;
	}

	void Transform::SetRotation(glm::quat& _rot)
	{
		mRotation = _rot;
		mHasChanged = true;
	}

	void Transform::SetScale(glm::vec3& _scale)
	{
		mScale = _scale;
		mHasChanged = true;
	}

	void Transform::SetEulerAngle(glm::vec3& _eul)
	{
		meulerAngle = _eul;
		mHasChanged = true;
	}

	void Transform::SetWorldMatrix(glm::mat4& _wm)
	{
		mWorldMatrix = _wm;
	}

	glm::mat4 Transform::UpdateMatrix()
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPosition);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), meulerAngle.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), meulerAngle.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), meulerAngle.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), mScale);
		mWorldMatrix = translation * rotation * scale;
		return mWorldMatrix;
	}

	void Transform::PhysicsSyncSetPosition(glm::vec3& _pos)
	{
		mPosition = _pos;
	}

	void Transform::PhysicsSyncSetRotation(glm::quat& _rot)
	{
		mRotation = _rot;
	}

	void Transform::PhysicsSyncSetScale(glm::vec3& _scale)
	{
		mScale = _scale;
	}

	void Transform::PhysicsSyncSetEulerAngle(glm::vec3& _eul)
	{
		meulerAngle = _eul;
	}

	bool Transform::GetHasChangedFlag() const
	{
		return mHasChanged;
	}

	void Transform::SetHasChangedFlag(bool _boolean)
	{
		mHasChanged = _boolean;
	}

	void Transform::UpdateMatrixWithOffset(Transform& parentTransform)
	{

		mPosition = parentTransform.GetPosition() + glm::vec3(0.5f, 0.5f, 0.5f);
	}

	glm::mat4 Transform::GetTransformMatrix() const
	{
		// Construct a transformation matrix based on the entity's position, rotation, and scale
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), mPosition);

		// Create rotation matrices for each axis based on Euler angles
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		rotationMatrix = glm::rotate(rotationMatrix, meulerAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, meulerAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, meulerAngle.z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), mScale);

		// Combine the translation, rotation, and scale matrices to get the final transformation matrix
		glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		return transformMatrix;
	}

	void Transform::updateEulerAngles()
	{
		meulerAngle = glm::eulerAngles(mRotation);
		mHasChanged = true;
	}

	AABB::AABB(const glm::vec3& min, const glm::vec3& max) : BoundingVolume{}, center{ (max + min) * 0.5f }, extents{ max.x - center.x, max.y - center.y, max.z - center.z }
	{
	}

	AABB::AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
		: BoundingVolume{}, center{ inCenter }, extents{ iI, iJ, iK }
	{

	}

	std::array<glm::vec3, 8> AABB::getVertice() const
	{
		std::array<glm::vec3, 8> vertice;
		vertice[0] = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
		vertice[1] = { center.x + extents.x, center.y - extents.y, center.z - extents.z };
		vertice[2] = { center.x - extents.x, center.y + extents.y, center.z - extents.z };
		vertice[3] = { center.x + extents.x, center.y + extents.y, center.z - extents.z };
		vertice[4] = { center.x - extents.x, center.y - extents.y, center.z + extents.z };
		vertice[5] = { center.x + extents.x, center.y - extents.y, center.z + extents.z };
		vertice[6] = { center.x - extents.x, center.y + extents.y, center.z + extents.z };
		vertice[7] = { center.x + extents.x, center.y + extents.y, center.z + extents.z };
		return vertice;
	}

	bool AABB::isOnOrForwardPlane(const Plane& plane) const
	{
		// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
		const float r = extents.x * std::abs(plane.normal.x) + extents.y * std::abs(plane.normal.y) +
			extents.z * std::abs(plane.normal.z);

		return -r <= plane.getSignedDistanceToPlane(center);
	}

	bool AABB::isOnFrustum(const Frustum& camFrustum, const Transform& transform) const
	{
		//Get global scale thanks to our transform
		const glm::vec3 globalCenter{ transform.GetTransformMatrix() * glm::vec4(center, 1.f) };

		// Scaled orientation
		const glm::vec3 right = transform.getRight() * extents.x;
		const glm::vec3 up = transform.getUp() * extents.y;
		const glm::vec3 forward = transform.getForward() * extents.z;

		const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

		const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

		const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

		const AABB globalAABB(globalCenter, newIi, newIj, newIk);

		return (globalAABB.isOnOrForwardPlane(camFrustum.leftFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.rightFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.topFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.bottomFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.nearFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.farFace));
	}

	RTTR_REGISTRATION
	{
		RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<Transform>("Transform")
			.constructor<glm::vec3 const&, glm::quat const&, glm::vec3 const&>()() //constructor with 3 args
			.property_readonly("id", &Transform::GetComponentID)
			.property("position", &Transform::GetPosition, &Transform::SetPosition)
			.property("rotation", &Transform::GetRotation, &Transform::SetRotation)
			.property("scale", &Transform::GetScale, &Transform::SetScale)
			.property("euler", &Transform::GetEulerAngle, &Transform::SetEulerAngle)
			.property("On rotation", &Transform::GetOnRotation, &Transform::SetOnRotation)
			/*.property("world matrix", &Transform::GetWorldMatrix, &Transform::SetWorldMatrix)*/; }, 1);
	}



	Plane::Plane(const glm::vec3& p1, const glm::vec3& norm): normal((norm)),distance(glm::dot(normal, p1))
	{

	}

	float Plane::getSignedDistanceToPlane(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
	}

	bool BoundingVolume::isOnFrustum(const Frustum& camFrustum) const
	{
		return (isOnOrForwardPlane(camFrustum.leftFace) &&
			isOnOrForwardPlane(camFrustum.rightFace) &&
			isOnOrForwardPlane(camFrustum.topFace) &&
			isOnOrForwardPlane(camFrustum.bottomFace) &&
			isOnOrForwardPlane(camFrustum.nearFace) &&
			isOnOrForwardPlane(camFrustum.farFace));
	}

	AABB generateAABB(Model* model)
	{
		glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
		for (auto&& mesh : model->GetMeshes())
		{
			for (auto&& vertex : mesh.GetVertices())
			{
				minAABB.x = std::min(minAABB.x, vertex.Position.x);
				minAABB.y = std::min(minAABB.y, vertex.Position.y);
				minAABB.z = std::min(minAABB.z, vertex.Position.z);

				maxAABB.x = std::max(maxAABB.x, vertex.Position.x);
				maxAABB.y = std::max(maxAABB.y, vertex.Position.y);
				maxAABB.z = std::max(maxAABB.z, vertex.Position.z);
			}
		}
		return AABB(minAABB, maxAABB);
	}
}