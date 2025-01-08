#pragma once
/*!***************************************************************************
	\file			IComponent.h
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\brief			This header file consists of the base class for the 
					Components
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <rttr/registration>
#include <rttr/type>

#include "ECS/Architecture/ECS.hpp"

namespace AeonCore
{
	class IComponent
	{
	public:
		IComponent()					= default;
		virtual ~IComponent()			= default;
		virtual void Destroy() {}
		
		ComponentType GetComponentID() const	{ return id; }

		bool GetIsComponentActive() const		{ return isActive; }

		RTTR_ENABLE();
	protected:
		ComponentType id{};
		bool isActive = true;
	};
}