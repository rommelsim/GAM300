#include "acpch.h"
/*!***************************************************************************
	\file			InputSystem.cpp
	\author			Lee Zhong En
	\par			zhongen.lee@digipen.edu
	\date			Oct 1, 2023
	\brief			This source file consists of function definitions of the
					InputSystemSystem class. The InputSystem detects and handles
					any Inputs given by the user.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "ECS/Systems/InputSystem.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Transform.h"
#include "Core/Log.h"
#include "Core/Application.h"

#include "Editor/EditorCamera.h"

#include "Camera/CameraManager.h"

namespace AeonCore
{
	bool camera_keystate = false;
	bool switch_keystate = false;
	float JoyStickSensitivity = 0.03f;

	void InputSystem::Init()
	{
		auto window = (GLFWwindow*)Application::GetApplication().GetWindow().GetNativeWindow();
		(void)window;

		//	CAllBACK FUNCTIONS
		{
			glfwSetJoystickCallback		(Input::joystick_callback);
			glfwSetMouseButtonCallback	(window, Input::mouseButtonCallback);
			glfwSetKeyCallback			(window, Input::keyCallback);
			glfwSetScrollCallback		(window, Input::mouseScrollCallback);
			//glfwSetCursorPosCallback	(window, Input::mouse_callback);
		}
		//Input::GetInstance().RegisterMapping("test_me", KeyMouseInput{ KBMInputType::Keyboard, AE_KEY_F }, JoyStickInput::Axes_Lstick_Axis_X);

		//	To register controllers already connected
		{
			for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; ++jid) {
				if (glfwJoystickPresent(jid)) {
					// Joystick is connected
					AeonCore_INFO("Controller already connected: ID {}", jid);

					Input& inputInst = Input::GetInstance();

					inputInst.m_IsJoyStickConnected = true;
					auto insertedPair = inputInst.mJoyStickInfoMap.insert({ jid, JoyStickInfo{jid} });

					JoyStickInfo& joyInfo = insertedPair.first->second;
					glfwGetJoystickButtons(jid, &(joyInfo.num_buttons));
					auto& joyTriggerVec = joyInfo.joystickButtonTrigger;
					joyTriggerVec.reserve(joyInfo.num_buttons);
					joyTriggerVec.insert(joyTriggerVec.begin(), joyInfo.num_buttons, KeyTriggerState::False);

					AeonCore_INFO("Controller is connected. {}, {}, {}", jid, inputInst.mJoyStickInfoMap.size(), joyInfo.num_buttons);

				}
			}
		}
		
	}

	void InputSystem::Update()
	{
		ZoneScopedN("Input Update");
		//put stuff to be calculated in the background here
		//i.e. button hold stuff like click and drag camera etc.
		Input::GetInstance().UpdateControllerMappings();
		Input::GetInstance().UpdateKeyTriggerState();

		////	CAMERA CONTROLS
		///*	Key
		// *	C : Enable Component Camera View
		// *	V : Switch between other component camera view
		// *
		// *	W : Move editor camera forward
		// *	A : Move editor camera backward
		// *	D : Move editor camera left
		// *	S : Move editor camera right
		// *
		// *	Mouse
		// *	Left click : Pan the camera
		// */
		{
			if (Input::IsKeyTriggered(AC_KEY_C))
			{
				//	Enable Component Camera
				//mEnableEntityCam = !mEnableEntityCam;
			}

			if (Input::IsKeyTriggered(AC_KEY_V))
			{
				//	Switch to another component Camera
				//mSwitchEntityCam = !mSwitchEntityCam;
			}

			/*if (Application::GetApplication().GetLoadStatus() == LoadStatus::NotRunning)*/ {
				Camera& cam = CameraManager::GetInstance().GetActiveCamera();

				if (cam.GetViewPortSelected())
				{
					if(cam.GetCameraRole() != CameraRole::CAMERA && Application::GetApplication().GetRuntimeState() != RuntimeState::Start)
					{
						mEditorCam_Forward = Input::IsKeyHeld(AC_KEY_W) ? true : false;
						mEditorCam_Left = Input::IsKeyHeld(AC_KEY_A) ? true : false;
						mEditorCam_Right = Input::IsKeyHeld(AC_KEY_D) ? true : false;
						mEditorCam_Backward = Input::IsKeyHeld(AC_KEY_S) ? true : false;
						mEditorCam_Up = Input::IsKeyHeld(AC_KEY_Z) ? true : false;
						mEditorCam_Down = Input::IsKeyHeld(AC_KEY_X) ? true : false;

						ProcessEditorCamera();
					}
				}
			}
		}

		//	Poll GLFW events
		{
			glfwPollEvents();
		}

		//for testing controller
		//Input& InputInst = Input::GetInstance();
		//auto& JoyStickInfoMap = InputInst.mJoyStickInfoMap;
		//if (!JoyStickInfoMap.empty()) {
		//	auto& JoyStickInfo = JoyStickInfoMap.begin()->second;
		//	//if (InputInst.GetMappingJoystickAxesValue("test_me", JoyStickInfo.joystickID))
		//	{
		//		AeonCore_INFO("Left Stick X value: {0}", InputInst.GetMappingJoystickAxesValue("test_me", JoyStickInfo.joystickID));
		//	}
		//}
	}

	void InputSystem::Destroy()
	{

	}

}

