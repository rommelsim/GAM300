#pragma once
/*!***************************************************************************
    \file			Scene.h
    \author			Lam En Qing
    \par			enqing.lam\@digipen.edu
    \brief			This header file consists of a Scene class that defines
					a scene in the engine.
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "Core/Log.h"
#include "ECS/Architecture/ECS.hpp"
#include "ECS/Architecture/Entity.hpp"

#include <rttr/registration_friend.h>
#include <queue>
#include <unordered_map>
#include <set>
#include <vector>
#include <glm/vec3.hpp>

namespace AeonCore {
	class IComponentArray;

	enum class SceneType {
		// If the scene will spawn entities via scripts, then the max entities will be MAX_ENTITIES
		WillSpawn,
		// If the scene will not spawn entities via scripts, then the max entities will be whatever the scene has
		WillNotSpawn,
		MAX
	};

	class AEON_API Scene {
	public:
		/// <summary>
		/// Default assigns Scene's type of Game and maxEntities to the defined MAX_ENTITIES limit in ECS.h
		/// </summary>
		Scene(SceneID sceneId, SceneType sceneType, unsigned int maxEntities);
		Scene(const Scene& sceneCopy) = delete;
		Scene(Scene&& sceneMoved) = delete;
		~Scene();

		Scene& operator=(const Scene& sceneCopy) = delete;

		SceneID GetSceneID();

		void SetSceneType(SceneType sceneType);
		SceneType GetSceneType();

		/// <summary>
		/// Returns whether the scene is empty: does the scene have any entities inside?
		/// </summary>
		/// <returns>true if no entities, false otherwise</returns>
		bool Empty();

		/// <summary>
		/// Clear a scene, will destroy all entities
		/// </summary>
		void Clear();

		static const std::vector<std::string>& GetTypeNames();

		unsigned int GetMaxEntityCount();
		unsigned int GetLivingCount();

		Entity* GetEntity(EntityID id);
		std::vector<Entity>& GetEntities();
		bool IsEntityNameRepeated(const std::string& name);
		bool EntityExists(EntityID id);

		Entity& CreateEntity(std::string name, EntityID parent = INVALID_ENTITY_ID);
		Entity& CreateChild(EntityID parent, const std::string& name = "");
		void DestroyEntity(EntityID entityId);

		void SetSignature(EntityID _entity, const Signature& _signature);
		Signature GetSignature(EntityID _entity) const;

		void ToChild(Entity& parent, Entity& child);
		void RemoveParent(Entity& child);
	private:
		RTTR_REGISTRATION_FRIEND;
		friend class SceneManager;
		friend class ComponentManager; // Used in GetComponentArray() to access mEntityComponents
		friend class Coordinator;      // ToChild(), RemoveParent(), mParentChild, mDeleteEntitiesRequest
		friend class Serialization;	   // CreateEntity()

		static std::vector<std::string> s_TypeNames;

		SceneID mSceneID;
		SceneType mSceneType;
		unsigned int mMaxEntities;

		glm::vec3 mAmbientLight;

		// Unfortunately, must move scene-unique variables from ComponentManager, EntityManager & Coordinator in here, which is all the variables that come after this comment
		std::queue<EntityID>   mAvailableEntities{}; // Entity ID pool
		std::vector<Entity>    mEntities{}; // Living Entities
		std::vector<Signature> mSignatures{};
		std::unordered_map<EntityID, std::vector<EntityID>> mParentChild{};
		std::set<EntityID> mDeleteEntitiesRequest{};

		std::unordered_map<std::string, std::shared_ptr<IComponentArray>> mEntityComponents; // entry for component type is added in ComponentManager's GetComponentArray()

		void SetMaxEntities(unsigned int maxEntities = MAX_ENTITIES);
		void SetAmbientLight(glm::vec3 light);
	};
}
