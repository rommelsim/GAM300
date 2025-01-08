/*!***************************************************************************
	\file			EditorApp.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definition to setup
					and display the editor
					
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "EditorApp.h"
#include "EditorLayer.h"
#include <Scripting/Scripting.h>
#include "Core/SceneManager.h"

namespace Editor {
	void ScriptFileWatchEvent(const std::string& path, const filewatch::Event change_type) {
		bool ifPathIsGood = path == "AeonScripts.dll"; // need to check if the path only contains the file we are watching, no folders in the path whatsoever
		//AeonCore_WARN("raaah path: {}, {}", ifPathIsGood, std::filesystem::absolute(path).string());

		EditorApp* editorApp = EditorApp::GetEditorApp();
		if (ifPathIsGood && change_type == filewatch::Event::modified) {
			AeonCore_WARN("ScriptFileWatch, {}", path);

			if (!editorApp->mDidSubmitToMainThread) {
				editorApp->mDidSubmitToMainThread = true;
				AeonCore::Application::GetApplication().SubmitToMainThread([]() {
					std::this_thread::sleep_for (std::chrono::seconds(1));
					EditorApp* editorApp = EditorApp::GetEditorApp();

					if (editorApp->GetRuntimeState() != AeonCore::RuntimeState::Stop) {
						SceneHierarchyPanel::SetSelectedEntityID(AeonCore::INVALID_ENTITY_ID);
						editorApp->OnRuntimeStop();

						std::string tempFile("./resources/scene/temp.scene");
						AeonCore::Coordinator::GetInstance().EmptyActiveScene();
						AeonCore::Scripting::ReloadAssembly(true);
						AeonCore::SceneManager::LoadScene(tempFile);
						if (std::remove(tempFile.c_str()) == 0)
						{
							AeonCore_INFO("Temp scene deleted");
						}
					} else {
						AeonCore::Scripting::ReloadAssembly(false);
					}
					//editorApp->UnsetFileWatch();


					editorApp->mDidSubmitToMainThread = false;
					//editorApp->InitializeFileWatch();
				});
			}
		}
	}

	EditorApp::EditorApp() {
		AeonCore::SceneManager::SetActiveScene(AeonCore::SceneManager::GetInstance().CreateNewScene(AeonCore::SceneType::WillSpawn, AeonCore::MAX_ENTITIES));

		mImGuiLayer = &EditorLayer::GetInstance();
		PushOverlay(mImGuiLayer);
		
		InitializeFileWatch();
	}

	EditorApp* EditorApp::GetEditorApp()
	{
		return dynamic_cast<EditorApp*>(&GetApplication());
	}

	void EditorApp::InitializeFileWatch() {
		auto absAppFilepath = std::filesystem::absolute(AeonCore::Scripting::sAppAssemblyFilepath);
		mScriptFileWatch = std::make_unique<filewatch::FileWatch<std::string>>(absAppFilepath.string(), ScriptFileWatchEvent);
	}

	void EditorApp::UnsetFileWatch() {
		mScriptFileWatch.reset();
	}
}