/*
* 		//Input::cameraCalculation();
		/*EditorCamera& editorCam = EditorCamera::GetInstance();
		CameraManager& camManager = CameraManager::GetInstance();
		Input& input = Input::GetInstance();*/


//archive stuff here
/*if (JoyStickInfo.buttons[0] == GLFW_PRESS)
			{
				std::cout << "A button on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[1] == GLFW_PRESS)
			{
				std::cout << "B button on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[2] == GLFW_PRESS)
			{
				std::cout << "X button on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[3] == GLFW_PRESS)
			{
				std::cout << "Y button on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[4] == GLFW_PRESS)
			{
				std::cout << "Left Bumper on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[5] == GLFW_PRESS)
			{
				std::cout << "Right Bumper on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[7] == GLFW_PRESS)
			{
				std::cout << "Start button on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[6] == GLFW_PRESS)
			{
				std::cout << "Back/select button on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[10] == GLFW_PRESS)
			{
				std::cout << "DPAD up on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[11] == GLFW_PRESS)
			{
				std::cout << "DPAD right on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[12] == GLFW_PRESS)
			{
				std::cout << "DPAD down on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[13] == GLFW_PRESS)
			{
				std::cout << "DPAD left on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[8] == GLFW_PRESS)
			{
				std::cout << "LeftStick on controller is pressed.\n";
			}
			if (JoyStickInfo.buttons[9] == GLFW_PRESS)
			{
				std::cout << "RightStick on controller is pressed.\n";
			}

//Entity* TestEntity = gCoordinator.GetEntityByName("Ship");
		//Entity* TestEntity2 = gCoordinator.GetEntityByName("Font");

		//if (TestEntity != nullptr && TestEntity2 != nullptr)
		//{

		//	Transform& EntityLocation = gCoordinator.GetComponent<Transform>(*TestEntity);
		//	Transform& EntityLocation2 = gCoordinator.GetComponent<Transform>(*TestEntity2);
		//
		//	Input& InputInst = Input::GetInstance();

		//	auto& JoyStickInfoMap = InputInst.mJoyStickInfo;

		//	if (!JoyStickInfoMap.empty())
		//	{
		//		//AeonCore_INFO("joystick info not empty");
		//		auto& JoyStickInfo = JoyStickInfoMap.begin()->second;

		//		glm::vec3 LeftJoyStickInput = EntityLocation.GetPosition();
		//		LeftJoyStickInput.x += JoyStickInfo.axesInput[0] * JoyStickSensitivity;
		//		LeftJoyStickInput.y += 0.0f;
		//		LeftJoyStickInput.z += JoyStickInfo.axesInput[1] * -JoyStickSensitivity;
		//
		//		EntityLocation.SetPosition(LeftJoyStickInput);

		//		if (JoyStickInfoMap.size() > 1)
		//		{
		//			auto& JoyStickInfo2 = (++JoyStickInfoMap.begin())->second;

		//			glm::vec3 LeftJoyStick2Input = EntityLocation2.GetPosition();
		//			LeftJoyStick2Input.x += JoyStickInfo2.axesInput[0] * JoyStickSensitivity;
		//			LeftJoyStick2Input.y += 0.0f;
		//			LeftJoyStick2Input.z += JoyStickInfo2.axesInput[1] * -JoyStickSensitivity;

		//			EntityLocation2.SetPosition(LeftJoyStick2Input);
		//		}
		//	}


		//for (auto& entity : gCoordinator.GetEntities())
		//{
		//	bool keystate = false;
		//	if (Input::IsKeyHeld(GLFW_KEY_C))
		//		keystate = !keystate;

		//	if (keystate)
		//	{
		//		//	component camera enable

		//
		//	}
		//	else
		//	{
		//		//	revert to editor camera
		//	}

		//	//if (gCoordinator.HasComponent<Camera>(entity))
		//	//{
		//	//	//	COMPONENT CAMERA
		//	//	Transform& trans = gCoordinator.GetComponent<Transform>(entity);
		//	//	Camera& camera = gCoordinator.GetComponent<Camera>(entity);
		//	//	bool keystate = false;
		//	//	if (Input::IsKeyHeld(GLFW_KEY_C))
		//	//	{
		//	//		AeonCore_INFO("Enable Object Camera...");
		//	//		keystate = !keystate;
		//	//	}

		//	//	if (keystate)
		//	//	{
		//	//		swwwwe
		//	//		//	Component Camera Enabled
		//	//		static bool firstMouse = true;
		//	//		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
		//	//		{
		//	//
		//	//			//glm::vec2 offset = Input::GetInstance().UpdateOffSet(firstMouse);
		//	//			float xpos = Input::GetMouseX();
		//	//			float ypos = Input::GetMouseY();
		//	//			static float lastx = 0.0f;
		//	//			static float lasty = 0.0f;

		//	//			if (firstMouse)
		//	//			{
		//	//				lastx = xpos;
		//	//				lasty = ypos;
		//	//				firstMouse = false;
		//	//			}

		//	//			float xoffset = xpos - lastx;
		//	//			float yoffset = lasty - ypos; // reversed since y-coordinates go from bottom to top

		//	//			lastx = xpos;
		//	//			lasty = ypos;

		//	//			Input::GetInstance().ProcessMouseMovement(camera, xoffset, yoffset, trans.GetPosition());
		//	//		}

		//	//		if (Input::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1))
		//	//			firstMouse = true;

		//	//	}
		//	//	else
		//	//	{
		//	//		camera.SetEnabled(false);

		//	//		//	EDITOR CAMERA
		//	//		EditorCamera& editorCam = EditorCamera::GetInstance();
		//	//		Input& input = Input::GetInstance();
		//	//		TimerStep& timer = TimerStep::GetInstance();

		//	//		if (!editorCam.GetIsHovered())
		//	//			continue;

		//	//		if (Input::IsKeyHeld(GLFW_KEY_W))	input.ProcessCameraKeyboard(editorCam, CameraMovement::FORWARD, timer.get_delta_time());
		//	//		if (Input::IsKeyHeld(GLFW_KEY_A))	input.ProcessCameraKeyboard(editorCam, CameraMovement::LEFT, timer.get_delta_time());
		//	//		if (Input::IsKeyHeld(GLFW_KEY_S))	input.ProcessCameraKeyboard(editorCam, CameraMovement::BACKWARD, timer.get_delta_time());
		//	//		if (Input::IsKeyHeld(GLFW_KEY_D))	input.ProcessCameraKeyboard(editorCam, CameraMovement::RIGHT, timer.get_delta_time());

		//	//		input.mouse_callback();
		//	//
		//	//	}
		//	//}
		//	//else
		//	//{
		//	//	//	EDITOR CAMERA
		//	//	EditorCamera& editorCam = EditorCamera::GetInstance();
		//	//	Input& input = Input::GetInstance();
		//	//	TimerStep& timer = TimerStep::GetInstance();

		//	//	if (!editorCam.GetIsHovered())
		//	//		continue;

		//	//	if (Input::IsKeyHeld(GLFW_KEY_W))	input.ProcessCameraKeyboard(editorCam, CameraMovement::FORWARD,	timer.get_delta_time());
		//	//	if (Input::IsKeyHeld(GLFW_KEY_A))	input.ProcessCameraKeyboard(editorCam, CameraMovement::LEFT,		timer.get_delta_time());
		//	//	if (Input::IsKeyHeld(GLFW_KEY_S))	input.ProcessCameraKeyboard(editorCam, CameraMovement::BACKWARD, timer.get_delta_time());
		//	//	if (Input::IsKeyHeld(GLFW_KEY_D))	input.ProcessCameraKeyboard(editorCam, CameraMovement::RIGHT,	timer.get_delta_time());

		//	//	input.mouse_callback();
		//	//}
		//}
*/
