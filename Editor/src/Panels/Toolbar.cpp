/*!***************************************************************************
	\file			Toolbar.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Nov 1, 2023
	\brief			This source file consists of function defintion for tool bar on
					the editor
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "Toolbar.h"
#include "SceneHierarchy.h"
//External Library
#include <Imgui.h>
#include <filesystem>
//Engine
#include <Core/Log.h>
#include <Resource/Texture.h>
#include <Resource/ResourceManager.h>
#include <Core/Application.h>


namespace Editor {

	void ToolBars::Show()
	{
		AeonCore::Application& engineApp = AeonCore::Application::GetApplication();
		
		auto &resourceManager = AeonCore::ResourceManager::GetInstance();
		std::filesystem::path working_dir(std::filesystem::current_path());
		auto parent_path = working_dir;

		std::string tempFile = "./resources/scene/temp.scene";
		//std::cout << parent_path << std::endl;
		//Play
		//std::string play = parent_path.string()+  "/resources/textures/Cat_2.dds";
		std::string play = parent_path.string() + "/resources/textures/play.dds";
		resourceManager.LoadTexture("play", play,AeonCore::Texture::Type::None);
		auto textureplayId=*resourceManager.GetTexture("play");
		//Pause
		std::string pause = parent_path.string() + "/resources/textures/pause.dds";
		resourceManager.LoadTexture("pause", pause, AeonCore::Texture::Type::None);
		auto texturePauseId = *resourceManager.GetTexture("pause");
		//Stop
		std::string stop = parent_path.string() + "/resources/textures/stop.dds";
		resourceManager.LoadTexture("stop", stop, AeonCore::Texture::Type::None);
		auto textureStopId = *resourceManager.GetTexture("stop");

		//AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		//To DO QOL: click play once --> play click play twice --> stop
		//auto& audio = gCoordinatorPP.GetComponent<AeonCore::AudioComponent>(entityAudio);
		auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoMove;
		bool toolbarflag = false;
		ImGui::Begin("##ToolBar", &toolbarflag, flags);

		float buttonWidth = 100.0f;
		float totalWidth = buttonWidth * 3;
		float centerX = (ImGui::GetWindowWidth() - totalWidth) * 0.5f;
		ImGui::SetCursorPosX(centerX);

		ImGui::BeginDisabled(engineApp.GetRuntimeState() == AeonCore::RuntimeState::Start);
		//if(ImGui::Button("Play")){
		if (ImGui::ImageButton(reinterpret_cast<void*>((uint64_t)textureplayId.GetTextureID()), ImVec2(50.0f, 50.0f))) {
			if (engineApp.GetRuntimeState() != AeonCore::RuntimeState::Pause) // if the previous state was not paused, we need to run serializer
			//if (!sceneSerialized)
			{
				AeonCore::SceneManager::SaveScene(tempFile);
				sceneSerialized = true;
			}
			engineApp.OnRuntimeStart();
		
		}

		ImGui::EndDisabled();
		// Align the "Pause" and "Stop" buttons to the sides
		ImGui::SameLine(centerX + buttonWidth);

		ImGui::BeginDisabled(engineApp.GetRuntimeState() != AeonCore::RuntimeState::Start);
		//if (ImGui::Button("Pause")) {
		if (ImGui::ImageButton(reinterpret_cast<void*>((uint64_t)texturePauseId.GetTextureID()), ImVec2(50.0f, 50.0f))) {
			engineApp.OnRuntimePause();
		}
		ImGui::EndDisabled();

		ImGui::SameLine(centerX + 2 * buttonWidth);

		ImGui::BeginDisabled(engineApp.GetRuntimeState() == AeonCore::RuntimeState::Stop);
		//if (ImGui::Button("Stop")) {
		if (ImGui::ImageButton(reinterpret_cast<void*>((uint64_t)textureStopId.GetTextureID()), ImVec2(50.0f, 50.0f))) {
			engineApp.OnRuntimeStop();
			//if (sceneSerialized)
			{
				// TODO: serializer.DeserializeScene(tempFile);
		        AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
				SceneHierarchyPanel::SetSelectedEntityID(AeonCore::INVALID_ENTITY_ID);
		        gCoordinator.EmptyActiveScene();

				AeonCore::SceneManager::LoadScene(tempFile);
				if (std::remove(tempFile.c_str()) == 0)
				{
					AeonCore_INFO("Temp scene deleted");
				}
				sceneSerialized = false;
			}
		}
		ImGui::EndDisabled();
		ImGui::End();
	}
}
