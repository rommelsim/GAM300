#pragma once
/*!***************************************************************************
    \file			SceneManager.h
    \author			Lam En Qing
    \par			enqing.lam\@digipen.edu
    \brief			This header file consists of a SceneManager class that manages
					the scene for the game.
    Copyright (C) 2023 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents without the
    prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include <string>

#include "AeonAPI.h"
#include "Scene.h"

namespace AeonCore {
	using SceneRef = std::shared_ptr<Scene>;

	class AEON_API SceneManager {
	public:
		SceneManager();

		static SceneManager& GetInstance();

		/// <summary>
		/// Explicitly creates the SceneManager singleton instance
		/// </summary>
		static void Init();
		/// <summary>
		/// Destroys the SceneManager singleton instance
		/// </summary>
		static void Destroy();

		SceneRef GetActiveScene() const;
		SceneRef GetScene(SceneID sceneId) const;
		SceneRef GetSceneFromEntity(EntityID entityId) const;
		Scene* GetRawSceneFromEntity(EntityID entityId) const;
		bool IsSceneCached(SceneID sceneId) const;

		/// <summary>
		/// Clear the loaded scene cache, used only in Editor application, to clear the scene cache after stopping runtime
		/// </summary>
		void ClearSceneCache();

		/// <summary>
		/// Get the scene that is being deserialized, either on main thread or a worker thread
		/// </summary>
		/// <returns>SceneRef, a shared_ptr to a Scene</returns>
		SceneRef GetLoadThreadScene() const;
		
		/// <summary>
		/// Create a new Scene pointer with an ID extracted from a pool. It can return a nullptr if the scene IDs were exhausted
		/// </summary>
		static SceneRef CreateNewScene(SceneType sceneType, unsigned maxEntities);
		static void SetActiveScene(SceneRef scene);
		static void SetLoadingScreenScene(const SceneRef& scene);

		static void RemoveSceneFromCache(const std::string& sceneFilepath);

		/// <summary>
		/// Load a scene provided a filepath and makes the loaded scene the active scene.
		/// </summary>
		/// <param name="sceneFilepath">You simply need to provide the filepath that is relative to the "./resources/scene" folder. That is if a scene file is inside the aforementioned folder, you simply need to provide the filename including file extension.</param>
		/// <param name="makeActiveScene">By default, this parameter is true. This makes the loaded scene the current active scene. You can choose to only load the scene and not make it the active scene by passing false</param>
		/// <param name="cacheScene">By default, this parameter is false. If you wish to save the loaded scene pointer to a map for later retrieval, pass in true</param>
		/// <returns>A pointer to the loaded scene, useful only if you don't let the function set said scene to be the active scene.</returns>
		static SceneRef LoadScene(const std::string& sceneFilepath, bool makeActiveScene = true, bool cacheScene = false);
		static void LoadSceneThread(const std::string& sceneFilepath, bool shouldCache);

		static void SaveScene(const std::string& sceneFilepath);
	private:
		friend class Serialization; // SetLoadThreadScene
		friend class Scene;

		SceneRef mActiveScene;
		SceneRef mOldActiveScene;
		SceneRef mLoadThreadScene; // Scene that is being deserialized, could be on the thread or on main thread
		SceneRef mLoadingScreenScene; // Scene pointer dedicated to hold a reference to the Loading Screen
		std::unordered_map<std::string, SceneRef> mLoadedSceneMap; // key is the scene filename (including .scene)

		std::queue<SceneID> mIdPool;

		static void SanitiseFilepath(const std::string& inFilepath, std::string* outFilename = nullptr, std::filesystem::path* outFsFilepath = nullptr);

		void SetLoadThreadScene(const SceneRef& scene);
	};
}