#pragma once

/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	InputSystem.h
 * authors:
		Lee Zhong En				(zhongen.lee@digipen.edu)
 *
 * brief:	This file contains the functions definitions for the inputsystem
 *
 *
 * Copyright (C) 2023 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the
   prior written consent of DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------*/
#include "Camera/CameraManager.h"
#include "ECS/Architecture/System.hpp"
#include "ECS/Architecture/Coordinator.hpp"

#include "Input/Input.h"
#include "Window/Window.h"
#include "Core/Application.h"

namespace AeonCore
{
	//bool mouseleftClickHold;
	class InputSystem : public System
	{
	public:
		InputSystem() = default;

		void Init() override;

		void Update() override;

		void Destroy() override;

		void ProcessComponentCamera()
		{
			CameraManager& camManager = CameraManager::GetInstance();
			EditorCamera& editorCam = EditorCamera::GetInstance();		(void)editorCam;
			Coordinator& gCoordinator = Coordinator::GetInstance();
			Input& input = Input::GetInstance();

			//	Controls for component camera
			Camera* cam = &camManager.GetActiveCamera();

			if (camManager.GetEnableEntityCam())
			{
				Transform& pos = gCoordinator.GetComponent<Transform>(camManager.GetActiveCameraID());

				static bool firstMouse = true;
				float xpos = Input::GetMouseX();
				float ypos = Input::GetMouseY();
				float xoffset = 0.f;
				float yoffset = 0.f;

				if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
				{
					//	moving component camera
					if (firstMouse)
					{
						lastX = xpos;
						lastY = ypos;
						firstMouse = false;
					}

					xoffset = xpos - lastX;
					yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

					lastX = xpos;
					lastY = ypos;
				}
				else
				{
					lastX = xpos;
					lastY = ypos;
				}

				input.ProcessMouseMovement(*cam, xoffset, yoffset, pos.GetPosition());

				float fov = cam->GetCameraZoom();
				float zoomIncr = input.GetScrollY() * 5.f;

				fov -= zoomIncr;
				input.SetScrollY(0);

				if (fov < 5.0f)
					fov = 5.0f;
				if (fov > 100.f)
					fov = 100.f;

				cam->SetCameraZoom(fov);

				if (gCoordinator.EntityExists(camManager.GetActiveCameraID()))
				{

					



					//if (camManager.GetEnableEntityCam())
					//{
					//	//auto cameraList = camManager.GetCameraList();

					//	//for (auto it = cameraList.begin(); it != cameraList.end();)
					//	//{
					//	//	if (camManager.GetSwitchEntityCam())
					//	//	{
					//	//		//	switch camera view (if more than 1 camera component
					//	//		++it;

					//	//		// Check if the iterator has reached the end
					//	//		if (it == cameraList.end())
					//	//			it = cameraList.begin();	// If it has, loop back to the beginning
					//	//	}

					//	//	camManager.SetActiveCamera(it->second);
					//	//	camTransform = &gCoordinator.GetComponent<Transform>(it->first);	//	retrieve transfrom component

					//	//	//AeonCore_INFO("{0} is active camera", gCoordinator.GetEntity(it->first)->GetEntityName());
					//	//	break;
					//	//}

					//	

					//		if (cam->GetViewPortSelected())
					//		{
					//			
					//		}
					//	}
				}
			}
		}

		void ProcessEditorCamera()
		{
			CameraManager& camManager = CameraManager::GetInstance();
			EditorCamera& editorCam = EditorCamera::GetInstance();
			Input& input = Input::GetInstance();

			TimerStep& timer = TimerStep::GetInstance();
			CameraMovement movement = CameraMovement::STOP;

			if (mEditorCam_Forward)		movement = CameraMovement::FORWARD;
			if (mEditorCam_Left)		movement = CameraMovement::LEFT;
			if (mEditorCam_Right)		movement = CameraMovement::RIGHT;
			if (mEditorCam_Backward)	movement = CameraMovement::BACKWARD;
			if (mEditorCam_Up)			movement = CameraMovement::UP;
			if (mEditorCam_Down)		movement = CameraMovement::DOWN;

			input.ProcessCameraKeyboard(editorCam, movement, timer.get_delta_time());

			input.mouse_callback(lastX, lastY);

			float fov = editorCam.GetCameraZoom();
			float zoomIncr = input.GetScrollY() * 5.f;

			fov -= zoomIncr;
			input.SetScrollY(0);

			if (fov < 5.0f)
				fov = 5.0f;
			if (fov > 100.f)
				fov = 100.f;

			editorCam.SetCameraZoom(fov);

			if (!camManager.GetEnableEntityCam())
			{
				//	revert to editor camera
				/*camManager.SetActiveCamera(&editorCam);
				if (editorCam.GetViewPortSelected())
				{
					
				}*/

				

			}
		}

	private:
		inline static bool mEditorCam_Forward = false;
		inline static bool mEditorCam_Left = false;
		inline static bool mEditorCam_Right = false;
		inline static bool mEditorCam_Backward = false;
		inline static bool mEditorCam_Up = false;
		inline static bool mEditorCam_Down = false;


		inline static float lastX = 0.f;
		inline static float lastY = 0.f;
	};
}
