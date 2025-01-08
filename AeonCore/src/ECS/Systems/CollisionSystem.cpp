#include "acpch.h"
/*!***************************************************************************
	\file			CollisionSystem.cpp
	\author			Leong Wai Kit
	\par			l.waikit\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions of the
					CollisionSystem class. The CollisionSystem is responsible
					for detecting collisions between entities in the system.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Systems/CollisionSystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Collider.h"
#include "ECS/Architecture/Coordinator.hpp"

#include "Core/Log.h"
#include "Rendering/GeomLib.h"

namespace AeonCore
{
	void CollisionSystem::Init()
	{
	}

	void CollisionSystem::Update()
	{
#if 0
		ZoneScopedN("Collision Update");
		Coordinator& coord = Coordinator::GetInstance();

		for (auto const& entity1 : mEntities)
		{
			Transform tran_e1 = coord.GetComponent<Transform>(entity1);
			Collider coll_e1 = coord.GetComponent<Collider>(entity1);
			coll_e1.updateAABB(tran_e1.GetPosition());
			
			for (auto const& entity2 : mEntities)
			{
				if (entity1 == entity2)
					continue;

				Transform tran_e2 = coord.GetComponent<Transform>(entity2);
				Collider coll_e2 = coord.GetComponent<Collider>(entity2);
				coll_e2.updateAABB(tran_e2.GetPosition());

				if (Intersects(coll_e1.aabb, coll_e2.aabb))
				{
					//AeonCore_INFO("Collider between 2 boxes...");
				}

				//AeonCore_INFO("{0} {1} {2} {3}", coord.GetEntity(entity2)->GetEntityName(), coll.aabb.center.x, coll.aabb.center.y, coll.aabb.center.z);

			}
		}
#endif
	}

	void CollisionSystem::Destroy()
	{
	}
}
