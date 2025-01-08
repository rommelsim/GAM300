#pragma once
/*!***************************************************************************
    \file			ACPhysicsUtility.hpp
    \author			Chua Yip Xuan & Felicia Ng Yuwen
    \par			yipxuan.chua\@digipen.edu & yuwenfelicia.ng\@digipen.edu
    \date			Oct 1, 2023
    \brief			This header file consists of the P
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <PxPhysicsAPI.h>
#include <glm/gtc/type_ptr.hpp>

#include "ECS/Components/Transform.h"

namespace AeonCore::ACPhysicsUtility
{
    physx::PxTransform ToPhysXTransform(/*const*/ Transform& _transform);
    physx::PxTransform ToPhysXTransform(const glm::mat4& _transform);
    physx::PxTransform ToPhysXTransform(const glm::vec3& _translation, const glm::vec3& _rotation);
    physx::PxTransform ToPhysXTransform(const glm::vec3 &_translation, const glm::quat &_rotation);
    physx::PxMat44 ToPhysXMatrix(const glm::mat4& _matrix);
    physx::PxVec3 ToPhysXVector(const glm::vec3& _vector);
    physx::PxVec4 ToPhysXVector(const glm::vec4& _vector);
    physx::PxQuat ToPhysXQuat(const glm::quat& _quat);

    glm::mat4 FromPhysXTransform(const physx::PxTransform& _transform);
    glm::mat4 FromPhysXMatrix(const physx::PxMat44& _matrix);
    glm::vec3 AEON_API FromPhysXVector(const physx::PxVec3& _vector);
    glm::vec4 AEON_API FromPhysXVector(const physx::PxVec4& _vector);
    glm::quat FromPhysXQuat(const physx::PxQuat& _quat);
}
