#include "acpch.h"
/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	Input.cpp
 * authors:
		Lee Zhong En				(zhongen.lee@digipen.edu)
 *
 * brief:	This file contains the function definition for the input class
 *
 *
 * Copyright (C) 2023 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the
   prior written consent of DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------*/
#include "Input/Input.h"
#include "Rendering/Rendering.h"
#include <iostream>
#include "Window/Window.h"
#include "Core/Application.h"
#include "Editor/EditorCamera.h"



namespace AeonCore {
	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will update the controller values to the parameters.
	*/
	/******************************************************************************/
	void Input::UpdateControllerMappings() 
	{
		for (auto& JoystickPair : mJoyStickInfoMap)
		{
			int JotstickID = JoystickPair.first;
			if (!glfwJoystickPresent(JoystickPair.first))
			{
				break;
			}
			if (glfwJoystickIsGamepad(JotstickID))
			{
				JoyStickInfo& GamePad1 = JoystickPair.second;
				GamePad1.axes = glfwGetJoystickAxes(JotstickID, &(GamePad1.num_axes));
				GamePad1.buttons = glfwGetJoystickButtons(JotstickID, &(GamePad1.num_buttons));
				if (GamePad1.axes == nullptr)
				{
					continue;
				}

				for (int i = 0; i < 6; i++)
				{
					GamePad1.axesInput[i] = GamePad1.axes[i];
				}

				if (GamePad1.axesInput[0] <= 0.08f && GamePad1.axesInput[0] >= -0.08f)
				{
					GamePad1.axesInput[0] = 0;
				}

				if (GamePad1.axesInput[1] <= 0.15f && GamePad1.axesInput[1] >= -0.15f)
				{
					GamePad1.axesInput[1] = 0;
				}

				if (GamePad1.joystickButtonTrigger.size() >= GamePad1.num_buttons) {
					for (int i = 0; i < GamePad1.num_buttons; i++)
					{
						KeyTriggerState& buttonState = GamePad1.joystickButtonTrigger[i];
						if (GamePad1.buttons[i] == GLFW_PRESS)
						{
							if (buttonState == KeyTriggerState::False)
								buttonState = KeyTriggerState::JustAdded;
							else if (buttonState == KeyTriggerState::JustAdded)
								buttonState = KeyTriggerState::True;
						}
						else
						{
							buttonState = KeyTriggerState::False;
						}
					}
				}
			}
		}
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will check whether controller is connected
	*/
	/******************************************************************************/
	bool Input::IsJoyStickConnected(int joystickId)
	{
		//return GetInstance()->m_IsJoyStickConnected;
		//glfwJoystickIsGamepad(GLFW_JOYSTICK_1);
		return glfwJoystickIsGamepad(joystickId);
	}


	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will return whether a key is pressed.
	\param keycode
		THis parameter contains the keycode for the key pressed
	\return
		This function returns 1 if pressed, 0 if not pressed
	*/
	/******************************************************************************/
	bool Input::IsKeyHeld(int keycode)
	{
		auto window = (GLFWwindow*)Application::GetApplication().GetWindow().GetNativeWindow();

		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will return whether mouse button is pressed.
	\param keycode
		THis parameter contains the mouse button
	\return
		This function returns 1 if pressed, 0 if not pressed
	*/
	/******************************************************************************/
	bool Input::IsMouseButtonPressed(int _button)
	{
		auto window = static_cast<GLFWwindow*> (Application::GetApplication().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, _button);

		return state == GLFW_PRESS;
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will return whether mouse button is held down.
	\param keycode
		THis parameter contains the mouse button
	\return
		This function returns 1 if pressed, 0 if not pressed
	*/
	/******************************************************************************/
	bool Input::IsMouseButtonHeld(int _button)
	{
		auto window = static_cast<GLFWwindow*> (Application::GetApplication().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, _button);

		return state == GLFW_PRESS;
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will return whether mouse button is released.
	\param keycode
		THis parameter contains the mouse button
	\return
		This function returns 1 if released, 0 if held down
	*/
	/******************************************************************************/
	bool Input::IsMouseButtonReleased(int _button)
	{
		auto window = static_cast<GLFWwindow*> (Application::GetApplication().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, _button);

		if (state == GLFW_RELEASE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will return the X value of mouse's position
	\return
		This function returns the mouse's X position
	*/
	/******************************************************************************/
	float Input::GetMouseX() {
		auto window = static_cast<GLFWwindow*> (Application::GetApplication().GetWindow().GetNativeWindow());

		double xPos{}, yPos{};
		glfwGetCursorPos(window, &xPos, &yPos);

		return static_cast<float>(xPos);
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will return the Y value of mouse's position
	\return
		This function returns the mouse's Y position
	*/
	/******************************************************************************/
	float Input::GetMouseY() {
		auto window = static_cast<GLFWwindow*> (Application::GetApplication().GetWindow().GetNativeWindow());

		double xPos{}, yPos{};
		glfwGetCursorPos(window, &xPos, &yPos);

		return static_cast<float>(yPos);
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will pair the mouse's X and Y position
	\return
		This function returns the mouse''s Y position's X and Y value
	*/
	/******************************************************************************/
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*> (Application::GetApplication().GetWindow().GetNativeWindow());

		double xPos{}, yPos{};
		glfwGetCursorPos(window, &xPos, &yPos);

		return std::pair<float, float>((float)xPos, (float)yPos);
	}


	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will callback in the event if controller is connected or
		disconnected.
	\param jid
		This parameter contains event code
	\param event
		This parameter contains the event, whether is it connected or disconnected.
	*/
	/******************************************************************************/
	void Input::joystick_callback(int jid, int event)
	{
		Input& inputInst = GetInstance();

		if (event == GLFW_CONNECTED)
		{
			//std::cout << "jid: " << jid << std::endl;

			inputInst.m_IsJoyStickConnected = true;
			auto insertedPair = inputInst.mJoyStickInfoMap.insert({ jid, JoyStickInfo{jid} });
			
			JoyStickInfo& joyInfo = insertedPair.first->second;
			glfwGetJoystickButtons(jid, &(joyInfo.num_buttons));
			auto& joyTriggerVec = joyInfo.joystickButtonTrigger;
			joyTriggerVec.reserve(joyInfo.num_buttons);
			joyTriggerVec.insert(joyTriggerVec.begin(), joyInfo.num_buttons, KeyTriggerState::False);

			AeonCore_INFO("Controller is connected. {}, {}, {}", jid, inputInst.mJoyStickInfoMap.size(), joyInfo.num_buttons );
		}
		else if (event == GLFW_DISCONNECTED)
		{
			inputInst.m_IsJoyStickConnected = false;
			inputInst.mJoyStickInfoMap.erase(jid);
			AeonCore_INFO("Controller is disconnect. {0}", inputInst.mJoyStickInfoMap.size());
		}
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will callback for the mouse actions
	\param GLFWindow*
	This parameter contains the pointer to the GLFW window
	\param button
		This parameter contains the button id
	\param action
		This parameter contains the action whether is it pressed or released.
	*/
	/******************************************************************************/
	void Input::mouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/)
	{
		if (action == GLFW_RELEASE)
		{
			if (button == GLFW_MOUSE_BUTTON_1)
			{
				Input::GetInstance().mouseleftClickHold = false;
				//INPUT_SYSTEM->mMouseLeftClickHold = false;
				//AeonCore_WARN("left click is released");
			}
			if (button == GLFW_MOUSE_BUTTON_2)
			{
				Input::GetInstance().mouseleftClickHold = false;
				//INPUT_SYSTEM->mMouseLeftClickHold = false;
				//AeonCore_WARN("left click is released");
			}

		}
		else if (action == GLFW_PRESS)
		{
			if (button == GLFW_MOUSE_BUTTON_1)
			{
				//std::cout << "Mouse Button left click" << std::endl;
				Input::GetInstance().mouseleftClickHold = true;

			}
			if (button == GLFW_MOUSE_BUTTON_2)
			{
				//std::cout << "Mouse Button left click" << std::endl;
				Input::GetInstance().mouseleftClickHold = true;

			}
		}
		else if (action == GLFW_RELEASE)
		{
			Input::GetInstance().mouseleftClickHold = false;
		}
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will callback for keyboard input
	\param GLFWindow*
		This parameter contains the pointer to the GLFW window
	\param key
		This parameter contains the key id
	\param action
		This parameter contains the action whether is it pressed or released.
	*/
	/******************************************************************************/
	void Input::keyCallback(GLFWwindow*, int key, int /*scancode*/, int action, int /*mods*/)
	{
		Input& InputInst = Input::GetInstance();

		if (action == GLFW_PRESS /*|| action == GLFW_REPEAT*/)
		{
			InputInst.keyTriggers.insert({key,KeyTriggerState
				::JustAdded});
		}

		if (action == GLFW_RELEASE)
		{
			InputInst.keyTriggers.erase(key);
		}
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will callback for the mouse scroll
	\param GLFWindow* window
		This parameter contains the pointer to the GLFW window
	\param xoffet
		This parameter contains the scroll xoffset
	\param yoffset
		This parameter contains the scroll yoffset
	*/
	/******************************************************************************/
	void Input::mouseScrollCallback(GLFWwindow* window, double Xoffset, double Yoffset)
	{
		Input::GetInstance().SetScrollX(static_cast<float>(Xoffset));
		Input::GetInstance().SetScrollY(static_cast<float>(Yoffset));
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will check whether the key is triggered
	\param keycode
		This parameter contains the keycode
	*/
	/******************************************************************************/
	bool Input::IsKeyTriggered(int keycode)
	{
		Input& InputInst = Input::GetInstance();

		auto keyItr = InputInst.keyTriggers.find(keycode);

		return keyItr != InputInst.keyTriggers.end() && keyItr->second == KeyTriggerState::True;
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will update the keytriggerstate
	\param keycode
		This parameter contains the keycode
	*/
	/******************************************************************************/
	void Input::UpdateKeyTriggerState()
	{
		for (auto& [keyCode, triggerState] : keyTriggers) {
			if (triggerState == KeyTriggerState::JustAdded) {
				triggerState = KeyTriggerState::True;
			}
			else if (triggerState == KeyTriggerState::True) {
				triggerState = KeyTriggerState::False;
			}
		}
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will check whether the key is released, returns false by default
	\param keycode
		This parameter contains the keycode
	*/
	/******************************************************************************/
	bool Input::IsKeyReleased(int keycode)
	{
		auto window = static_cast<GLFWwindow*> (Application::GetApplication().GetWindow().GetNativeWindow());
		auto state = glfwGetKey (window, keycode);

		if (state == GLFW_RELEASE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Input::IsJoystickButtonPressed(int JoystickID, JoyStickInput joyStickInput) {
		if (!IsJoyStickConnected(JoystickID)) {
			return false;
		}

		auto joyStickItr = mJoyStickInfoMap.find(JoystickID);
		if (joyStickItr != mJoyStickInfoMap.end()) {
			//AeonCore_INFO("IsMappingJoyStickButtonPressed: Joystick {} exists", JoystickID);
			return joyStickItr->second.joystickButtonTrigger[(int)joyStickInput] == KeyTriggerState::JustAdded;
		}
		return false;
	}

	float Input::GetJoystickAxesValue(int JoystickID, JoyStickInput joyStickInput) {
		if (!IsJoyStickConnected(JoystickID)) {
			return 0.0f;
		}

		auto joyStickItr = mJoyStickInfoMap.find(JoystickID);
		if (joyStickItr != mJoyStickInfoMap.end()) {
			switch (joyStickInput) 
			{
				case JoyStickInput::Axes_Lstick_Axis_X:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_LSTICK_AXIS_X];
				case JoyStickInput::Axes_Lstick_Axis_Y:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_LSTICK_AXIS_Y];
				case JoyStickInput::Axes_Rstick_Axis_X:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_RSTICK_AXIS_X];
				case JoyStickInput::Axes_Rstick_Axis_Y:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_RSTICK_AXIS_Y];
				case JoyStickInput::Axes_Ltrigger:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_LTRIGGER];
				case JoyStickInput::Axes_Rtrigger:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_RTRIGGER];
			}
		}
		return 0.0f;
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will register the keymapping either is keyboard/mouse or
		joystick
	\param name
		This parameter contains the action name
	\param KeyMouseInput
		This parameter contains keyboard/mouse related inputs
	\param JoystickInput
		This parameter contains joystick related inputs
	*/
	/******************************************************************************/
	void Input::RegisterMapping(const std::string& name, KeyMouseInput keyBoardMousecode, JoyStickInput joyStickCode)
	{
		mInputActionMap.emplace(name, MappingInfo{ keyBoardMousecode , joyStickCode });
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function sets the the action to keybopard mouse input
	\param name
		This parameter contains the action name
	\param KBMInput kbmtype
		This parameter contains keyboard/mouse type
	\param keycode
		This parameter contains keycode
	*/
	/******************************************************************************/
	void Input::SetMappingKey(const std::string& name, KBMInputType kbmType , int keycode)
	{		
		auto mappingItr = mInputActionMap.find(name);
		if (mappingItr != mInputActionMap.end())
		{
			mappingItr->second.keyboardMouseInput = KeyMouseInput{kbmType, keycode};
		}
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function sets the the action to joystick
	\param name
		This parameter contains the action name
	\param JoyStickINput
		This parameter contains Joystick Input
	*/
	/******************************************************************************/
	void Input::SetMappingJoystick(const std::string& name, JoyStickInput button)
	{

		auto mappingItr = mInputActionMap.find(name);
		if (mappingItr != mInputActionMap.end())
		{
			mappingItr->second.joystickInput = button;
		}
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function checks whether the mapping key is pressed
	\param name
		This parameter contains the action name
	*/
	/******************************************************************************/
	bool Input::IsMappingKeyPressed(const std::string& name)
	{
		auto mappingItr = mInputActionMap.find(name);
		if (mappingItr != mInputActionMap.end())
		{
			KeyMouseInput& mapKeyInput = mappingItr->second.keyboardMouseInput;
			if (mapKeyInput.type == KBMInputType::Keyboard)
			{
				return IsKeyTriggered(mapKeyInput.InputCode);
			}
			else
			{
				return IsMouseButtonPressed(mapKeyInput.InputCode);
			}
		}

		return false;
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function checks whether the mapping key is held
	\param name
		This parameter contains the action name
	*/
	/******************************************************************************/
	bool Input::IsMappingKeyHeld(const std::string& name)
	{
		auto mappingItr = mInputActionMap.find(name);
		if (mappingItr != mInputActionMap.end())
		{
			KeyMouseInput& mapKeyInput = mappingItr->second.keyboardMouseInput;
			if (mapKeyInput.type == KBMInputType::Keyboard)
			{
				return IsKeyHeld(mapKeyInput.InputCode);
			}
			else
			{
				return IsMouseButtonHeld(mapKeyInput.InputCode);
			}
		}
		return false;
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function checks whether the mapping joystick button is pressed
	\param JoystickID
		This parameter contains the joystick ID
	*/
	/******************************************************************************/
	bool Input::IsMappingJoyStickButtonPressed(const std::string& name, int JoystickID)
	{
		if (!IsJoyStickConnected(JoystickID))
		{
			return false;
		}
		auto mappingItr = mInputActionMap.find(name);
		if (mappingItr != mInputActionMap.end())
		{
			//AeonCore_INFO("IsMappingJoyStickButtonPressed: Mapping {} exists", name);
			if (mappingItr->second.joystickInput > JoyStickInput::Button_Dpad_Left)
			{
				return false;
			}
			auto joyStickItr = mJoyStickInfoMap.find(JoystickID);
			if (joyStickItr != mJoyStickInfoMap.end())
			{
				//AeonCore_INFO("IsMappingJoyStickButtonPressed: Joystick {} exists", JoystickID);
				return joyStickItr->second.joystickButtonTrigger[(int)mappingItr->second.joystickInput] == KeyTriggerState::JustAdded;
			}
			
		}
		
		return false;
	}

	/******************************************************************************/
	/*!
	Function Level Documentation:
	\brief
		This function will return the joystick's axes' values
	\param JoystickID
		This parameter contains the joystick ID
	*/
	/******************************************************************************/
	float Input::GetMappingJoystickAxesValue(const std::string& name, int JoystickID)
	{
		if (!IsJoyStickConnected(JoystickID))
		{
			return 0.0f;
		}
		auto mappingItr = mInputActionMap.find(name);
		if (mappingItr != mInputActionMap.end())
		{
			auto joyStickItr = mJoyStickInfoMap.find(JoystickID);
			if (joyStickItr != mJoyStickInfoMap.end())
			{
				auto controllerInput = mappingItr->second.joystickInput;
				switch (controllerInput) 
				{
				case JoyStickInput::Axes_Lstick_Axis_X:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_LSTICK_AXIS_X];
				case JoyStickInput::Axes_Lstick_Axis_Y:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_LSTICK_AXIS_Y];
				case JoyStickInput::Axes_Rstick_Axis_X:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_RSTICK_AXIS_X];
				case JoyStickInput::Axes_Rstick_Axis_Y:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_RSTICK_AXIS_Y];
				case JoyStickInput::Axes_Ltrigger:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_LTRIGGER];
				case JoyStickInput::Axes_Rtrigger:
					return joyStickItr->second.axesInput[AC_GAMEPAD_AXES_RTRIGGER];
				}
				//AeonCore_INFO("IsMappingJoyStickButtonPressed: Joystick {} exists", JoystickID);
				//return joyStickItr->second.joystickButtonTrigger[(int)mappingItr->second.joystickInput] == KeyTriggerState::JustAdded;
			}
		}
		return 0.0f;
	}
}
// Archive for zhong en
/*
*  if (currInst.axes[0]) //left stick x-axis
//	{
//		//if (abs(currInst.axes[0] > 0.10f))
//		//{
//		//	currInst.axes[0] = 0.0f;
//		//}
//		//std::cout << "Left Stick x-axis: " << currInst.axes[0] << std::endl;
//	}
//	if (currInst.axes[1]) //left stick y-axis
//	{
//		//if (abs(currInst.axes[0] > 0.10f))
//		//{
//		//	currInst.axes[0] = 0.0f;
//		//}
//		//std::cout << "Left Stick y-axis: " << currInst.axes[1] << std::endl;
//	}
//	if (currInst.axes[2]) //Right stick x-axis
//	{
//		//if (abs(currInst.axes[0] > 0.10f))
//		//{
//		//	currInst.axes[0] = 0.0f;
//		//}
//		//std::cout << "Right Stick x-axis: " << currInst.axes[2] << std::endl;
//	}
//	if (currInst.axes[3]) //Right stick y-axis
//	{
//		//if (abs(currInst.axes[0] > 0.10f))
//		//{
//		//	currInst.axes[0] = 0.0f;
//		//}
//		//std::cout << "Right Stick y-axis: " << currInst.axesInput[3] << std::endl;
//	}
//	if (currInst.axes[4]) //Left Trigger
//	{
//		//if (abs(currInst.axes[0] > 0.10f))
//		//{
//		//	currInst.axes[0] = 0.0f;
//		//}
//		//std::cout << "Left Trigger: " << currInst.axes[4] << std::endl;
//	}
//	if (currInst.axes[5]) //Right trigger
//	{
//		//if (abs(currInst.axes[0] > 0.10f))
//		//{
//		//	currInst.axes[0] = 0.0f;
//		//}
//		//std::cout << "Right Trigger: " << currInst.axes[5] << std::endl;
//	}
// 
// //void Input::cameraCalculation()
	//{
	//	static bool firstmouse = true;
	//	if (Input::GetInstance().mouseleftClickHold)
	//	{
	//		

	//		float xpos = static_cast<float>(Input::GetMouseX());
	//		float ypos = static_cast<float>(Input::GetMouseY());
	//		static float lastx = 0.0f;
	//		static float lasty = 0.0f;
	//		static float lastz = 3;
	//		if (firstmouse)
	//		{
	//			lastx = xpos;
	//			lasty = ypos;
	//			firstmouse = false;
	//		}

	//		//float xoffset = xpos - lastx;
	//		float xoffset = xpos - lastx;
	//		float yoffset = lasty - ypos; // reversed since y-coordinates go from bottom to top

	//		lastx = xpos;
	//		lasty = ypos;

	//		//editorvp_camera->SetPosition(glm::vec3(xoffset, yoffset, 3));

	//		//EditorCamera::GetInstance().ProcessMouseMovement(xoffset, yoffset);
	//		//editorvp_camera->SetPosition(glm::vec3(0, 0, 3));
	//		
	//	}
	//	if (Input::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1))
	//	{
	//		firstmouse = true;
	//	}
	//}
//}
in SetMappingKey function
/*for (auto& [mappingname, mappingkey] : mInputActionMap)
{
	if (mappingname != name && mappingkey == keycode)
	{
		AeonCore_ERROR("Key is in use by {0}", name.c_str());
		return;
	}
}

in register mapping function
		//for (auto& [mappingname, mappinginfo] : mInputActionMap)
		//{
		//	KeyMouseInput& mapKeyInput = mappinginfo.keyboardMouseInput;

		//	/*if (mappingname != name && mapKeyInput.InputCode == keyBoardMousecode.InputCode && mapKeyInput.type == keyBoardMousecode.type)
		//	{
		//		AeonCore_ERROR("Key is in use by {0}", name.c_str());
		//		return;
		//	}
		//}

in setmapping function
		/*for (auto& [mappingname, mappingkey] : mInputActionMap)
		{
			if (mappingname != name && mappingkey == keycode)
			{
				AeonCore_ERROR("Key is in use by {0}", name.c_str());
				return;
			}
		}

in keycallback function
			/*if (IsKeyHeld(GLFW_KEY_W))  EditorCamera::GetInstance().ProcessKeyboard(Camera_Movement::FORWARD, TimerStep::GetInstance().get_delta_time());
			if (IsKeyHeld(GLFW_KEY_A))  EditorCamera::GetInstance().ProcessKeyboard(Camera_Movement::LEFT, TimerStep::GetInstance().get_delta_time());
			if (IsKeyHeld(GLFW_KEY_S))  EditorCamera::GetInstance().ProcessKeyboard(Camera_Movement::BACKWARD, TimerStep::GetInstance().get_delta_time());
			if (IsKeyHeld(GLFW_KEY_D))  EditorCamera::GetInstance().ProcessKeyboard(Camera_Movement::RIGHT, TimerStep::GetInstance().get_delta_time());

in keycallback after glfw release

		//if (action == GLFW_REPEAT)
		//{
		//	if (IsKeyPressed(GLFW_KEY_W))  EditorCamera::GetInstance().ProcessKeyboard(Camera_Movement::FORWARD, TimerStep::GetInstance().get_delta_time());
		//	if (IsKeyPressed(GLFW_KEY_A))  EditorCamera::GetInstance().ProcessKeyboard(Camera_Movement::LEFT, TimerStep::GetInstance().get_delta_time());
		//	if (IsKeyPressed(GLFW_KEY_S))  EditorCamera::GetInstance().ProcessKeyboard(Camera_Movement::BACKWARD, TimerStep::GetInstance().get_delta_time());
		//	if (IsKeyPressed(GLFW_KEY_D))  EditorCamera::GetInstance().ProcessKeyboard(Camera_Movement::RIGHT, TimerStep::GetInstance().get_delta_time());
		//}

in mousecallback function
				//static bool firstmouse = true;
				//if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && action != GLFW_RELEASE)
				//{
				//	float xpos = static_cast<float>(Input::GetMouseX());
				//	float ypos = static_cast<float>(Input::GetMouseY());
				//	static float lastx = 0.0f;
				//	static float lasty = 0.0f;
				//	static float lastz = 3;
				//	if (firstmouse)
				//	{
				//		lastx = xpos;
				//		lasty = ypos;
				//		firstmouse = false;
				//	}

				//	//float xoffset = xpos - lastx;
				//	float xoffset = xpos - lastx;
				//	float yoffset = lasty - ypos; // reversed since y-coordinates go from bottom to top

				//	lastx = xpos;
				//	lasty = ypos;

				//	//editorvp_camera->SetPosition(glm::vec3(xoffset, yoffset, 3));

				//	EditorViewport::GetInstance().editorvp_camera->ProcessMouseMovement(xoffset, yoffset);
				//	//editorvp_camera->SetPosition(glm::vec3(0, 0, 3));
				//}

				//if (Input::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1))
				//{
				//	firstmouse = true;
				//}

in update controller mapping

		/*if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
		{
			axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &(num_axes));
			buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &(num_buttons));
			if (axes == nullptr)
			{
				return;
			}

			for (int i = 0; i < 6; i++)
			{
				axesInput[i] = axes[i];
			}

			if (axesInput[0] <= 0.08f && axesInput[0] >= -0.08f)
			{
				axesInput[0] = 0;
			}

			if (axesInput[1] <= 0.15f && axesInput[1] >= -0.15f)
			{
				axesInput[1] = 0;
			}
		}
*/
			





