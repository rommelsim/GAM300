#pragma once
/*!***************************************************************************
    \file			RigidBody.h
    \author			Ng Yuwen Felicia & Chua Yip Xuan 
    \par			yuwenfelicia.ng\@digipen.edu & yipxuan.chua\@digipen.edu 
    \date			Oct 1, 2023
    \brief			This header file consists of the Rigidbody component and
					its functions
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <PxQueryReport.h>
#include <PxRigidActor.h>
#include <glm/glm.hpp>

#include "IComponent.h"
#include "Transform.h"
#include <rttr/registration_friend.h>

namespace AeonCore
{
    class AEON_API RigidBody : public IComponent
    {
    public:
        enum class RigidBodyType { STATIC, DYNAMIC };

        struct RaycastResult
        {
            EntityID id;
            glm::vec3 hitPosition;  
            glm::vec3 hitNormal;   
            float distance;         
        };

        //default 
        RigidBody();

        RigidBody(float _mass, float _linearDrag, float _angularDrag, bool _useGravity, bool _isAutoCenterOfMass, bool _isAutoTensor, bool _isKinematic, RigidBodyType _type);

        ~RigidBody() override;

        void Destroy() override;

        float               GetMass() const;//
        float               GetLinearDrag() const;//
        float               GetAngularDrag() const;//
        bool                GetUseGravity() const; //
        bool                GetIsAutoCenterOfMass() const;//
        glm::vec3           GetCenterOfMass() const;//
        bool                GetIsAutoTensor() const;//
        glm::vec3           GetInertiaTensor() const;//
        glm::vec3           GetInertiaTensorRotation() const;//
        bool                GetIsKinematic() const;
        RigidBodyType       GetRigidBodyType() const; //
        physx::PxRigidActor *GetPhysxActor();
        bool                GetHasChanged() const;
        glm::vec3           GetVelocity();

        void                SetMass(const float _mass);
        void                SetLinearDrag(const float _ld);
        void                SetAngularDrag(const float _ad);
        void                SetUseGravity(const bool _useGravity);
        void                SetIsAutoCenterOfMass(const bool _isAutoCenterOfMass);
        void                SetCenterOfMass(const glm::vec3 _centerOfMass);
        void                SetIsAutoTensor(const bool _isAutoTensor);
        void                SetInertiaTensor(const glm::vec3 _inertiaTensor);
        void                SetInertiaTensorRotation(const glm::vec3 _inertiaTensorRot);
        void                SetIsKinematic(const bool _isKinematic);
        void                SetRigidBodyType(const RigidBodyType _bodyType);
        void                SetHasChanged(const bool _value);
        void                SetPhysxActor(physx::PxRigidActor *_actor);
        void                SetVelocity(glm::vec3 _vel);

        void                AddForce(glm::vec3 _force);
        void                AddImpulse(glm::vec3 _force);
        bool                Raycast(const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance, RaycastResult &result);
    private:
        RTTR_ENABLE(IComponent);

        float                   mMass;                  //serialize
        float                   mLinearDrag;            //serialize
        float                   mAngularDrag;           //serialize
        bool                    mUseGravity;            //serialize
        bool                    mIsAutoCenterOfMass;    //serialize
        glm::vec3               mCenterOfMass;          //serialize
        bool                    mIsAutoTensor;          //serialize
        glm::vec3               mInertiaTensor;         //serialize
        glm::vec3               mInertiaTensorRotation; //serialize
        bool                    mIsKinematic;           //serialize
        RigidBodyType           mBodyType;              //serialize
        bool					mHasChanged = false;    //dont need serialize

        physx::PxRigidActor     *mPhysxActor =nullptr;  //dont need serialize
    };
}