#pragma once
/*!***************************************************************************
	\file			ACPhysicsMaterial.hpp
	\author			Chua Yip Xuan & Felicia Ng Yuwen
	\par			yipxuan.chua\@digipen.edu & yuwenfelicia.ng\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the P
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <PxMaterial.h>

#include "Core/AeonAPI.h"
#include "rttr/registration.h"
#include "rttr/registration_friend.h"

namespace AeonCore::ACPhysics
{
	class AEON_API ACPhysicsMaterial
	{
	public:
		ACPhysicsMaterial(float _staticFriction =0.6f, float _dynamicFriction = 0.6f, float _restitution = 0.0f);
		~ACPhysicsMaterial() = default;

		float GetStaticFriction() const;
		float GetDynamicFriction() const;
		float GetRestitution() const;

		void SetStaticFriction(const float _sf);
		void SetDynamicFriction(const float _df);
		void SetRestitution(const float _res);
	private:
		RTTR_REGISTRATION_FRIEND;
		float mStaticFriction;
		float mDynamicFriction;
		float mRestitution;
	};
}
