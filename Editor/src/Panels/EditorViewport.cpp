/*!***************************************************************************
	\file			EditorViewport.cpp
	\author			Rommel, Lee Zhong En, Chew Shi Ru
	\par			shiru.chew\@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions for the editor
					viewport
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
//Editor
#include "EditorViewport.h"
//External Lib
#include <imgui.h>
#include <ImGuizmo.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
//Engine
#include <AeonCore.h>
#include <Input/Input.h>
#include <Rendering/Rendering.h>
#include <ECS/Components/Transform.h>
#include <ECS/Components/Camera.h>
#include <ECS/Components/Collider.h>
#include <ECS/Components/RigidBody.h>
#include <Core/Log.h>
#include <Serialization/Serialization.hpp>
#include <Rendering/Framebuffer/FramebufferManager.h>

#include <ECS/Systems/Renderer2DSystem.h>

#include <ECS/Components/Particle.h>

#include "Camera/CameraManager.h"
#include "Physics/ACPhysicsInternal.hpp"
#include "Physics/ACPhysicsUtility.hpp"

namespace Editor {

	inline glm::vec2 GetMousePosNormalisedVP(glm::vec2 min, glm::vec2 max, glm::vec2 centerpos, glm::vec2 mousepos)
	{
		glm::vec2 minBound{ min.x + centerpos.x, min.y + centerpos.y };
		glm::vec2 maxBound{ max.x + centerpos.x, max.y + centerpos.y };

		mousepos.x -= minBound.x;
		mousepos.y -= minBound.y;

		glm::vec2 viewportSize = maxBound - minBound;
		mousepos.y = viewportSize.y - mousepos.y;

		glm::vec2 coords{ ((2.f * mousepos.x) / viewportSize.x) - 1.0f, ((2.0f * mousepos.y) / viewportSize.y) - 1.0f };

		return { (1 + coords.x) * 0.5, (1 + coords.y) * 0.5 };
	}

	EditorViewport::EditorViewport() {
		Init();
	}

	EditorViewport::~EditorViewport()
	{
		ShutDown();
	}

	glm::vec3 EditorViewport::ConvertViewportToNDC(int mx, int my, int width, int height)
	{
		float x = (2 * mx) / width - 1.0f;
		float y = (2 * my) / height + 1.0f;
		float z = 1.0f;

		// Return Ray_NDC
		return glm::vec3(x, y, z);
	}
	glm::vec4 EditorViewport::ConvertNDCToClip(glm::vec3 ray_nds)
	{
		// Return Ray_Clip
		return glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 0.0f);
	}

	glm::vec4 EditorViewport::ConvertClipToEye(glm::mat4 projection, glm::vec4 ray_clip)
	{
		glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

		// Return Ray_Eye
		return ray_eye;
	}
	glm::vec3 EditorViewport::ConvertEyeToWorld(glm::mat4 view, glm::vec3 ray_eye)
	{
		glm::vec3 ray_world = glm::inverse(view) * glm::vec4(ray_eye, 1.0f);
		ray_world = glm::normalize(ray_world);

		// Return Ray_world
		return ray_world;
	}

	void EditorViewport::Init()
	{
		AeonCore::Application& app = AeonCore::Application::GetApplication();
		m_window = &app.GetWindow();

	}
	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	void EditorViewport::ShowViewport(AeonCore::EntityID entity)
	{
		// TODO: Check for user input
		// if cursor over imgui window
		//	activate camera
		// else
		//	dont use camera
		// ImGui::Begin("GameRender");


		// we get the screen position of the window
		ImGui::Begin("Render VP", NULL, ImGuiWindowFlags_NoMove); //mouse checking should be inside here for camera
		ImGuizmo::BeginFrame();

		auto viewportOffset = glm::vec2(0, 0);// ImGui::GetCursorPos();		// inlucdes tab bar

		AeonCore::Camera& cam = AeonCore::CameraManager::GetInstance().GetActiveCamera();
		//AeonCore::EditorCamera& editCam = AeonCore::EditorCamera::GetInstance();

		cam.SetViewPortSelected(ImGui::IsWindowFocused() /*&& !ImGuizmo::IsUsing()*/);

		// Calculate the aspect ratio of the image and the content region
		auto viewportSize = ImGui::GetContentRegionAvail();
		float imageAspectRatio = (float)1920 / (float)1080;
		float contentRegionAspectRatio = viewportSize.x / viewportSize.y;
		static float imageWidth = 1920, imageHeight = 1080;

		//AeonCore::FramebufferManager::GetInstance().RescaleFramebuffer("f1", (int)viewportSize.x, (int)viewportSize.y);
		//AeonCore::FramebufferManager::GetInstance().BindFramebuffer("f1");
		//auto fb = AeonCore::FramebufferManager::GetInstance().GetFramebuffer("f1");
		auto id = AeonCore::FramebufferManager::GetInstance().GetFrameTexture("f1");

		// External side need to reset to default framebuffer 0 for opengl. Otherwise, won't render IMGUI + Engine stuff!
		// AeonCore::FramebufferManager::GetInstance().ResetToDefaultFrameBufferOpenGL();
		
		// Can use this function to check whats the current framebuffer ID attached to current engine. Default framebuffer is 0!
		// AeonCore::FramebufferManager::GetInstance().GetCurrentFrameBufferAttachedToOpenGL();
		
		//AeonCore_INFO("{0}", AeonCore::FramebufferManager::GetInstance().GetCurrentFrameBufferAttachedToOpenGL());
		// // Calculate the aspect ratio based on the desired aspect ratio
		// float aspectRatio = 16.0f / 9.0f; // Change this to your desired aspect ratio
		//
		// // Calculate the new dimensions while maintaining the aspect ratio
		// float newWidth, newHeight;
		// float windowWidth = ImGui::GetWindowWidth();
		// float windowHeight = ImGui::GetWindowHeight();
		//
		// if (windowWidth / windowHeight > aspectRatio)
		// {
		// 	newWidth = windowHeight * aspectRatio;
		// 	newHeight = windowHeight;
		// }
		// else
		// {
		// 	newWidth = windowWidth;
		// 	newHeight = windowWidth / aspectRatio;
		// }
		//
		// /*newHeight = windowHeight;
		// newWidth = windowWidth;*/
		//
		//
		// // Calculate the position to center the image
		// float posX = (windowWidth - newWidth) * 0.5f;
		// float posY = (windowHeight - newHeight) * 0.5f;
		//
		// // Set the dimensions and position for the image
		// ImVec2 imageSize = ImVec2(newWidth, newHeight);
		// ImVec2 imagePosition = ImVec2(posX, posY);
		//
		// // Once camera is updated with the new apsect ratio, the new projection matrix is genrerated.
		// editCam.SetWindowWidthAndHeight(newWidth, newHeight);
		//
		// // Display the image with the calculated dimensions and position
		// ImGui::SetCursorPos(imagePosition);
		//
		// ImGui::Image((ImTextureID)static_cast<uint64_t>(id), imageSize, ImVec2(0, 1), ImVec2(1, 0));


		// Scale the image horizontally if the content region is wider than the image
		if (contentRegionAspectRatio > imageAspectRatio)
		{
			imageWidth = viewportSize.y * imageAspectRatio;

			float xPadding = (viewportSize.x - imageWidth) / 2;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + xPadding);
			ImGui::Image((ImTextureID)(intptr_t)id, ImVec2(imageWidth, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		}
		// Scale the image vertically if the content region is taller than the image
		else
		{
			imageHeight = viewportSize.x / imageAspectRatio;

			float yPadding = (viewportSize.y - imageHeight) / 2;
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yPadding);
			ImGui::Image((ImTextureID)(intptr_t)id, ImVec2(viewportSize.x, imageHeight), ImVec2(0, 1), ImVec2(1, 0));
		}

		auto vpMin = ImGui::GetWindowContentRegionMin();
		auto vpMax = ImGui::GetWindowContentRegionMax();
		auto vpOffset = ImGui::GetWindowPos();

		auto vpMouse = ImGui::GetMousePos();
		auto vpCursor = ImGui::GetCursorScreenPos();
		glm::vec2 vpPos = { vpMouse.x - vpCursor.x, -(vpMouse.y - vpCursor.y) };

		glm::vec2 bounding[2]{};

		bounding[0] = { vpMin.x + vpOffset.x, vpMin.y + vpOffset.y };
		bounding[1] = { vpMax.x + vpOffset.x, vpMax.y + vpOffset.y };

		ImVec2 mouse = ImGui::GetMousePos();
		glm::vec2 screenSpaceMousePos = { AeonCore::Input::GetMouseX(), AeonCore::Input::GetMouseY() };

		glm::vec2 minBound{ vpMin.x, vpMin.y };
		glm::vec2 maxBound{ vpMax.x, vpMax.y };
		glm::vec2 offset{ vpOffset.x, vpOffset.y };
		glm::vec2 mousemouse{ mouse.x, mouse.y };
		glm::vec3 mousePos = glm::vec3(GetMousePosNormalisedVP(minBound, maxBound, offset, mousemouse), 1.0f);

		glm::vec3 mousePos_VP = { mousePos.x * viewportSize.x, mousePos.y * viewportSize.y, 1.0f };


		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();

		

		AeonCore::EntityID pixel = static_cast<AeonCore::EntityID>(AeonCore::FramebufferManager::GetInstance().ReadPixel("f1", 1, (int)mousePos_VP.x, (int)mousePos_VP.y));
		// AeonCore_INFO("Mouse : {}, {}", mousePos_VP.x, mousePos_VP.y);
		// AeonCore_WARN("Pixel : {}", GetPixelID());
		SetPixelID(pixel);
		



