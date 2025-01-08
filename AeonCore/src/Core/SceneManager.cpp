#include "acpch.h"
/*!***************************************************************************
	\file			SceneManager.cpp
	\author			Lam En Qing
	\par			enqing.lam\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file defines the SceneManager class 

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/SceneManager.h"

#include "Core/Application.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "ECS/Systems/AudioSystem.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/PhysicsSystem.h"
#include "ECS/Systems/Renderer2DSystem.h"
#include "Scripting/Scripting.h"
#include "Serialization/Serialization.hpp"

namespace AeonCore {
	SceneManager* g_Instance = nullptr;
	std::string s_Scene_Path_String("resources/scene/"); // evaluates to "resources/scene/"
	std::filesystem::path s_SCENE_PATH = "./" + s_Scene_Path_String;

	SceneManager::SceneManager() {
		// Dunno why but cannot do up to 255, but we stop at 254
		for (SceneID _scene = 0; _scene <= MAX_SCENES; _scene++) {
			mIdPool.push(_scene);
		}
		AeonCore_ERROR("Max of SceneID type {}", std::numeric_limits<SceneID>::max());
		SceneID value = 255;
		AeonCore_ERROR("SceneID check {} {}", value == std::numeric_limits<SceneID>::max(), value == MAX_SCENES);
	}

	SceneManager& SceneManager::GetInstance() {
		if (g_Instance == nullptr)
			g_Instance = new SceneManager();

		return *g_Instance;
	}

	void SceneManager::Init() {
		GetInstance();
	}

	void SceneManager::Destroy() {
		if (g_Instance == nullptr) return;

		g_Instance->mLoadedSceneMap.clear();
		g_Instance->mActiveScene.reset();
		g_Instance->mLoadThreadScene.reset();
		g_Instance->mLoadingScreenScene.reset();

		delete g_Instance;
	}

	std::shared_ptr<Scene> SceneManager::GetActiveScene() const {
		return mActiveScene;
	}

	SceneRef SceneManager::GetScene(SceneID sceneId) const {
		if (mActiveScene && mActiveScene->GetSceneID() == sceneId)
			return mActiveScene;

		if (mOldActiveScene && mOldActiveScene->GetSceneID() == sceneId)
			return mOldActiveScene;

		if (mLoadThreadScene && mLoadThreadScene->GetSceneID() == sceneId)
			return mLoadThreadScene;

		if (mLoadingScreenScene && mLoadingScreenScene->GetSceneID() == sceneId)
			return mLoadingScreenScene;

		for (auto& [_, sceneRef] : mLoadedSceneMap) {
			if (sceneRef->GetSceneID() == sceneId) {
				return sceneRef;
			}
		}
		return nullptr;
	}

	SceneRef SceneManager::GetSceneFromEntity(EntityID entityId) const {
		return GetScene(ECSUtils::ExtractSceneID(entityId));
	}

	Scene* SceneManager::GetRawSceneFromEntity(EntityID entityId) const
	{
		SceneRef scenePtr = GetScene(ECSUtils::ExtractSceneID(entityId));
		if (scenePtr)
			return scenePtr.get();
		return nullptr;
	}

	bool SceneManager::IsSceneCached(SceneID sceneId) const {
		for (auto&[_, sceneRef] : mLoadedSceneMap) {
			if (sceneRef->GetSceneID() == sceneId)
				return true;
		}
		return false;
	}

	void SceneManager::ClearSceneCache() {
		mLoadedSceneMap.clear();
		mLoadThreadScene.reset();
	}

	SceneRef SceneManager::GetLoadThreadScene() const {
		return mLoadThreadScene;
	}

	void SceneManager::SetLoadThreadScene(const SceneRef& scene) {
		mLoadThreadScene = scene;
	}

	std::shared_ptr<Scene> SceneManager::CreateNewScene(SceneType sceneType, unsigned maxEntities) {
		if (g_Instance->mIdPool.empty()) {
			AeonCore_ASSERT(false, "Ran out of scene IDs")

			return nullptr;
		}

		SceneID sceneId = g_Instance->mIdPool.front();
		g_Instance->mIdPool.pop();
		return std::make_shared<Scene>(sceneId, sceneType, maxEntities);
	}

	void SceneManager::SetActiveScene(std::shared_ptr<Scene> scene) {
		if (scene == nullptr) return;

		/*if (g_Instance->GetActiveScene() == scene) {
			AeonCore_ERROR("Scene you want to set as active is already active!");
			return;
		}*/
		Coordinator &coord = Coordinator::GetInstance();
		auto audioSystem = std::dynamic_pointer_cast<AudioSystem>(coord.GetSystem<AudioSystem>());
		auto physicsSystem = std::dynamic_pointer_cast<PhysicsSystem>(coord.GetSystem<PhysicsSystem>());

		// Before the old active scene goes away, we must check if the scene is cached. If it is, we call OnCachedSceneInactive!
		if (g_Instance->GetActiveScene() && g_Instance->IsSceneCached(g_Instance->GetActiveScene()->GetSceneID())) {
			audioSystem->OnScenedSceneInactive();
			Scripting::OnCachedSceneInactive(g_Instance->GetActiveScene()->GetSceneID());
		}


		// Since it's setting active scene, need to inform systems

		// LightingManager is a system, and it keeps track of EntityID to Lighting component pointer
		// CameraManager keeps track of EntityID to Camera component pointer, the record is removed only on Camera component destroy
		std::dynamic_pointer_cast<Renderer2DSystem>(coord.GetSystem<Renderer2DSystem>())->Destroy();
		std::dynamic_pointer_cast<LightManager>(coord.GetSystem<LightManager>())->Destroy();
		std::dynamic_pointer_cast<CameraSystem>(coord.GetSystem<CameraSystem>())->Destroy();
		
		// we use OldActiveScene to allow the Collider & RigidBody representatives in PhysX to be destroyed!
		// Cannot rely on Scene destructor because the SceneRef no longer exists when entering the destructor
		g_Instance->mOldActiveScene = g_Instance->mActiveScene;
		g_Instance->mActiveScene = scene;
		if (g_Instance->mOldActiveScene && !g_Instance->IsSceneCached(g_Instance->mOldActiveScene->GetSceneID()))
		{
			//AeonCore_INFO("break here {}", g_Instance->mOldActiveScene.use_count());

			physicsSystem->Destroy();
		}
			/*delete g_Instance->mOldActiveScene.get();*/
		g_Instance->mOldActiveScene = nullptr;
		//AeonCore_ERROR("After activeScene reassignment");
		coord.RefreshSystems();

		physicsSystem->AfterSceneLoaded();
		LightManager::GetInstance().SetAmbientLight(scene->mAmbientLight);

		if (g_Instance->mLoadThreadScene != nullptr)
			g_Instance->mLoadThreadScene.reset();

		if (g_Instance->IsSceneCached(scene->GetSceneID())) {
			audioSystem->OnCachedSceneActive();
			Scripting::OnCachedSceneActive(scene->GetSceneID());
		}
	}

	void SceneManager::SetLoadingScreenScene(const SceneRef& scene) {
		g_Instance->mLoadingScreenScene = scene;
	}

	void SceneManager::RemoveSceneFromCache(const std::string& sceneFilepath) {
		std::string sanitised_sceneFilepath;
		SanitiseFilepath(sceneFilepath, &sanitised_sceneFilepath);
		
		auto sceneFindResult = g_Instance->mLoadedSceneMap.find(sanitised_sceneFilepath);

		g_Instance->mLoadedSceneMap.erase(sanitised_sceneFilepath);
	}

	std::shared_ptr<Scene> SceneManager::LoadScene(const std::string& sceneFilepath, bool makeActiveScene, bool cacheScene) {
		if (g_Instance->mLoadThreadScene != nullptr) {
			AeonCore_ASSERT(false, "Cannot load another scene while one is still loading!");
			return nullptr;
		}

		AeonCore_WARN("LoadScene: {}", sceneFilepath);

		std::string sanitised_sceneFilepath;
		std::filesystem::path fs_path;
		SanitiseFilepath(sceneFilepath, &sanitised_sceneFilepath, &fs_path);

		auto sceneFindResult = g_Instance->mLoadedSceneMap.find(sanitised_sceneFilepath);

		// if scene was not cached
		if (sceneFindResult == g_Instance->mLoadedSceneMap.end()) {
			std::shared_ptr<Scene> deserializedScene = Serialization::GetInstance().DeserializeScene(fs_path.string());

			if (deserializedScene == nullptr) {
				AeonCore_ERROR("SceneManager::Load - Failed to deserialize scene");
				return nullptr;
			}

			// if want to make the loaded scene active AND the loaded scene pointer is not same as deserialized scene
			if (makeActiveScene) {
				SetActiveScene(deserializedScene);
			}
			g_Instance->mLoadThreadScene.reset(); // regardless of makeActiveScene boolean flag, we must release the mLoadThreadScene

			if (cacheScene) {
				AeonCore_INFO("Caching scene, {}", sanitised_sceneFilepath);
				g_Instance->mLoadedSceneMap.emplace(sanitised_sceneFilepath, deserializedScene);
			}

			return deserializedScene;
		}

		AeonCore_INFO("A cached scene, {}, was retrieved for loading!", sanitised_sceneFilepath);

		if (makeActiveScene) {
			SetActiveScene(sceneFindResult->second);
		}
		return sceneFindResult->second;
	}

	void SceneManager::LoadSceneThread(const std::string& sceneFilepath, bool shouldCache) {
		// This function should only be called twice: First to create a thread, the Second to run the thread's logic which is the else block
		Application& app = Application::GetApplication();

		if (g_Instance->mLoadThreadScene != nullptr) {
			AeonCore_ERROR("Cannot load another scene while one is still loading!");
			return;
		}

		if (app.GetLoadStatus() == LoadStatus::NotRunning) {
			AeonCore_ERROR("Start Load Scene Thread");

			app.mLoadStatus = LoadStatus::ReadScene;
			SetActiveScene(g_Instance->mLoadingScreenScene);
			app.mSceneLoadThread = std::thread(LoadSceneThread, sceneFilepath, shouldCache);
		} else {
			Scripting::AttachMonoToThread();
			while (true) {
				if (app.GetLoadStatus() == LoadStatus::ReadScene) {
					AeonCore_ERROR("Thread: Before Load Scene");
					g_Instance->mLoadThreadScene = LoadScene(sceneFilepath, false, shouldCache); // Load the scene, read the scene file
					app.mLoadStatus = LoadStatus::LoadResources; // After the scene file is processed, move onto loading resources

					bool hasRead = true;
					void* params[1];
					params[0] = &hasRead;
					Scripting::InvokeEventFunction("SetHasSceneRead", params);

					int requestCount = static_cast<int>(app.mThreadLoadResourceRequests.size());
					params[0] = &requestCount;
					Scripting::InvokeEventFunction("SetLoadResourceRequestCount", params);
					AeonCore_ERROR("Thread: After Load Scene");
				}
				else if (app.GetLoadStatus() == LoadStatus::LoadResources) {
					if (app.mThreadLoadResourceRequests.empty()) {
						Scripting::DetachMonoFromThread();
						app.mLoadStatus = LoadStatus::Finish;
						break;
					}
				}


			}
		}
	}

	void SceneManager::SaveScene(const std::string& sceneFilepath) {
		std::filesystem::path fs_path;
		SanitiseFilepath(sceneFilepath, nullptr, &fs_path);
		Serialization::GetInstance().SerializeScene(fs_path.string());
	}

	void SceneManager::SanitiseFilepath(const std::string& inFilepath, std::string* outFilename, std::filesystem::path* outFsFilepath) {
		std::string sanitised_sceneFilepath(inFilepath);
		std::replace(sanitised_sceneFilepath.begin(), sanitised_sceneFilepath.end(), '\\', '/');
		std::filesystem::path fs_path(sanitised_sceneFilepath);

		const std::string fsPath_string(fs_path.string());
		std::string sceneFilename(sanitised_sceneFilepath);
		
		// if the string does have resources/scene, we need to extract out the filename
		const auto findPos = fsPath_string.find(s_Scene_Path_String);
		if (findPos != std::string::npos) {
			sceneFilename = sanitised_sceneFilepath.substr(findPos + s_Scene_Path_String.size());
		}

		if (outFilename) {
			*outFilename = sceneFilename;
		}

		fs_path = s_SCENE_PATH / sceneFilename; // just to be sure, confirm plus chop, just reconstruct the filepath

		if (outFsFilepath) {
			*outFsFilepath = fs_path;
		}

		/*AeonCore_INFO("Constructed path:{}", fs_path.string());
		AeonCore_INFO("New substring:   {}", sceneFilename);*/
	}
}
