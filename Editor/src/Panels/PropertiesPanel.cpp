/*!***************************************************************************
	\file			PropertiesPanel.cpp
	\author			Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definition for the inspector
					panel on the editor.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "PropertiesPanel.h"
#include "../ImGuiUtility.h"

//External Library
#include <iostream>
#include <imgui.h>
#include <glm/vec3.hpp>
#include <filesystem>

//Engine
#include <ECS/Components/Transform.h>
#include <ECS/Components/Behaviour.h>
#include <ECS/Components/Audio.h>
#include <ECS/Components/AudioListener.h>
#include <ECS/Components/Font.h>
#include <ECS/Systems/Renderer2DSystem.h>
#include <Scripting/Scripting.h>
#include <Core/Log.h>
#include <Core/Application.h>
#include <Rendering/GeomLib.h>
#include <Resource/ResourceManager.h>
#include <Resource/Material.h>

#include "ECS/Components/Collider.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Systems/PhysicsSystem.h"
#define TRANSFORM_MAX_NAME_SIZE 64
#define TRANSFORM_MAX_TAG_SIZE 64

namespace Editor {
	std::string PropertiesPanel::s_fileExt = "";
	void PropertiesPanel::Show(AeonCore::EntityID entity)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();

		const char* a = "0"; (void)a;
		static bool check = true;
		ImGui::Begin("Inspector");
		//check should replace with NULL ENTITY
		if (!gCoordinatorPP.EntityExists(entity) || !gCoordinatorPP.HasComponent<AeonCore::Transform>(entity)) {
			ImGui::End();
			return;
		}

		TransformComponent(entity);
		RigidBodyComponent(entity);
		Collider(entity);
		RenderComponent(entity);
		Audio(entity);
		AudioListener(entity); /////////////////////////////////////////// ROY ////////////////////
		LightingComponent(entity);
		FontComponent(entity);
		ParticlesComponent(entity);

		//Scripting
		ScriptComponent(entity);
		UIComponent(entity);
		CameraComponent(entity);
		AddComponenet(entity);
		ImGui::End();

	}
	void PropertiesPanel::TransformComponent(AeonCore::EntityID& entityTransform)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();

		//Tansform 
		ImGui::CollapsingHeader(" Transform", ImGuiTreeNodeFlags_Leaf);
		std::string name = gCoordinatorPP.GetEntity(entityTransform)->GetEntityName();
		if (ImGuiUtility::DrawEnterInput("Name", "##entityLabel", &name))
		{
			gCoordinatorPP.GetEntity(entityTransform)->SetEntityName(name.c_str());
		}


		ImGuiUtility::DrawTextLabelControl("ID", "##TransformTag", (char*)std::to_string(AeonCore::ECSUtils::ExtractRealEntityId(gCoordinatorPP.GetEntity(entityTransform)->GetEntityID())).c_str(), TRANSFORM_MAX_TAG_SIZE);

		glm::vec3 pos = gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).GetPosition();
		//glm::vec3 rot = gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).GetEulerAngle();
		glm::vec3 scale = gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).GetScale();
		bool onRotation = gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).GetOnRotation();

		if (ImGuiUtility::DrawVec3Control("Position", pos))
			gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).SetPosition(pos);

		glm::vec3 rot = gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).GetEulerAngle();
		glm::vec3 rotDegrees = glm::vec3(ToDegrees(rot.x), ToDegrees(rot.y), ToDegrees(rot.z));

		if (ImGuiUtility::DrawVec3Control("Euler Angles", rotDegrees)) {
			rot = glm::vec3(ToRadians(rotDegrees.x), ToRadians(rotDegrees.y), ToRadians(rotDegrees.z));
			gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).SetEulerAngle(rot);
		}
		//ImGuiUtility::DrawVec3Control("Euler Angles", gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).GetEulerAngle());

		if (ImGuiUtility::DrawVec3Control("Scale", scale))
			gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).SetScale(scale);

		if(ImGuiUtility::DrawCheckBox("Turn on rotation", "##TurnOnRotationTag", &onRotation))
			gCoordinatorPP.GetComponent<AeonCore::Transform>(entityTransform).SetOnRotation(onRotation);
	}
	
	void PropertiesPanel::ScriptComponent(AeonCore::EntityID& entityScript)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();

		if (gCoordinatorPP.HasComponent<AeonCore::Behaviour>(entityScript)) {
			bool isVisible = true;
			if (ImGui::CollapsingHeader(" Behaviour", &isVisible)) {
				AeonCore::Behaviour& behaviourComp = gCoordinatorPP.GetComponent<AeonCore::Behaviour>(entityScript);
				const auto& behaviourClzzs = AeonCore::Scripting::GetBehaviourClasses();
				static auto selectedAvailScript{ behaviourClzzs.begin() };

				if (AeonCore::Scripting::sTellEditorScriptsReloaded) {
					selectedAvailScript = behaviourClzzs.end();
					AeonCore::Scripting::sTellEditorScriptsReloaded = false;
				}

				ImGui::Text("Available Scripts:");
				ImGui::SameLine();

				ImGui::PushItemWidth(150.0f);
				// Add selected script for specific entity (idk how to do this better, but yeah)
				if (ImGui::BeginCombo("##AvailableScripts", selectedAvailScript == behaviourClzzs.end() ? "" : selectedAvailScript->first.c_str())) {
					for (auto bvrIt = behaviourClzzs.begin(); bvrIt != behaviourClzzs.end(); bvrIt++) {
						if (behaviourComp.HasBehaviourScript(bvrIt->first)) continue;

						const bool is_selected = bvrIt == selectedAvailScript;

						if (ImGui::Selectable(bvrIt->first.c_str(), is_selected)) {
							selectedAvailScript = bvrIt;
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();

				ImGui::SameLine();
				ImGui::BeginDisabled(selectedAvailScript == behaviourClzzs.end());
				if (ImGui::Button("Add Script")) {
					behaviourComp.AddBehaviourScript(selectedAvailScript->first);
					selectedAvailScript = behaviourClzzs.end();
				}
				ImGui::EndDisabled();

				auto* vectorScriptInst = AeonCore::Scripting::GetBehaviourInstances(behaviourComp.GetOwnerID());
				if (vectorScriptInst != nullptr) {
					bool isRuntimeStopped = AeonCore::Application::GetApplication().GetRuntimeState() == AeonCore::RuntimeState::Stop;
					int scriptToRemove = -1; // -1 is no script to remove, greater than -1 means got script to remove
					for (auto& nameIdPair : behaviourComp.mBehaviourNames) {
						AeonCore::ScriptInstanceRef scriptInst = (*vectorScriptInst)[nameIdPair.second];

						if (!scriptInst->GetScriptClass()->IsClassValid()) continue;

						ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;
						bool hasFields = !scriptInst->GetScriptClass()->GetFields().empty();

						if (!hasFields) {
							nodeFlags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
							//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing() - ImGui::GetStyle().FramePadding.x);
						}

						bool isopen = ImGui::TreeNodeEx(nameIdPair.first.c_str(), nodeFlags);
						if (ImGui::BeginPopupContextItem()) {
							if (ImGui::MenuItem("Reset values")) {
								scriptInst->ResetFields();
							}
							if (ImGui::MenuItem("Remove script")) {
								scriptToRemove = nameIdPair.second;
							}
							ImGui::EndPopup();
						}
						if (!hasFields) {
							//ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing() - ImGui::GetStyle().FramePadding.x);
						} else if (isopen) {
							std::string bvrImguiID("##Behaviour_" + scriptInst->GetScriptClass()->GetName() + "_");

							for (auto& nameToField : scriptInst->GetScriptClass()->GetFields()) {
								std::string fieldName{ nameToField.first };
								const AeonCore::ScriptFieldRef& scriptableField = nameToField.second;
								bool isDefaultValue; // to know whether default value was retrieved or not

								if (scriptableField->mType == AeonCore::ScriptDataType::Bool) {
									const bool oldValBool{ scriptInst->GetFieldValue<bool>(fieldName, isRuntimeStopped, &isDefaultValue) };
									bool bvrVal{ oldValBool };

									ImGuiUtility::DrawCheckBox(fieldName + (isDefaultValue ? "" : "*"), (bvrImguiID + fieldName).c_str(), &bvrVal);

									if (bvrVal != oldValBool) {
										scriptInst->SetFieldValue<bool>(fieldName, bvrVal, isRuntimeStopped);
									}
								}
								else if (scriptableField->mType == AeonCore::ScriptDataType::Float) {
									const float oldValFloat{ scriptInst->GetFieldValue<float>(fieldName, isRuntimeStopped, &isDefaultValue) };
									float fieldValFloat{ oldValFloat };
									ImGuiUtility::DrawInputFloat(fieldName + (isDefaultValue ? "" : "*"), (bvrImguiID + fieldName).c_str(), &fieldValFloat, 0.01f, 0.1f, "%.2f");

									if (oldValFloat != fieldValFloat) {
										scriptInst->SetFieldValue<float>(fieldName, fieldValFloat, isRuntimeStopped);
									}
								}
								else if (scriptableField->mType == AeonCore::ScriptDataType::Double) {
									const double oldValDouble{ scriptInst->GetFieldValue<double>(fieldName, isRuntimeStopped, &isDefaultValue) };
									double fieldValDouble{ oldValDouble };
									ImGuiUtility::DrawInputDouble(fieldName + (isDefaultValue ? "" : "*"), (bvrImguiID + fieldName).c_str(), &fieldValDouble, 0.01f, 0.1f, "%.2f");

									if (oldValDouble != fieldValDouble) {
										scriptInst->SetFieldValue<double>(fieldName, fieldValDouble, isRuntimeStopped);
									}
								}
								else if (scriptableField->mType == AeonCore::ScriptDataType::Int) {
									const int oldValInt{ scriptInst->GetFieldValue<int>(fieldName, isRuntimeStopped, &isDefaultValue) };
									int fieldValInt{ oldValInt };
									ImGuiUtility::DrawInputInt(fieldName + (isDefaultValue ? "" : "*"), (bvrImguiID + fieldName).c_str(), &fieldValInt);

									if (fieldValInt != oldValInt) {
										scriptInst->SetFieldValue<int>(fieldName, fieldValInt, isRuntimeStopped);
									}
								}
								else if (scriptableField->mType == AeonCore::ScriptDataType::String) {
									const std::string oldValStr{ scriptInst->GetFieldValueStr(fieldName, isRuntimeStopped, &isDefaultValue) };
									char* fieldValStr{ const_cast<char*>(oldValStr.c_str()) };
									ImGuiUtility::DrawTextLabelControl(fieldName + (isDefaultValue ? "" : "*"), (bvrImguiID + fieldName).c_str(), fieldValStr, TRANSFORM_MAX_NAME_SIZE);

									if (fieldValStr != oldValStr) {
										scriptInst->SetFieldValueStr(fieldName, fieldValStr, isRuntimeStopped);
									}
								}
								else if (scriptableField->mType == AeonCore::ScriptDataType::Vector3) {
									const glm::vec3 oldValVec{ scriptInst->GetFieldValue<glm::vec3>(fieldName, isRuntimeStopped, &isDefaultValue) };
									glm::vec3 fieldValVec{ oldValVec };
									ImGuiUtility::DrawVec3Control(fieldName + (isDefaultValue ? "" : "*"), fieldValVec);

									if (oldValVec != fieldValVec) {
										scriptInst->SetFieldValue<glm::vec3>(fieldName, fieldValVec, isRuntimeStopped);
									}
								}
							}
							ImGui::TreePop();
						}
					}

					if (scriptToRemove >= 0) {
						AeonCore::Scripting::DestroyBehaviourInstance(&behaviourComp, scriptToRemove);
					}
				}
			}

			if (!isVisible) {
				gCoordinatorPP.RemoveComponent<AeonCore::Behaviour>(entityScript);
			}
		}
	}
	void PropertiesPanel::LightingComponent(AeonCore::EntityID& entityLighting)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool close = true;
		//check for component 
		if (gCoordinatorPP.HasComponent<AeonCore::Lighting>(entityLighting)) {
			if (ImGui::CollapsingHeader("Light",&close, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) 
			{
				AeonCore::Lighting& lightingCompo = gCoordinatorPP.GetComponent<AeonCore::Lighting>(entityLighting);

				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::Lighting>(entityLighting);
				}
				glm::vec3 _GetLightPos = lightingCompo.GetPosition();

				glm::vec3 _GetLightColor = lightingCompo.GetColor();
				float _SetLightColor[3] = {_GetLightColor.x, _GetLightColor.y, _GetLightColor.z};
				ImGuiUtility::DrawColor(" Light Color", "##LightColor", _SetLightColor);
				lightingCompo.SetColor(glm::vec3(_SetLightColor[0], _SetLightColor[1], _SetLightColor[2]));

				glm::vec3 lightDirection = lightingCompo.GetLightDirection();
				if (ImGuiUtility::DrawVec3Control(" Direction", lightDirection))
				{
					lightingCompo.SetLightDirection(lightDirection);
				}

				float lightIntensity = lightingCompo.GetLightIntensity();
				ImGuiUtility::DrawDragFloat("Intensity", "##lightIntensity", &lightIntensity);
				lightingCompo.SetLightIntensity((short)lightIntensity);
				short xyz = lightingCompo.GetLightIntensity(); (void)xyz;

				float cutoff = lightingCompo.GetLightCutoff();
				ImGuiUtility::DrawDragFloat("Inner Cutoff", "##lightcutoff", &cutoff);
				lightingCompo.SetLightCutoff((short)cutoff);

				float Outercutoff = lightingCompo.GetLightOuterCutoff();
				ImGuiUtility::DrawDragFloat("Outer Cutoff", "##lightoutercutoff", &Outercutoff);
				lightingCompo.SetLightOuterCutoff((short)Outercutoff);

				//enum
				const char* types[] = { "SPOTLIGHT", "POINT", "DIRECTIONAL" };
				auto lightType = lightingCompo.GetLightType();
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" Light Type");
				ImGui::NextColumn();


				if (ImGui::BeginCombo("##Select Light", types[static_cast<int>(lightType)]))
				{
					for (int i = 0; i < IM_ARRAYSIZE(types); i++)
					{
						const bool isSelected = (lightType == static_cast<AeonCore::Lighting::LightType>(i));
						if (ImGui::Selectable(types[i], isSelected))
						{
							lightType = static_cast<AeonCore::Lighting::LightType>(i);
							lightingCompo.SetLightType(lightType);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Columns(1);
			}
		}

	}

	void PropertiesPanel::RenderComponent(AeonCore::EntityID& entityRender)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool close = true;
		std::filesystem::path working_dir(std::filesystem::current_path());
		auto parent_path = working_dir;

		if (gCoordinatorPP.HasComponent<AeonCore::Render>(entityRender)) {
			AeonCore::Render& renderCompo = gCoordinatorPP.GetComponent<AeonCore::Render>(entityRender);
			auto abc = gCoordinatorPP.GetComponent<AeonCore::Render>(entityRender);
			std::filesystem::path diffusePath = renderCompo.GetDiffuseTexture();
			std::filesystem::path modelPath = renderCompo.GetModelPath();
			std::filesystem::path specularPath = renderCompo.GetSpecularTexture();
			if (ImGui::CollapsingHeader(" Model",&close, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::Render>(entityRender);
				}
				/*On the fly*/
				ImGuiUtility::DrawDropDragTarget(" Model", modelPath.empty()?"##":modelPath.filename().string().c_str()); 
				if (ImGui::BeginDragDropTarget())
				{

					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_OBJECT_ITEM"))
					{
						//auto modelData = (const char*)payload->Data;
						CompressObject(renderCompo, (const char*)payload->Data);
		
					}
					else if (const ImGuiPayload* payload2 = ImGui::AcceptDragDropPayload("MOVING_ITEM")) {
						auto movingData = (const char*)payload2->Data;
						std::string filePath = std::filesystem::current_path().string() + "/resources/objects/";
						renderCompo.SetModel(filePath + static_cast<std::string>(movingData));
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::CollapsingHeader("	Material", ImGuiTreeNodeFlags_Leaf);
				ImGuiUtility::DrawDropDragTarget(" Diffuse", diffusePath.empty() ? "##" : diffusePath.filename().string().c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_ITEM"))
					{
						CompressTexture(renderCompo, (const char*)payload->Data, "Diffuse");
					}
					ImGui::EndDragDropTarget();
				}

				ImGuiUtility::DrawDropDragTarget(" Specular", specularPath.empty() ? "##" : specularPath.filename().string().c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_ITEM"))
					{
						CompressTexture(renderCompo, (const char*)payload->Data, "Specular");
					}
					ImGui::EndDragDropTarget();
				}

				float shininess = renderCompo.GetShininess();
				ImGuiUtility::DrawDragFloat("Shininess", "##shiniess", &shininess);
				renderCompo.SetShininess(shininess);

				ImGuiUtility::DrawErrorMessage("Error Popup", s_fileExt);
			}
		
			
		}

	}
	
	//TO TAKE IN ENTITY ID
	void PropertiesPanel::Audio(AeonCore::EntityID& entityAudio)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool close = true;
		if (gCoordinatorPP.HasComponent<AeonCore::AudioComponent>(entityAudio)) {
			auto& audio = gCoordinatorPP.GetComponent<AeonCore::AudioComponent>(entityAudio);
			if (ImGui::CollapsingHeader("Audio", &close, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::AudioComponent>(entityAudio);
				}
				/*Drag Drop Audio File*/
				std::string labelName;
				if (audio.GetSoundFile().empty()) {
					labelName = "##";
				}
				else {
					std::filesystem::path labelPath = audio.GetSoundFile();
					labelName = labelPath.filename().string();
				}
				ImGuiUtility::DrawDropDragTarget(" Audio Name", labelName.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_ITEM"))
					{
						auto audioData = (const char*)payload->Data;
						std::string audioName = audioData;
						audio.setSoundFile("./resources/audios/" + audioName);
					}

					ImGui::EndDragDropTarget();
				}
				/*End Drag Drop*/
				/*Channel Type*/
				const char* types[] = { "SFX", "BGM", "Voice" };
				auto audioType = audio.GetChannelType();
				ImGui::Columns(2,"channelType",false);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" Channel Type");
				ImGui::NextColumn();
				if (ImGui::BeginCombo("##Select Audio Type", types[static_cast<int>(audioType)]))
				{
					for (int i = 0; i < IM_ARRAYSIZE(types); i++)
					{
						const bool isSelected = (audioType == static_cast<AeonCore::AudioComponent::ChannelType>(i));
						if (ImGui::Selectable(types[i], isSelected))
						{
							audioType = static_cast<AeonCore::AudioComponent::ChannelType>(i);
							audio.setChannelType(audioType);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Columns(1);
				//AeonCore_INFO("Current audio channel {}", audioTypeValue);
				/*End Channel ID*/

				float volume = audio.GetVolume();
				/*Volume*/
				ImGuiUtility::DrawDragFloat(" Volume", "##audioVolumeDrag", &volume);
				audio.SetVolume(volume);

				/* Pitch Slider */
				//float pitch = audio.GetPitch();
				//ImGui::SliderFloat("Pitch", &pitch, 0.5f, 2.0f); // Adjust the range as needed
				//audio.SetPitch(pitch); // Set the new pitch value
				float pitch = audio.GetPitch();

				//0.5 to 2 
				ImGuiUtility::DrawDragFloat(" Pitch", "##audioVolumePitch", &pitch, 0.5f,2.f);
				audio.SetPitch(pitch);
				/* Pan Slider */
				//-1 to 1
				float pan = audio.GetPan();
				ImGuiUtility::DrawDragFloat(" Pan", "##audioVolumePan", &pan, -1.f,1.f);
				audio.SetPan(pan);

			

				
				///*Volume*/
				//ImGuiUtility::DrawDragFloat(" VolumeDrag", "##audioVolumeDrag", &audio.GetVolume());
				/*Mute*/
				bool mode3D = audio.GetIIIDMode();
				ImGui::Columns(3, "audio3D", true);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" On 3D Mode");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##on3DMode", &mode3D))
				{
					mode3D ? audio.SetIIIDMode(true) : audio.SetIIIDMode(false);
				}
				ImGui::Columns(1, "audio3D");
				/*Mute*/
				bool mute = audio.GetMute();
				ImGui::Columns(3, "audioMute", true);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" On Mute");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##onMute", &mute)) 
				{
					mute ? audio.SetMute(true) : audio.SetMute(false);
				}
				ImGui::Columns(1, "audioMute");
				/*Loop*/
				bool loop = audio.GetLoop();
				ImGui::Columns(2, "audioLoop", false);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" On Loop");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##audio", &loop)) 
				{
					loop ? audio.SetLoop(true) : audio.SetLoop(false);
				}
				ImGui::Columns(1,"audioLoop");
				/* "Play on Awake" Checkbox */
				bool playOnAwake = audio.GetPlayOnAwake(); // Get the current "Play on Awake" state
				ImGui::Columns(2, "audioAwake", false);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" On Awake");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##onAwake", &playOnAwake))
				{
					//audio.SetPlayOnAwake(playOnAwake); // Set the new "Play on Awake" state

					playOnAwake ? audio.SetPlayOnAwake(true) : audio.SetPlayOnAwake(false);
				}
				ImGui::Columns(1, "audioAwake");


				ImGui::Columns(2, "audioPlay", false);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::NextColumn();
				//PLAY STOP 
				//ImGui::Columns(2);
				//ImGui::NextColumn();
				if (ImGui::Button("Play" ,ImVec2(100, 25)))
				{
					audio.PlaySound();
				}

				ImGui::SameLine();
				if (ImGui::Button("Stop", ImVec2(100, 25)))
				{
					audio.StopAllSounds();
				}
				//ImGui::Columns(1);
				ImGui::Columns(1, "audioPlay");
				//To Do QOL: when invoke playButton then stop button will appear

			}
		}

	}

	void PropertiesPanel::UIComponent(AeonCore::EntityID& entityComponent)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		std::filesystem::path working_dir(std::filesystem::current_path());
		auto parent_path = working_dir;
		bool close = true;
		if (gCoordinatorPP.HasComponent<AeonCore::UI>(entityComponent)) {
			auto& ui = gCoordinatorPP.GetComponent<AeonCore::UI>(entityComponent);
			if (ImGui::CollapsingHeader(" UI", &close, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::UI>(entityComponent);
				}
				std::filesystem::path labelPath = ui.GetTexturePath();
				ImGui::Columns(2,"UIFormat", false);
				ImGui::Text(" Format ");
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::NextColumn();
				const char* formats[] = { "Image", "Word" };
				auto formatType = ui.GetFormat();

				if (ImGui::BeginCombo("##UIFormat", formats[(int)formatType]))
				{
					for (int i = 0; i < IM_ARRAYSIZE(formats); i++)
					{
						const bool isSelected = (formatType == static_cast<AeonCore::UI::Format>(i));
						if (ImGui::Selectable(formats[i], isSelected))
						{
							formatType = static_cast<AeonCore::UI::Format>(i);
							ui.SetFormat(formatType);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Columns(1, "UIFormat", false);

				std::string text = ui.GetText();
				if (ImGuiUtility::DrawEnterInput(" Text", "##fontLabel", &text))
				{
					ui.SetText(text.c_str());
				}

				ImGuiUtility::DrawDropDragTarget(" Texture", labelPath.empty() ? "##" : labelPath.filename().string().c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_ITEM"))
					{
					CompressTexture(ui, (const char*)payload->Data, "UI");
					}
					ImGui::EndDragDropTarget();
				}
				ImGuiUtility::DrawErrorMessage("Error Popup", s_fileExt);
				const char* types[] = { "Default", "Foreground" ,"Background" };
				auto uiType = ui.GetLayer();
				ImGui::Columns(2, "UIType", false);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" Layer");
				ImGui::NextColumn();
				if (ImGui::BeginCombo("##Select UI Type", types[static_cast<int>(uiType)]))
				{
					for (int i = 0; i < IM_ARRAYSIZE(types); i++)
					{
						const bool isSelected = (uiType == static_cast<AeonCore::UI::Layer>(i));
						if (ImGui::Selectable(types[i], isSelected))
						{
							uiType = static_cast<AeonCore::UI::Layer>(i);

							// ui.SetLayer(uiType); 
							// RendererSystem::UpdateLayer(&ui, uiType);
							AeonCore::Renderer2DSystem::UpdateLayer(entityComponent, &ui, uiType);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Columns(1);



				unsigned int order = ui.GetOrder();
				ImGuiUtility::DrawInputInt(" Order", "##order", &order);
				ui.SetOrder(order);


				glm::vec3 clr = ui.GetColor();
				float _clr[3] = { clr.r, clr.g, clr.b };
				ImGuiUtility::DrawColor(" Font Color", "##Font Color", _clr);
				ui.SetColor(glm::vec3(_clr[0], _clr[1], _clr[2]));

				float opacity = ui.GetOpacity();
				ImGuiUtility::DrawDragFloat(" Opacity", "##Opacity", &opacity);
				ui.SetOpacity(opacity);

			}
		}
	}

	void PropertiesPanel::FontComponent(AeonCore::EntityID& entityFont)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		std::filesystem::path working_dir(std::filesystem::current_path());
		auto parent_path = working_dir;
		bool close  = true;
		//check for component 
		if (gCoordinatorPP.HasComponent<AeonCore::Font>(entityFont)) {
			if (ImGui::CollapsingHeader("Font",&close, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
				AeonCore::Font &fontCompo =gCoordinatorPP.GetComponent<AeonCore::Font>(entityFont);
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::Font>(entityFont);
				}
				std::string text = fontCompo.GetText();
				if (ImGuiUtility::DrawEnterInput(" Text", "##fontLabel", &text))
				{
					fontCompo.SetText(text.c_str());
				}
				//set type
				std::string labelName;
				if (fontCompo.GetFontPath().empty()) {
					labelName = "##";
				}
				else {
					std::filesystem::path labelPath= fontCompo.GetFontPath();
					labelName = labelPath.filename().string();
				}
				ImGuiUtility::DrawDropDragTarget(" Font", labelName.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_FONT_ITEM"))
					{
						auto fontData = (const char*)payload->Data;
						std::string fontName = fontData;
						fontCompo.SetFontType((parent_path.string() + "/resources/fonts/" + fontName).c_str());
						AeonCore_INFO("Drop Drag {} succeesful", fontName);
					}
					ImGui::EndDragDropTarget();
				}
				//set color
				glm::vec3 fontColor = fontCompo.GetColor();
				float fontColorArray[3] = { fontColor.r,fontColor.g,fontColor.b };
				ImGuiUtility::DrawColor(" Color", "##FontColor", fontColorArray);
				fontCompo.SetColor(glm::vec3(fontColorArray[0], fontColorArray[1], fontColorArray[2]));

			}
		}

	}

	void PropertiesPanel::ParticlesComponent(AeonCore::EntityID& entityParticles)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool close = true;

		if (gCoordinatorPP.HasComponent<AeonCore::Particles>(entityParticles)) {
			AeonCore::Particles& particlesCompo = gCoordinatorPP.GetComponent<AeonCore::Particles>(entityParticles);

			float life					= particlesCompo.GetParticleLife();
			unsigned int maxParticles	= particlesCompo.GetMaximumParticles();
			unsigned int spawnRate		= particlesCompo.GetSpawnParticleRate();

			glm::vec3 gravity		= particlesCompo.GetParticleGravity();
			glm::vec3 minVelocity	= particlesCompo.GetMinVelocity();
			glm::vec3 maxVelocity	= particlesCompo.GetMaxVelocity();
			glm::vec3 offset		= particlesCompo.GetParticleOffset();

			std::filesystem::path diffusePath	= particlesCompo.GetDiffuseTexture();
			std::filesystem::path modelPath		= particlesCompo.GetModelPath();
			std::filesystem::path specularPath	= particlesCompo.GetSpecularTexture();
			if (ImGui::CollapsingHeader("Particles", &close, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::Particles>(entityParticles);
					//gCoordinatorPP.RemoveComponent<AeonCore::Lighting>(entityRender);
				}


				if (ImGuiUtility::DrawInputInt("Max Particles", "##Max Particles", &maxParticles))
					gCoordinatorPP.GetComponent<AeonCore::Particles>(entityParticles).SetMaximumParticles(maxParticles);

				if (ImGuiUtility::DrawInputInt("Spawn Rate", "##Spawn Rate", &spawnRate))
					gCoordinatorPP.GetComponent<AeonCore::Particles>(entityParticles).SetSpawnParticleRate(spawnRate);

				if (ImGuiUtility::DrawInputFloat("Life", "##Life", &life, 0.01f, 0.1f, "%.2f")) 
					gCoordinatorPP.GetComponent<AeonCore::Particles>(entityParticles).SetParticleLife(life);

				if (ImGuiUtility::DrawVec3Control("Gravity", gravity))
					gCoordinatorPP.GetComponent<AeonCore::Particles>(entityParticles).SetParticleGravity(gravity);

				if (ImGuiUtility::DrawVec3Control("Offset", offset))
					gCoordinatorPP.GetComponent<AeonCore::Particles>(entityParticles).SetParticleOffset(offset);

				if (ImGuiUtility::DrawVec3Control("Min Velocity", minVelocity))
					gCoordinatorPP.GetComponent<AeonCore::Particles>(entityParticles).SetParticleMinVelocity(minVelocity);

				if (ImGuiUtility::DrawVec3Control("Max Velocity", maxVelocity))
					gCoordinatorPP.GetComponent<AeonCore::Particles>(entityParticles).SetParticleMaxVelocity(maxVelocity);



				/*On the fly*/
				ImGuiUtility::DrawDropDragTarget(" Particle Model", modelPath.empty() ? "##" : modelPath.filename().string().c_str()); //Only able to accept files from texture folder ; if others needed please let me know :)
				if (ImGui::BeginDragDropTarget())
				{
					std::filesystem::path working_dir2(std::filesystem::current_path());
					auto parent_path_cdrive = working_dir2.string();
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_OBJECT_ITEM"))
					{
						CompressObject(particlesCompo, (const char*)payload->Data);
					}
					else if (const ImGuiPayload* payload2 = ImGui::AcceptDragDropPayload("MOVING_ITEM")) {
						auto movingData = (const char*)payload2->Data;
						std::string filePath = std::filesystem::current_path().string() + "/resources/objects/";
						particlesCompo.SetModel(filePath + static_cast<std::string>(movingData));
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::CollapsingHeader("	Material", ImGuiTreeNodeFlags_Leaf);

				ImGuiUtility::DrawDropDragTarget(" Diffuse", diffusePath.empty() ? "##" : diffusePath.filename().string().c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_ITEM"))
					{
						CompressTexture(particlesCompo, (const char*)payload->Data, "Diffuse");
					}
					ImGui::EndDragDropTarget();
				}

				ImGuiUtility::DrawDropDragTarget(" Specular", specularPath.empty() ? "##" : specularPath.filename().string().c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_ITEM"))
					{
						CompressTexture(particlesCompo, (const char*)payload->Data, "Specular");
					}
					ImGui::EndDragDropTarget();
				}
				ImGuiUtility::DrawErrorMessage("Error Popup", s_fileExt);
			}


		}
	}

	void PropertiesPanel::RigidBodyComponent(AeonCore::EntityID& entityRigidBody)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool close = true;
		if (gCoordinatorPP.HasComponent<AeonCore::RigidBody>(entityRigidBody)) {
			if (ImGui::CollapsingHeader("Rigid Body", &close, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
				AeonCore::RigidBody& rigidCompo = gCoordinatorPP.GetComponent<AeonCore::RigidBody>(entityRigidBody);
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::RigidBody>(entityRigidBody);
				}

				//enum
				const char* types[] = { "STATIC", "DYNAMIC" };
				auto rigidType = rigidCompo.GetRigidBodyType();
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" Rigid Body Type");
				ImGui::NextColumn();

				//rigidType = static_cast<AeonCore::RigidBody::RigidBodyType>(1);
				//rigidCompo.SetRigidBodyType(rigidType);
				if (ImGui::BeginCombo("##Select Type", types[static_cast<int>(rigidType)]))
				{
					for (int i = 0; i < IM_ARRAYSIZE(types); i++)
					{
						const bool isSelected = (rigidType == static_cast<AeonCore::RigidBody::RigidBodyType>(i));
						if (ImGui::Selectable(types[i], isSelected))
						{
							rigidType = static_cast<AeonCore::RigidBody::RigidBodyType>(i);
							rigidCompo.SetRigidBodyType(rigidType);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Columns(1);
				if (rigidType == AeonCore::RigidBody::RigidBodyType::STATIC)
				{
					ImGui::BeginDisabled();
				}

				//gravity 
				bool gravity = rigidCompo.GetUseGravity();
				ImGui::Columns(2, "gravity", true);
				ImGui::SetColumnWidth(0, 160.0f);
				ImGui::Text(" Gravity");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##gravity", &gravity))
				{
					gravity ? rigidCompo.SetUseGravity(true) : rigidCompo.SetUseGravity(false);
				}
				ImGui::Columns(1, "gravity");

				//kinematic
				bool kinematic = rigidCompo.GetIsKinematic();
				ImGui::Columns(2, "kinematic", true);
				ImGui::SetColumnWidth(0, 160.0f);
				ImGui::Text(" Is Kinematic");
				ImGui::NextColumn();
				if (ImGui::Checkbox("##kinematic", &kinematic))
				{
					kinematic ? rigidCompo.SetIsKinematic(true) : rigidCompo.SetIsKinematic(false);
				}
				ImGui::Columns(1, "kinematic");

				//mass
				float mass = rigidCompo.GetMass();
				//ImGuiUtility::DrawDragFloat(" Mass", "##mass", &mass);
				if (ImGui::DragFloat(" Mass", &mass, 0.1f, 0.0f, 10000.0f, "%.3f"))
				{
					rigidCompo.SetMass(std::max(mass, 0.0f));
				}

				//linear drag 
				float lineardrag = rigidCompo.GetLinearDrag();
				/*ImGuiUtility::DrawDragFloat(" Linear Drag", "##linear", &linear);
				rigidCompo.SetLinearDrag(linear);*/
				if (ImGui::DragFloat(" Linear Drag", &lineardrag, 0.01f, 0.0f, 100.0f, "%.3f"))
				{
					rigidCompo.SetLinearDrag(lineardrag);
				}

				//angular drag 
				float angulardrag = rigidCompo.GetAngularDrag();
				/*ImGuiUtility::DrawDragFloat(" Angular Drag", "##angular", &angulardrag);
				rigidCompo.SetAngularDrag(angulardrag);*/
				if (ImGui::DragFloat(" Angular Drag", &angulardrag, 0.01f, 0.0f, 100.0f, "%.3f"))
				{
					rigidCompo.SetAngularDrag(angulardrag);
				}
				if (rigidType == AeonCore::RigidBody::RigidBodyType::STATIC)
				{
					ImGui::EndDisabled();
				}
			}

		}
	}

	void PropertiesPanel::Collider(AeonCore::EntityID& entityCube)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool close = true;
		if (gCoordinatorPP.HasComponent<AeonCore::Collider>(entityCube)) {
			//half extents 
			if (ImGui::CollapsingHeader("Collider", &close, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
				auto& colCompo = gCoordinatorPP.GetComponent<AeonCore::Collider>(entityCube);
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::Collider>(entityCube);
				}
				//half-extent

				/*glm::vec3 halfextent = cubeCompo.GetHalfExtents();
				ImGuiUtility::DrawVec3Control(" Half-extent", halfextent);
				cubeCompo.SetHalfExtents(halfextent);*/

				//enum
				const char *types[] = { "CUBE", "SPHERE", "CAPSULE" };
				auto colType = colCompo.GetColliderType();
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" Collider Type");
				ImGui::NextColumn();

				//rigidType = static_cast<AeonCore::RigidBody::RigidBodyType>(1);
				//rigidCompo.SetRigidBodyType(rigidType);
				if ( ImGui::BeginCombo("##Select Collider", types[static_cast<int>(colType)]) )
				{
					for ( int i = 0; i < IM_ARRAYSIZE(types); i++ )
					{
						const bool isSelected = (colType == static_cast<AeonCore::Collider::ColliderType>(i));
						if ( ImGui::Selectable(types[i], isSelected) )
						{
							colType = static_cast<AeonCore::Collider::ColliderType>(i);
							colCompo.SetColliderType(colType);
						}
						if ( isSelected )
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::Columns(1);

				//show bounds (bool) 
				bool isShowingBounds = colCompo.GetIsShowingBounds();
				ImGui::Columns(2, "bounds", true);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" Show Bounds");
				ImGui::NextColumn();
				if ( ImGui::Checkbox("##bounds", &isShowingBounds) )
				{
					isShowingBounds ? colCompo.SetIsShowingBounds(true) : colCompo.SetIsShowingBounds(false);
				}
				ImGui::Columns(1, "bounds");

				//trigger (bool) 
				bool trig = colCompo.IsTrigger();
				ImGui::Columns(2, "istrigger", true);
				ImGui::SetColumnWidth(0, 125.0f);
				ImGui::Text(" Is Trigger");
				ImGui::NextColumn();
				if ( ImGui::Checkbox("##istrigger", &trig) )
				{
					trig ? colCompo.SetTrigger(true) : colCompo.SetTrigger(false);
				}
				ImGui::Columns(1, "istrigger");

				glm::vec3 offset = colCompo.GetOffset();
				if (ImGuiUtility::DrawVec3Control(" ColOffset",offset))
				{
					colCompo.SetOffset(offset);
				}

				auto geom = colCompo.GetGeometry();
				switch (colType )
				{
				case AeonCore::Collider::ColliderType::CUBE:
					{
						AeonCore::ACPhysics::CubeGeometry * boxgeom = dynamic_cast<AeonCore::ACPhysics::CubeGeometry*>(geom);
						if ( !boxgeom ) return;
						glm::vec3 halfExt = boxgeom->GetHalfExtents();
						if ( ImGuiUtility::DrawVec3ControlTmpForXuan(" Cube Half Extents", halfExt, 0.01f,1000.0f)
							)
						{
							boxgeom->SetHalfExtents(halfExt);
							colCompo.SetGeometry(boxgeom);
						}
						break;
					}
				case AeonCore::Collider::ColliderType::SPHERE:
					{
						AeonCore::ACPhysics::SphereGeometry *spheregeom = dynamic_cast<AeonCore::ACPhysics::SphereGeometry *>(geom);
						if ( !spheregeom ) return;
						float radius = spheregeom->GetRadius();
						if (ImGui::DragFloat(" Sphere Radius", &radius, 0.01f, 0.01f, 1000.0f, "%.3f"))
						{
							spheregeom->SetRadius(radius);
							colCompo.SetGeometry(spheregeom);
						}
						break;
					}
				case AeonCore::Collider::ColliderType::CAPSULE:
					{
						AeonCore::ACPhysics::CapsuleGeometry *capgeom = dynamic_cast<AeonCore::ACPhysics::CapsuleGeometry *>(geom);
						if ( !capgeom ) return;
						float hh = capgeom->GetHalfHeight();
						float rad = capgeom->GetRadius();
						if (ImGui::DragFloat(" Capsule Half Height", &hh, 0.01f, 0.01f, 1000.0f, "%.3f"))
						{
							capgeom->SetHalfHeight(hh);
							colCompo.SetGeometry(capgeom);
						}
						if ( ImGui::DragFloat(" Capsule Radius", &rad, 0.01f, 1.0f, 1000.0f, "%.3f") )
						{
							capgeom->SetRadius(rad);
							colCompo.SetGeometry(capgeom);
						}
						break;
					}
				default:
					{
						AeonCore_WARN("ImGui Collider Type is not available.");
					}
				}

				auto material = colCompo.GetMaterial();
				float dynamicFriction = material.GetDynamicFriction();
				float staticFriction = material.GetStaticFriction();
				float resti = material.GetRestitution();
				if (
					ImGui::DragFloat(" Static Friction", &dynamicFriction, 0.01f) ||
					ImGui::DragFloat(" Dynamic Friction", &staticFriction, 0.01f) ||
					ImGui::DragFloat(" Restitution", &resti, 0.01f)
					)
				{
					AeonCore::ACPhysics::ACPhysicsMaterial tmpMat(staticFriction, dynamicFriction, resti);
					colCompo.SetMaterial(tmpMat);
				}

				
			}
				
		}
	}
	void PropertiesPanel::CameraComponent(AeonCore::EntityID& entityCamera)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool close = true;
		if (gCoordinatorPP.HasComponent<AeonCore::Camera>(entityCamera)) {
			if (ImGui::CollapsingHeader("Camera", &close, ImGuiTreeNodeFlags_Leaf)) {
				//AeonCore::Camera& cameraCompo = gCoordinatorPP.GetComponent<AeonCore::Camera>(entityCamera);
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::Camera>(entityCamera);
				}
			}
		}
	}

	void PropertiesPanel::AudioListener(AeonCore::EntityID& entityAudioListener)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool close = true;
		if (gCoordinatorPP.HasComponent<AeonCore::AudioListenerComponent>(entityAudioListener)) {
			if (ImGui::CollapsingHeader("AudioListener", &close, ImGuiTreeNodeFlags_Leaf)) {
				//AeonCore::Camera& cameraCompo = gCoordinatorPP.GetComponent<AeonCore::Camera>(entityCamera);
				if (!close) {
					gCoordinatorPP.RemoveComponent<AeonCore::AudioListenerComponent>(entityAudioListener);
				}
			}
		}
	}

	void PropertiesPanel::AddComponenet(AeonCore::EntityID& entityAdd)
	{
		AeonCore::Coordinator& gCoordinatorPP = AeonCore::Coordinator::GetInstance();
		bool toAdd = false;
		ImGui::Separator();
		std::vector<std::string> componentList{ "Font", "Audio", "AudioListener", "Behaviour","Render","UI","Camera", "Light", "Rigid Body", "Collider", "Particles"};
		// check which components the entity has and remove them from the list first.
		std::string componentselected{ "None Selected" };

		
		ImGui::Columns(2, "addCompo",false);
		ImGui::SetColumnWidth(0, 125.0f);
		ImGui::Text("Add Component");
		ImGui::NextColumn();
		//ImGui::CollapsingHeader(" Add Component", ImGuiTreeNodeFlags_Leaf);
		if (ImGui::BeginCombo("##Componenets", componentselected.c_str()))
		{
			for (int j = 0; j < componentList.size(); j++)
			{
				//bool is_selected = (componentselected == componentList[j]);
				if (ImGui::Selectable(componentList[j].c_str(), componentselected == componentList[j]))
				{
					AeonCore_INFO("Component selectable");
					componentselected = componentList[j];

					//ImGui::SetItemDefaultFocus();

				}
				if (componentselected == componentList[j])
				{
					ImGui::SetItemDefaultFocus();
					toAdd = true;
				}
				if (toAdd)
				{
					AeonCore_INFO("TO ADD COMPONENT");
					//if (ImGui::Button("Add Component", ImVec2{ ImGui::GetWindowContentRegionWidth(),25.0f }))
					//{
					AeonCore_INFO("Add Component");
			
					if (componentselected == "Font")
					{
						AeonCore_INFO("Add Font Component");
						std::filesystem::path working_dir(std::filesystem::current_path());
						auto parent_path = working_dir;
						glm::vec3 scaleValue = { 0.1f, 0.1f, 0.1f };
						if (!gCoordinatorPP.HasComponent<AeonCore::Render>(entityAdd) && !gCoordinatorPP.HasComponent<AeonCore::Font>(entityAdd)) {
							gCoordinatorPP.GetComponent<AeonCore::Transform>(entityAdd).SetScale(scaleValue);
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::Font(std::string(parent_path.string() + "/resources/fonts/OpenSans-Italic.ttf").c_str()));
							AeonCore::Font& fontCompo = gCoordinatorPP.GetComponent<AeonCore::Font>(entityAdd);
							fontCompo.SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
							AeonCore_INFO("Add Font Component");
						}
					}
					else if (componentselected == "Audio")
					{
						if (!gCoordinatorPP.HasComponent<AeonCore::AudioComponent>(entityAdd))
							
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::AudioComponent());
						AeonCore_INFO("Add audio Component");
						//gCoordinatorPP.AddComponent(entityAdd, Render(gCoordinatorPP))
					}
					///////////////////////////////////////////////////////////////////////////////////////////////////////////// ROY /////////////////////////////
					else if (componentselected == "AudioListener")
					{
						if (!gCoordinatorPP.HasComponent<AeonCore::AudioListenerComponent>(entityAdd))

							gCoordinatorPP.AddComponent(entityAdd, AeonCore::AudioListenerComponent());
						AeonCore_INFO("Add audio Listener Component");
						//gCoordinatorPP.AddComponent(entityAdd, Render(gCoordinatorPP))
					}
					else if (componentselected == "Behaviour")
					{
						if (!gCoordinatorPP.HasComponent<AeonCore::Behaviour>(entityAdd)) {
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::Behaviour(entityAdd));
							AeonCore_INFO("Add Behaviour Component");
						}
					}
					else if (componentselected == "Light") {
						if (!gCoordinatorPP.HasComponent<AeonCore::Lighting>(entityAdd)) {
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::Lighting(entityAdd));
							AeonCore_INFO("Add Light Component");
						}
					}
					else if (componentselected == "Render") {
						if (!gCoordinatorPP.HasComponent<AeonCore::Render>(entityAdd)&&!gCoordinatorPP.HasComponent<AeonCore::Font>(entityAdd)) {
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::Render(gCoordinatorPP.GetEntity(entityAdd)->GetEntityName(), entityAdd));

							AeonCore_INFO("Add Render Component");
						}
					}
					else if (componentselected == "UI")
					{
						if (!gCoordinatorPP.HasComponent<AeonCore::UI>(entityAdd)) {
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::UI(entityAdd));
							glm::vec3 uiScale = glm::vec3(600.0f, 350.f, 1.0f);
							gCoordinatorPP.GetComponent<AeonCore::Transform>(entityAdd).SetScale(uiScale);
							AeonCore_INFO("Add UI Component");
						}
					}
					else if (componentselected == "Rigid Body")
					{
						if (!gCoordinatorPP.HasComponent<AeonCore::RigidBody>(entityAdd)) {
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::RigidBody());
							/*auto sSystem = gCoordinatorPP.GetSystem<AeonCore::PhysicsSystem>();
							auto physSystem = std::dynamic_pointer_cast<AeonCore::PhysicsSystem>(sSystem);
							physSystem->AddToActorMap(entityAdd);*/
							AeonCore_INFO("Add Rigid Body Component");
						}
					}
					else if (componentselected == "Collider")
					{
						auto flags = !gCoordinatorPP.HasComponent<AeonCore::Collider>(entityAdd) && !gCoordinatorPP.HasComponent<AeonCore::Collider>(entityAdd) && !gCoordinatorPP.HasComponent<AeonCore::Collider>(entityAdd);
						if (flags) {
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::Collider());
							AeonCore_INFO("Add Collider");
						}
					}
					else if (componentselected == "Particles")
					{
						if (!gCoordinatorPP.HasComponent<AeonCore::Particles>(entityAdd)) {
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::Particles(entityAdd));
							AeonCore_INFO("Add Particle Component");
						}
					}

					
					else if (componentselected == "Camera")
					{
						if (!gCoordinatorPP.HasComponent<AeonCore::Camera>(entityAdd)&&!gCoordinatorPP.GetEntity(entityAdd)->IsChild()) {
							gCoordinatorPP.AddComponent(entityAdd, AeonCore::Camera());
							AeonCore_INFO("Add Camera Component");
						}
						if (gCoordinatorPP.GetEntity(entityAdd)->IsChild()) {
							AeonCore_ERROR("Camera Component cannot be added to child");
						}
					}

				}
			}

			ImGui::EndCombo();
		}
		ImGui::Columns(1,"addCompo");
	
	}
	void PropertiesPanel::CompressTexture(AeonCore::Render& renderComp,const char* payloadData, const std::string& textureType)
	{
		auto textureName = std::string(payloadData);
		std::filesystem::path path(payloadData);
		std::string fileExt = path.extension().string();
		int quality = 2048;
		std::string working_dir = std::filesystem::current_path().string();
		std::string fileLocation = working_dir + "\\resources\\textures";

		if (fileExt != ".dds") {

			std::string compilerTexturePath = working_dir + "\\texconv.exe";
			std::string command = compilerTexturePath + " \"" + fileLocation + "\\" + textureName + "\" -w " + std::to_string(quality) + " -h " + std::to_string(quality) + " -f BC3_UNORM -o \"" + fileLocation + "\"";
			system(command.c_str());
			ImGui::OpenPopup("Error Popup");
		}
		else {
			if (textureType == "Diffuse") {
				renderComp.SetDiffuseTexture(fileLocation + "\\" + textureName);
			}
			else if (textureType == "Specular") {
				renderComp.SetSpecularTexture(fileLocation + "\\" + textureName);
			}
		}

	}
	void PropertiesPanel::CompressTexture(AeonCore::UI& uiComp, const char* payloadData, const std::string& textureType)
	{
		auto textureName = std::string(payloadData);
		std::filesystem::path path(payloadData);
		std::string fileExt = path.extension().string();
		int quality = 2048;
		std::string working_dir = std::filesystem::current_path().string();
		std::string fileLocation = working_dir + "\\resources\\textures";

		if (fileExt != ".dds") {

			std::string compilerTexturePath = working_dir + "\\texconv.exe";
			std::string command = compilerTexturePath + " \"" + fileLocation + "\\" + textureName + "\" -w " + std::to_string(quality) + " -h " + std::to_string(quality) + " -f BC3_UNORM -o \"" + fileLocation + "\"";
			system(command.c_str());
			ImGui::OpenPopup("Error Popup");
		}
		else {
			uiComp.SetTexturePath(fileLocation + "\\" + textureName);
		}

	}
	void PropertiesPanel::CompressTexture(AeonCore::Particles& particlesComp, const char* payloadData, const std::string& textureType)
	{
		auto textureName = std::string(payloadData);
		std::filesystem::path path(payloadData);
		std::string fileExt = path.extension().string();
		int quality = 2048;
		std::string working_dir = std::filesystem::current_path().string();
		std::string fileLocation = working_dir + "\\resources\\textures";

		if (fileExt != ".dds") {

			std::string compilerTexturePath = working_dir + "\\texconv.exe";
			std::string command = compilerTexturePath + " \"" + fileLocation + "\\" + textureName + "\" -w " + std::to_string(quality) + " -h " + std::to_string(quality) + " -f BC3_UNORM -o \"" + fileLocation + "\"";
			system(command.c_str());
			ImGui::OpenPopup("Error Popup");
		}
		else {
			if (textureType == "Diffuse") {
				particlesComp.SetDiffuseTexture(fileLocation + "\\" + textureName);
			}
			else if (textureType == "Specular") {
				particlesComp.SetSpecularTexture(fileLocation + "\\" + textureName);
			}
		}
	}
	void PropertiesPanel::CompressObject(AeonCore::Render& renderComp, const char* payloadData)
	{
		auto modelName = std::string(payloadData);
		std::filesystem::path path(payloadData);
		std::string fileExt = path.extension().string();
		std::string working_dir = std::filesystem::current_path().string();
		std::string fileLocation = working_dir + "\\resources\\objects";

		if (fileExt != ".bin") {
			auto lowercaseDriver = std::tolower(static_cast<int>(working_dir[0]));
			std::string compilerTexturePath = std::string(1, lowercaseDriver) +working_dir.substr(2) + "\\Compiler.exe";
			std::string command = "wsl /mnt/" + compilerTexturePath+" -geom \""+fileLocation + "\\" +modelName+"\" -o \""+fileLocation+"\"";
			std::replace(command.begin(), command.end(), '\\', '/');
			system(command.c_str());
			ImGui::OpenPopup("Error Popup");

		}
		else {
			renderComp.SetModel(fileLocation + "\\" + modelName);
		}
	}
	void PropertiesPanel::CompressObject(AeonCore::Particles& particleComp, const char* payloadData)
	{
		auto modelName = std::string(payloadData);
		std::filesystem::path path(payloadData);
		std::string fileExt = path.extension().string();
		std::string working_dir = std::filesystem::current_path().string();
		std::string fileLocation = working_dir + "\\resources\\objects";

		if (fileExt != ".bin") {
			auto lowercaseDriver = std::tolower(working_dir[0]);
			std::string compilerTexturePath = std::string(1, lowercaseDriver) + working_dir.substr(2) + "\\Compiler.exe";
			std::string command = "wsl /mnt/" + compilerTexturePath + " -geom \"" + fileLocation + "\\" + modelName + "\" -o \"" + fileLocation + "\"";
			std::replace(command.begin(), command.end(), '\\', '/');
			system(command.c_str());
			ImGui::OpenPopup("Error Popup");

		}
		else {
			particleComp.SetModel(fileLocation + "\\" + modelName);
		}
	}
}

