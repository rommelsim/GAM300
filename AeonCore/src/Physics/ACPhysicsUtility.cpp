#include "acpch.h"
/*!***************************************************************************
    \file			ACPhysicsUtility.cpp
    \author			Chua Yip Xuan & Felicia Ng Yuwen
    \par			yipxuan.chua\@digipen.edu & yuwenfelicia.ng\@digipen.edu
    \date			Oct 1, 2023
    \brief			This header file consists of the P
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Physics/ACPhysicsUtility.hpp"
#include <glm/gtx/quaternion.hpp>

namespace AeonCore::ACPhysicsUtility
{
    physx::PxTransform ToPhysXTransform( /*const*/ Transform& _transform)
    {
        physx::PxQuat q;
        if(_transform.GetOnRotation())
            q = ToPhysXQuat(glm::normalize(glm::quat(_transform.GetEulerAngle()))); //  rotation on
        else
            q = ToPhysXQuat(glm::normalize(glm::quat(_transform.GetRotation())));   //  rotation off

        physx::PxVec3 p = ToPhysXVector(_transform.GetPosition());
        return physx::PxTransform(p, q);
    }

    physx::PxTransform ToPhysXTransform(const glm::mat4& _transform)
    {
        physx::PxQuat r = ToPhysXQuat(glm::normalize(glm::toQuat(_transform)));
        physx::PxVec3 p = ToPhysXVector(glm::vec3(_transform[3]));
        return physx::PxTransform(p, r);
    }

    physx::PxTransform ToPhysXTransform(const glm::vec3& _translation, const glm::vec3& _rotation)
    {
        return physx::PxTransform(ToPhysXVector(_translation), ToPhysXQuat(glm::quat(_rotation)));
    }

    physx::PxTransform ToPhysXTransform(const glm::vec3 &_translation, const glm::quat &_rotation)
    {
        return physx::PxTransform(ToPhysXVector(_translation), ToPhysXQuat(_rotation));
    }

    physx::PxMat44 ToPhysXMatrix(const glm::mat4& _matrix)
    {
        /*return *(physx::PxMat44*)&_matrix;*/

        physx::PxMat44 result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result[i][j] = _matrix[i][j];
            }
        }

        return result;
    }

    physx::PxVec3 ToPhysXVector(const glm::vec3& _vector)
    {
        //return *(physx::PxVec3*)&_vector;
        return physx::PxVec3(_vector.x, _vector.y, _vector.z);
    }

    physx::PxVec4 ToPhysXVector(const glm::vec4& _vector)
    {
        //return *(physx::PxVec4*)&_vector;
        return physx::PxVec4(_vector.x, _vector.y, _vector.z, _vector.w);
    }

    physx::PxQuat ToPhysXQuat(const glm::quat& _quat)
    {
        return physx::PxQuat(_quat.x, _quat.y, _quat.z, _quat.w);
    }

    glm::mat4 FromPhysXTransform(const physx::PxTransform& _transform)
    {
        glm::quat _rotation = FromPhysXQuat(_transform.q);
        glm::vec3 position = FromPhysXVector(_transform.p);
        return glm::translate(glm::mat4(1.0F), position) * glm::toMat4(_rotation);
    }

    glm::mat4 FromPhysXMatrix(const physx::PxMat44& _matrix)
    {
        // *(glm::mat4*)&_matrix;

        glm::mat4 result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result[i][j] = _matrix[i][j];
            }
        }

        return result;
    }

    glm::vec3 FromPhysXVector(const physx::PxVec3& _vector)
    {
        //return *(glm::vec3*)&_vector;
        return glm::vec3(_vector.x, _vector.y, _vector.z);
    }

    glm::vec4 FromPhysXVector(const physx::PxVec4& _vector)
    {
        //return *(glm::vec4*)&_vector;
        return glm::vec4(_vector.x, _vector.y, _vector.z, _vector.w);
    }

    glm::quat FromPhysXQuat(const physx::PxQuat& _quat)
    {
        //return *(glm::quat*)&_quat;
        return glm::quat(_quat.w, _quat.x, _quat.y, _quat.z);
    }

}
