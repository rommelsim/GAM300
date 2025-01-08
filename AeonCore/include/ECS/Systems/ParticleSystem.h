#pragma once
/*!***************************************************************************
	\file			ParticleSystem.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage Paritcle
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/Log.h"
#include "Core/Singleton.hpp"
#include "ECS/Components/Particle.h"
#include "ECS/Architecture/System.hpp"

#include <ECS/Components/IComponent.h>
#include <ECS/Architecture/ECS.hpp>

#include <glm/glm.hpp>
#include <rttr/registration.h>

namespace AeonCore
{
	class ParticleSystem : public System
	{
	public:
		void Init() override;
		void Update() override;
		void Destroy() override;

	private:
	};
}