//		/////////////////////////////////////////////////////// PHYSICS DEBUG DRAW HI ROMMEL ///////////////////////////////////////////
//#if _DEBUG
//		auto sceneTemp = &AeonCore::ACPhysics::PhysicsInternal::GetInstance().GetPhysicsScene();
//
//		sceneTemp->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
//		sceneTemp->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
//
//		//Camera &cam = CameraManager::GetInstance().GetActiveCamera();
//
//		const physx::PxRenderBuffer &rb = sceneTemp->getRenderBuffer();
//		for ( physx::PxU32 i = 0; i < rb.getNbLines(); i++ )
//		{
//			const physx::PxDebugLine &line = rb.getLines()[i];
//			// render the line
//
//			glm::vec3 grey(0.71f, 0.71f, 0.71f);
//
//			if(AeonCore::Application::GetApplication().GetRuntimeState() == AeonCore::RuntimeState::Start)
//			{
//				AeonCore::Rendering::GetInstance().DrawLine(
//					AeonCore::ACPhysicsUtility::FromPhysXVector(line.pos0), AeonCore::ACPhysicsUtility::FromPhysXVector(line.pos1), grey, cam);
//			}
//
//		}
//		/////////////////////////////////////////////////////// PHYSICS DEBUG DRAW HI ROMMEL ///////////////////////////////////////////
//#endif

		// Unbind framebuffer
	    //AeonCore::FramebufferManager::GetInstance().UnbindFramebuffer("f1");


		for (auto& e : gCoordinator.GetEntities()) {
			(void)e;
			if (gCoordinator.EntityExists(entity))
			{
				AeonCore::Transform& transform = gCoordinator.GetComponent<AeonCore::Transform>(entity);
				static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);
				static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);
				if (ImGui::IsWindowFocused())
				{
					if (AeonCore::Input::IsKeyTriggered(AC_KEY_Q))
						currentGizmoOperation = ImGuizmo::TRANSLATE;
					/*	if (AeonCore::Input::IsKeyTriggered(AE_KEY_E))
							currentGizmoOperation = ImGuizmo::ROTATE;*/
					if (AeonCore::Input::IsKeyTriggered(AC_KEY_R)) // r Key
						currentGizmoOperation = ImGuizmo::SCALE;
				}

				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				//float size = 100.f;
				// ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - viewportPanelSize.y), viewportPanelSize.x, viewportPanelSize.y);
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
				//ImGuizmo::SetGizmoSizeClipSpace(1.f / (viewportPanelSize.x / size));

				glm::mat4 camProjection = glm::perspective(glm::radians(cam.GetCameraZoom()), (16.0f / 9.f), 0.1f, 100.0f);

				//glm::mat4 camProjection = glm::ortho(0.0, static_cast<double>(windowWidth), -1.0, static_cast<double>(windowHeight));
				glm::mat4 matrix = transform.GetTransformMatrix(); // Assuming you have a function to get the transformation matrix
				///glm::mat4 camProj =
				ImGuizmo::Manipulate(glm::value_ptr(cam.GetCameraViewMatrix()), glm::value_ptr(cam.GetCameraProjectionMatrix()), currentGizmoOperation, currentGizmoMode, glm::value_ptr(matrix), nullptr);

				if (ImGuizmo::IsUsing()||ImGuizmo::IsOver()) {
					float matrixTranslation[3], matrixRotation[3], matrixScale[3];
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matrix), matrixTranslation, matrixRotation, matrixScale);
					glm::vec3 newPosition = glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
					transform.SetPosition(newPosition);

					glm::vec3 newScale = glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]); //x is sus
					transform.SetScale(newScale);
				}

			}
		}
		AeonCore::EntityID getPixel = GetPixelID();
		if (!ImGuizmo::IsUsing()) {
			PixelPicking(getPixel);
		}

		//target create model
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MOVING_ITEM"))
			{
				//AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();
				auto modelData = (const char*)payload->Data;
				std::string modelName = modelData;
				std::filesystem::path working_dir(std::filesystem::current_path());
				auto parent_path = working_dir.string();
				////create new entity
				AeonCore::EntityID e0 = gCoordinator.CreateEntity().GetEntityID(); //by giving the entity a name you can get the entity by name too
				////add transform
				gCoordinator.AddComponent(e0, AeonCore::Transform(glm::vec3(1, 1, 1)));
				////add render
				gCoordinator.AddComponent(e0, AeonCore::Render(parent_path + "/resources/objects/" + modelName, gCoordinator.GetEntity(e0)->GetEntityName(), e0));

				//auto& render = gCoordinator.GetComponent<AeonCore::Render>(e0);
				AeonCore_INFO("Drop Drag {} succeesful", modelName);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_SCENE_ITEM"))
			{
				auto sceneData = (const char*)payload->Data;
				std::string sceneName = sceneData;
				std::filesystem::path working_dir(std::filesystem::current_path());
				auto parent_path = working_dir.string();
				std::string filePath = parent_path + "/resources/scene/" + sceneName;

        		gCoordinator.EmptyActiveScene();
        		AeonCore::SceneManager::LoadScene(filePath);

				AeonCore_INFO("Drop Drag {} succeesful", sceneName);
				AeonCore_WARN("Drop Drag {} succeesful", parent_path + "/resources/scene" + sceneName);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::End();


	}

	void Editor::EditorViewport::ShutDown()
	{
		//delete editorvp_camera;
	}
	bool EditorViewport::PixelPicking(AeonCore::EntityID entityOnlyID)
	{
		static bool ableToSelectEntity = true;
		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();


		if (AeonCore::Input::IsMouseButtonHeld((GLFW_MOUSE_BUTTON_1)) &&ImGui::IsWindowFocused()) {
			AeonCore::EntityID entityID = AeonCore::ECSUtils::EncodeSceneID(entityOnlyID, AeonCore::SceneManager::GetInstance().GetActiveScene()->GetSceneID());
			if (gCoordinator.EntityExists(entityID)) {
				EditorLayer::SetEntityID(entityID);
				SceneHierarchyPanel::SetSelectedEntityID(entityID);
				ableToSelectEntity = false;
				return true;
			}
			else {
				ableToSelectEntity = true;

			}
			return true;
		}

		EditorLayer::SetEntityID(0);
		return false;
	}
}



