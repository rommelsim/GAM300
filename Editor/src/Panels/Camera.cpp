/*!***************************************************************************
	\file			Camera.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Nov 1, 2023
	\brief			This source file consists of function defintion for settings panel on
					the editor
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "Camera.h"
#include "../ImGuiUtility.h"
//External library
#include <iostream>
#include <imgui.h>
//Engine
#include <Camera/CameraManager.h>
#include <ECS/Components/Camera.h>
#include <ECS/Components/Render.h>
#include <Rendering/Rendering.h>
#include "ECS/Systems/RenderSystem.h"
#include "Core/Application.h"

namespace Editor {
	void Camera::Show(AeonCore::EntityID& cameraID)
	{
		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
		ImGui::Begin("Settings");

		// Scene setting
		{
			ImGui::Text("Scene"); ImGui::Separator();

			AeonCore::SceneRef activeScene = AeonCore::SceneManager::GetInstance().GetActiveScene();
			AeonCore::SceneType selectedSceneType = activeScene->GetSceneType();
			int selectedSceneTypeInt = static_cast<int>(selectedSceneType);
			const auto& sceneTypeValues = AeonCore::Scene::GetTypeNames();

			ImGui::Text("Type"); ImGui::SameLine();
			ImGui::PushItemWidth(150.0f);
			if (ImGui::BeginCombo("##SceneType", sceneTypeValues[selectedSceneTypeInt].c_str())) {
				for (int i = 0; i < static_cast<int>(sceneTypeValues.size()); i++) {
					bool is_selected = selectedSceneTypeInt == i;

					if (ImGui::Selectable(sceneTypeValues[i].c_str(), is_selected)) {
						activeScene->SetSceneType(static_cast<AeonCore::SceneType>(i));
						is_selected = true;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}

		// Camera Setting
		{
			ImGui::Text("Camera");
			ImGui::Separator();
			auto entities = gCoordinator.GetEntities();
			ImGui::Columns(4, 0, false);

			AeonCore::CameraManager& cameraman = AeonCore::CameraManager::GetInstance();
			AeonCore::EditorCamera& editorcam = AeonCore::EditorCamera::GetInstance();
			if (ImGui::Button("Editor Cam"))
			{
				cameraman.SetActiveCamera(&editorcam);
				cameraman.SetEnableEntityCam(false);

				if (AeonCore::Application::GetApplication().GetRuntimeState() != AeonCore::RuntimeState::Start)
				{
				
				}
			}

			for (auto it : entities) {
				if (it.IsChild()) 
				{
					continue; 
				}
			
				//check all entities for camera component
				if (gCoordinator.HasComponent<AeonCore::Camera>(it)) {
					//ImGui::Text(it.GetEntityName().c_str());
				
					//static bool toggle{ false };
					auto& cameraComp = gCoordinator.GetComponent<AeonCore::Camera>(it.GetEntityID()); (void)cameraComp;


					//ImGui::BeginDisabled(cameraman.GetActiveCameraID() == it.GetEntityID());
					if (ImGui::Button(it.GetEntityName().c_str()))
					{
						if (AeonCore::Application::GetApplication().GetRuntimeState() != AeonCore::RuntimeState::Start)
						{
							cameraman.SwitchCamera(it.GetEntityID());
							cameraman.SetEnableEntityCam(true);
							/*toggle = !toggle;*/
							m_Camera = it.GetEntityID();

							//AeonCore_WARN("Camera ID {}", it.GetEntityID());
							////auto test = it.GetEntityID();
							//std::cout << "Move Speed" << cameraComp.GetCameraMovementSpeed() << std::endl;
						}
					}

					ImGui::NextColumn();
				}

			}


			ImGui::Columns(1);
			ImGui::Dummy(ImVec2(0, 5.0f));
			//AeonCore_WARN("Current Camera ID: {}", m_Camera);
			if (gCoordinator.HasComponent<AeonCore::Camera>(m_Camera)) {
				ImGui::Separator();
				auto &cameraComp = gCoordinator.GetComponent<AeonCore::Camera>(m_Camera);
				float moveSpeed = cameraComp.GetCameraMovementSpeed();
				ImGuiUtility::DrawDragFloat("Movement Speed", "##moveSpeed", &moveSpeed,1.f,5.f);
				cameraComp.SetCameraMovementSpeed(moveSpeed);
				//camera position 
				glm::vec3 cameraPos = cameraComp.GetCameraPosition();
				if (ImGuiUtility::DrawVec3Control("Camera Position", cameraPos))
					cameraComp.SetCameraPosition(cameraPos);

				bool selectedGameCamera		{ cameraComp.GetCameraRoleSelected() };
				bool selectedPlayer1Camera	{ cameraComp.GetCameraPlayer1Selected() };
				bool selectedPlayer2Camera	{ cameraComp.GetCameraPlayer2Selected() };

				ImGuiUtility::DrawCheckBox("Game Camera ", "##GameCamera", &selectedGameCamera);
				ImGuiUtility::DrawCheckBox("Player1 Camera ", "##Player1Camera", &selectedPlayer1Camera);
				ImGuiUtility::DrawCheckBox("Player2 Camera ", "##Player2Camera", &selectedPlayer2Camera);

				if (selectedGameCamera) 
				{
					cameraComp.SetCameraRole(AeonCore::CameraRole::CAMERA);
					cameraComp.SetCameraRoleSelected(true);
				}

				else 
				{
					cameraComp.SetCameraRole(AeonCore::CameraRole::NONE);
				}
			}
			ImGui::Dummy(ImVec2(0, .5f));
			ImGui::Separator();
		}

		//Light settings
		{
			ImGui::Text("Light");
			ImGui::Separator();
			//auto& RenderingEngineInstance = AeonCore::Rendering::GetInstance();
			// glm::vec3 ambientLight = RenderingEngineInstance.GetUniversalLightColor();
			glm::vec3 ambientLight = AeonCore::LightManager::GetInstance().GetAmbientLight();
			float ambientColor[3] = { ambientLight.r,ambientLight.g,ambientLight.b };
			ImGuiUtility::DrawColor(" Universal", "##universalLight", ambientColor);
			//RenderingEngineInstance.SetUniversalLightColor(glm::vec3(ambientColor[0], ambientColor[1], ambientColor[2]));
			AeonCore::LightManager::GetInstance().SetAmbientLight(glm::vec3(ambientColor[0], ambientColor[1], ambientColor[2]));
		}

		// UI Renderer Flag
		ImGuiUtility::DrawCheckBox("Render UI ", "##RenderUIEditor", &AeonCore::Rendering::GetInstance().RenderUIEditor);

		// Disable shadow
		ImGuiUtility::DrawCheckBox("Shadows ", "##ShadowUIEditor", &AeonCore::RenderSystem::onShadow);
		//AeonCore_INFO("{0}", AeonCore::RenderSystem::onShadow);

		// Disable HDR
		ImGuiUtility::DrawCheckBox("HDR ", "##HDRUIEditor", &AeonCore::RenderSystem::HDR);
		//AeonCore_INFO("{0}", AeonCore::RenderSystem::HDR);

		// Gamma 
		ImGui::DragFloat(" Gamma ", &AeonCore::RenderSystem::mGamma, 0.01f, 1.0f, 10.0f);

		ImGui::End();
	}
}