// Archive

// glm::vec4 mouse_world = glm::inverse(editCam.GetCameraProjectionMatrix()) * (glm::inverse(editCam.GetCameraViewMatrix()) * glm::vec4(mousePos, 1.0f));
// glm::vec4 mouse_clip = ConvertNDCToClip(mousePos);
// glm::vec4 mouse_eye = ConvertClipToEye(editCam.GetCameraProjectionMatrix(), mouse_clip);
// glm::vec3 mouse_world = ConvertEyeToWorld(editCam.GetCameraViewMatrix(), mouse_eye);

// 	// View
// 	glm::mat4 camView = editCam.GetCameraViewMatrix();
//
// 	// Projection
// 	glm::mat4 camProjection = glm::perspective(glm::radians(editCam.GetCameraZoom()), newWidth / newHeight, 0.1f, 100.0f);
//
// 	// Convert viewport mouse position to NDC
// 	glm::vec3 ray_ndc = this->ConvertViewportToNDC(mouseX, mouseY, viewportSize.x, viewportSize.y);
//
// 	// Convert NDC to Clip
// 	glm::vec4 ray_clip = this->ConvertNDCToClip(ray_ndc);
//
// 	// Convert Clip to Eye
// 	glm::vec4 ray_eye = this->ConvertClipToEye(camProjection, ray_clip);
//
// 	// Convert Eye to World
// 	glm::vec3 ray_world = this->ConvertEyeToWorld(camView, ray_eye);
//
// }

// ---- EQ demo of using Runtime ----
/*AeonCore::Application& engineApp = AeonCore::Application::GetApplication();

ImGui::BeginDisabled(engineApp.GetRuntimeState() == AeonCore::RuntimeState::Run);
if (ImGui::Button("Play")) {
	AeonCore::Application::GetApplication().OnRuntimeStart();
}
ImGui::EndDisabled();

ImGui::BeginDisabled(engineApp.GetRuntimeState() != AeonCore::RuntimeState::Run); // as long as state is either pause or stop, disable. Equivalent condition is if state is not Run
if (ImGui::Button("Pause")) {
	AeonCore::Application::GetApplication().OnRuntimePause();
}
ImGui::EndDisabled();

ImGui::BeginDisabled(engineApp.GetRuntimeState() == AeonCore::RuntimeState::Stop);
if (ImGui::Button("Stop")) {
	AeonCore::Application::GetApplication().OnRuntimeStop();
}
ImGui::EndDisabled();*/

