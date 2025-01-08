#pragma once
/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	Input.ch
 * authors:
		Lee Zhong En				(zhongen.lee@digipen.edu)
 *
 * brief:	This file contains the function declaration for the input class
 *
 *
 * Copyright (C) 2023 DigiPen Institute of Technology.
   Reproduction or disclosure of this file or its contents without the
   prior written consent of DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------*/

#include <memory>
#include <map>

#include "Core/AeonAPI.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Core/Singleton.hpp"

#include "ECS/Components/Camera.h"
#include "Editor/EditorCamera.h"
#include "AeonCore.h"

#define 	AC_KEY_SPACE GLFW_KEY_SPACE   
#define 	AC_KEY_APOSTROPHE GLFW_KEY_APOSTROPHE    /* ' */
#define 	AC_KEY_COMMA GLFW_KEY_COMMA    /* , */
#define 	AC_KEY_MINUS GLFW_KEY_MINUS   4/* - */
#define 	AC_KEY_PERIOD GLFW_KEY_PERIOD    /* . */
#define 	AC_KEY_SLASH GLFW_KEY_SLASH   47 /* / */
#define 	AC_KEY_0 GLFW_KEY_0   
#define 	AC_KEY_1 GLFW_KEY_1   
#define 	AC_KEY_2 GLFW_KEY_2   
#define 	AC_KEY_3 GLFW_KEY_3   
#define 	AC_KEY_4 GLFW_KEY_4   
#define 	AC_KEY_5 GLFW_KEY_5   
#define 	AC_KEY_6 GLFW_KEY_6   
#define 	AC_KEY_7 GLFW_KEY_7   
#define 	AC_KEY_8 GLFW_KEY_8   
#define 	AC_KEY_9 GLFW_KEY_9   
#define 	AC_KEY_SEMICOLON GLFW_KEY_SEMICOLON    /* ; */
#define 	AC_KEY_EQUAL GLFW_KEY_EQUAL    /* = */
#define 	AC_KEY_A GLFW_KEY_A   
#define 	AC_KEY_B GLFW_KEY_B   
#define 	AC_KEY_C GLFW_KEY_C   
#define 	AC_KEY_D GLFW_KEY_D   
#define 	AC_KEY_E GLFW_KEY_E   
#define 	AC_KEY_F GLFW_KEY_F   
#define 	AC_KEY_G GLFW_KEY_G   
#define 	AC_KEY_H GLFW_KEY_H   
#define 	AC_KEY_I GLFW_KEY_I   
#define 	AC_KEY_J GLFW_KEY_J   
#define 	AC_KEY_K GLFW_KEY_K   
#define 	AC_KEY_L GLFW_KEY_L   
#define 	AC_KEY_M GLFW_KEY_M   
#define 	AC_KEY_N GLFW_KEY_N   
#define 	AC_KEY_O GLFW_KEY_O   
#define 	AC_KEY_P GLFW_KEY_P   
#define 	AC_KEY_Q GLFW_KEY_Q   
#define 	AC_KEY_R GLFW_KEY_R   
#define 	AC_KEY_S GLFW_KEY_S   
#define 	AC_KEY_T GLFW_KEY_T   
#define 	AC_KEY_U GLFW_KEY_U   
#define 	AC_KEY_V GLFW_KEY_V   
#define 	AC_KEY_W GLFW_KEY_W   
#define 	AC_KEY_X GLFW_KEY_X   
#define 	AC_KEY_Y GLFW_KEY_Y   
#define 	AC_KEY_Z GLFW_KEY_Z   
#define 	AC_KEY_LEFT_BRACKET GLFW_KEY_LEFT_BRACKET    /* [ */
#define 	AC_KEY_BACKSLASH GLFW_KEY_BACKSLASH    /* \ */
#define 	AC_KEY_RIGHT_BRACKET GLFW_KEY_RIGHT_BRACKET    /* ] */
#define 	AC_KEY_GRAVE_ACCENT GLFW_KEY_GRAVE_ACCENT    /* ` */
#define 	AC_KEY_KEY_WORLD_1 GLFW_KEY_WORLD_1    /* non-US #1 */
#define 	AC_KEY_WORLD_2 GLFW_KEY_WORLD_2   /* non-US #2 */
#define 	AC_KEY_ESCAPE GLFW_KEY_ESCAPE   
#define 	AC_KEY_ENTER GLFW_KEY_ENTER   
#define 	AC_KEY_TAB GLFW_KEY_TAB   
#define 	AC_KEY_BACKSPACE GLFW_KEY_BACKSPACE   
#define 	AC_KEY_INSERT GLFW_KEY_INSERT   
#define 	AC_KEY_DELETE GLFW_KEY_DELETE   
#define 	AC_KEY_RIGHT GLFW_KEY_RIGHT   
#define 	AC_KEY_LEFT GLFW_KEY_LEFT   
#define 	AC_KEY_DOWN GLFW_KEY_DOWN   
#define 	AC_KEY_UP GLFW_KEY_UP   
#define 	AC_KEY_PAGE_UP GLFW_KEY_PAGE_UP   
#define 	AC_KEY_PAGE_DOWN GLFW_KEY_PAGE_DOWN   
#define 	AC_KEY_HOME GLFW_KEY_HOME   
#define 	AC_KEY_END GLFW_KEY_END   
#define 	AC_KEY_CAPS_LOCK GLFW_KEY_CAPS_LOCK   
#define 	AC_KEY_SCROLL_LOCK GLFW_KEY_SCROLL_LOCK   
#define 	AC_KEY_NUM_LOCK GLFW_KEY_NUM_LOCK   
#define 	AC_KEY_PRINT_SCREEN GLFW_KEY_PRINT_SCREEN   
#define 	AC_KEY_PAUSE GLFW_KEY_PAUSE   
#define 	AC_KEY_F1 GLFW_KEY_F1   
#define 	AC_KEY_F2 GLFW_KEY_F2   
#define 	AC_KEY_F3 GLFW_KEY_F3   
#define 	AC_KEY_F4 GLFW_KEY_F4   
#define 	AC_KEY_F5 GLFW_KEY_F5   
#define 	AC_KEY_F6 GLFW_KEY_F6   
#define 	AC_KEY_F7 GLFW_KEY_F7   
#define 	AC_KEY_F8 GLFW_KEY_F8   
#define 	AC_KEY_F9 GLFW_KEY_F9   
#define 	AC_KEY_F10 GLFW_KEY_F10   
#define 	AC_KEY_F11 GLFW_KEY_F11   
#define 	AC_KEY_F12 GLFW_KEY_F12   
//#define 	AC_KEY_F13 GLFW_KEY_LEFT_SHIFT   
#define 	AC_KEY_LEFT_CONTROL GLFW_KEY_LEFT_CONTROL   
#define 	AC_KEY_LEFT_ALT GLFW_KEY_LEFT_ALT  
#define 	AC_KEY_LEFT_SUPER GLFW_KEY_LEFT_SUPER   
#define 	AC_KEY_RIGHT_SHIFT GLFW_KEY_RIGHT_SHIFT   
#define 	AC_KEY_LEFT_SHIFT GLFW_KEY_LEFT_SHIFT   
#define 	AC_KEY_RIGHT_CONTROL GLFW_KEY_RIGHT_CONTROL   
#define 	AC_KEY_RIGHT_ALT GLFW_KEY_RIGHT_ALT  
#define 	AC_KEY_RIGHT_SUPER GLFW_KEY_RIGHT_SUPER   
#define 	AC_KEY_MENU GLFW_KEY_MENU   
#define 	AC_KEY_LAST   GLFW_KEY_MENU

#define 	AC_GAMEPAD_BUTTON_A  0
#define 	AC_GAMEPAD_BUTTON_B  1
#define 	AC_GAMEPAD_BUTTON_X  2
#define 	AC_GAMEPAD_BUTTON_Y  3
#define 	AC_GAMEPAD_BUTTON_LEFT_BUMPER  4
#define 	AC_GAMEPAD_BUTTON_RIGHT_BUMPER  5
#define 	AC_GAMEPAD_BUTTON_BACK  6
#define 	AC_GAMEPAD_BUTTON_START  7
//#define 	AC_GAMEPAD_BUTTON_GUIDE  ???
#define 	AC_GAMEPAD_BUTTON_LEFT_THUMB  8
#define 	AC_GAMEPAD_BUTTON_RIGHT_THUMB  9
#define 	AC_GAMEPAD_BUTTON_DPAD_UP  10
#define 	AC_GAMEPAD_BUTTON_DPAD_RIGHT  11
#define 	AC_GAMEPAD_BUTTON_DPAD_DOWN  12
#define		AC_GAMEPAD_BUTTON_DPAD_LEFT 13
#define 	AC_GAMEPAD_BUTTON_LAST   AC_GAMEPAD_BUTTON_DPAD_LEFT
#define 	AC_GAMEPAD_BUTTON_CROSS   AC_GAMEPAD_BUTTON_A
#define 	AC_GAMEPAD_BUTTON_CIRCLE   AC_GAMEPAD_BUTTON_B
#define 	AC_GAMEPAD_BUTTON_SQUARE   AC_GAMEPAD_BUTTON_X
#define 	AC_GAMEPAD_BUTTON_TRIANGLE  AC_GAMEPAD_BUTTON_Y

#define		AC_GAMEPAD_AXES_LSTICK_AXIS_X 0
#define		AC_GAMEPAD_AXES_LSTICK_AXIS_Y 1
#define		AC_GAMEPAD_AXES_RSTICK_AXIS_X 2
#define		AC_GAMEPAD_AXES_RSTICK_AXIS_Y 3
#define		AC_GAMEPAD_AXES_LTRIGGER 4
#define		AC_GAMEPAD_AXES_RTRIGGER 5
//i took out from F12 onwards and ALL of the KP keys in glfw documentation

namespace AeonCore
{
	enum class KeyTriggerState {
		JustAdded,
		True,
		False,
	};

	//enum class JoystickState {
	//	JustAdded,
	//	True,
	//	False,
	//	Released // Used only for joystick button state update in UpdateControllerMappings
	//};

	struct JoyStickInfo {
		int joystickID;
		const float* axes = nullptr;
		const unsigned char* buttons = nullptr;
		float axesInput[6];
		int num_axes = 0;
		int num_buttons = 0;

		std::vector<KeyTriggerState> joystickButtonTrigger;
		//std::map<int, KeyTriggerState> joystickButtonTrigger;

	};
	enum class KBMInputType {
		Mouse, Keyboard
	};

	struct KeyMouseInput {
		KBMInputType type{ KBMInputType::Keyboard };
		int InputCode;
	};

	enum class JoyStickInput {
		Button_A, Button_B, Button_X, Button_Y,
		Button_Left_Bumper, Button_Right_Bumper,
		Button_Back, Button_Start,
		Button_Left_Thumb, Button_Right_Thumb,
		Button_Dpad_Up, Button_Dpad_Right, Button_Dpad_Down,
		Button_Dpad_Left,

		Axes_Lstick_Axis_X, Axes_Lstick_Axis_Y,
		Axes_Rstick_Axis_X, Axes_Rstick_Axis_Y,
		Axes_Ltrigger, Axes_Rtrigger
	};

	struct MappingInfo {
		KeyMouseInput keyboardMouseInput;
		JoyStickInput joystickInput;
	};


/******************************************************************************/
/*!
Function Level Documentation:
\brief
	This class contains the declarations for input system.
*/
/******************************************************************************/
	class AEON_API Input : public  Singleton<Input>
	{

	public:
		//const float* axes = nullptr;
		//const unsigned char* buttons = nullptr;
		//static float axesInput[6];
		//int num_axes = 0;
		//int num_buttons = 0;
		bool m_IsJoyStickConnected = false;

		std::map<int, JoyStickInfo> mJoyStickInfoMap;//int = JoystickID

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
		static bool IsKeyHeld(int keycode);

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
		static bool IsMouseButtonPressed(int _button);

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
		static bool IsMouseButtonHeld(int _button);

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
		static bool IsMouseButtonReleased(int _button);

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will return the X value of mouse's position
		\return
			This function returns the mouse's X position
		*/
		/******************************************************************************/
		static float GetMouseX();

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will return the Y value of mouse's position
		\return
			This function returns the mouse's Y position
		*/
		/******************************************************************************/
		static float GetMouseY();

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will pair the mouse's X and Y position
		\return
			This function returns the mouse''s Y position's X and Y value
		*/
		/******************************************************************************/
		static std::pair<float, float> GetMousePosition();

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will update the controller values to the parameters.
		*/
		/******************************************************************************/
		void UpdateControllerMappings();

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will check whether controller is connected
		*/
		/******************************************************************************/
		static bool IsJoyStickConnected(int joystickId);

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
		static void joystick_callback(int jid, int event);

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
		static void mouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/);
		
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
		static void keyCallback(GLFWwindow*, int key, int /*scancode*/, int action, int /*mods*/);

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
		static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will check whether the key is triggered
		\param keycode
			This parameter contains the keycode
		*/
		/******************************************************************************/
		static bool IsKeyTriggered(int keycode);

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will update the keytriggerstate
		\param keycode
			This parameter contains the keycode
		*/
		/******************************************************************************/
		void UpdateKeyTriggerState();

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will check whether the key is released, returns false by default
		\param keycode
			This parameter contains the keycode
		*/
		/******************************************************************************/
		static bool IsKeyReleased(int keycode);
		
		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function checks whether a specific joystick's button is pressed
		\param JoystickID
			This parameter contains the joystick ID
		\param joyStickInput
			This parameter contains joystick related inputs
		*/
		/******************************************************************************/
		bool IsJoystickButtonPressed(int JoystickID, JoyStickInput joyStickInput);
		
		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will return a specific joystick's axes' values
		\param JoystickID
			This parameter contains the joystick ID
		\param joyStickInput
			This parameter contains joystick related inputs
		*/
		/******************************************************************************/
		float GetJoystickAxesValue(int JoystickID, JoyStickInput joyStickInput);

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
		void RegisterMapping(const std::string& name, KeyMouseInput, JoyStickInput);

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
		void SetMappingKey(const std::string& name, KBMInputType kbmType, int keycode);

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
		void SetMappingJoystick(const std::string& name, JoyStickInput);
		
		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function checks whether the mapping key is pressed
		\param name
			This parameter contains the action name
		*/
		/******************************************************************************/
		bool IsMappingKeyPressed(const std::string& name);

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function checks whether the mapping key is held
		\param name
			This parameter contains the action name
		*/
		/******************************************************************************/
		bool IsMappingKeyHeld(const std::string& name);

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function checks whether the mapping joystick button is pressed
		\param JoystickID
			This parameter contains the joystick ID
		*/
		/******************************************************************************/
		bool IsMappingJoyStickButtonPressed(const std::string& name, int JoystickID);

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will return the joystick's axes' values
		\param JoystickID
			This parameter contains the joystick ID
		*/
		/******************************************************************************/
		float GetMappingJoystickAxesValue(const std::string& name, int JoystickID);

		//static void cameraCalculation();

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will return mouse callback for the camera
		*/
		/******************************************************************************/
		/*-----------------------------------------------------------------------------------------------
		 *	MOUSE CALLBACK
		 -----------------------------------------------------------------------------------------------*/
		void mouse_callback(float& _lastX, float& _lastY)
		{
			EditorCamera& editorCam = EditorCamera::GetInstance();

			static bool firstMouse = true;
			float xpos = Input::GetMouseX();
			float ypos = Input::GetMouseY();

			if (IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
			{
				if (firstMouse)
				{
					_lastX = xpos;
					_lastY = ypos;
					firstMouse = false;
				}

				float xoffset = xpos - _lastX;
				float yoffset = _lastY - ypos; // reversed since y-coordinates go from bottom to top

				_lastX = xpos;
				_lastY = ypos;

				ProcessMouseMovement(editorCam, xoffset, yoffset);
			}
			else
			{
				_lastX = xpos;
				_lastY = ypos;
			}
		}

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will process mouse movement for camera controls
		\param _cam
			This parameter contains the reference to the camera
		\param xoffset
			This parameter contains the xoffset to move camera
		\param yoffset
			This parameter contains the yoffset to move camera
		\param _constrainPitch
			This parameter contains the flag to set constrain pitch
		*/
		/******************************************************************************/
		/*-----------------------------------------------------------------------------------------------
		 *	Editor Camera Functions
		 -----------------------------------------------------------------------------------------------*/
		void ProcessMouseMovement(EditorCamera& _cam, float _xoffset, float _yoffset, GLboolean _constrainPitch = true)
		{
			float mouseSensitivity = _cam.GetCameraSensitivity();
			_xoffset *= mouseSensitivity;
			_yoffset *= mouseSensitivity;

			float yaw = _cam.GetCameraYaw();
			float pitch = _cam.GetCameraPitch();

			yaw += _xoffset;
			pitch += _yoffset;

			_cam.SetCameraYaw(yaw);

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (_constrainPitch)
			{
				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;
			}
			_cam.SetCameraPitch(pitch);
			/*_cam.UpdateCameraVectors();
			_cam.GetViewMatrix();*/
		}

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will process keyboard input for camera controls
		\param _cam
			This parameter contains the reference to the camera
		\param _movement
			This parameter contains the type of camera movement
		\param _DeltaTime
			This parameter contains the Dt of the camera
		*/
		/******************************************************************************/
		void ProcessCameraKeyboard(EditorCamera& _cam, CameraMovement _movement, float _deltaTime)
		{
			float velocity = _cam.GetCameraMovementSpeed() * _deltaTime;
			glm::vec3 position = _cam.GetCameraPosition();
			glm::vec3 front = _cam.GetCameraFront();
			glm::vec3 right = _cam.GetCameraRight();
			glm::vec3 up = _cam.GetCameraUp();
			
			if (_movement == CameraMovement::FORWARD)	position += front * velocity;
		    if (_movement == CameraMovement::BACKWARD)	position -= front * velocity;
		    if (_movement == CameraMovement::LEFT)		position -= right * velocity;
			if (_movement == CameraMovement::RIGHT)		position += right * velocity;
			if (_movement == CameraMovement::UP)		position += up * velocity;
			if (_movement == CameraMovement::DOWN)		position -= up * velocity;
			_cam.SetCameraPosition(position);
			//_cam.GetViewMatrix();
		}

		/******************************************************************************/
		/*!
		Function Level Documentation:
		\brief
			This function will process mouse movement to move the camera
		\param _cam
			This parameter contains the reference to the camera
		\param xoffset
			This parameter contains the xoffset to move camera
		\param yoffset
			This parameter contains the yoffset to move camera
		\param _target
			This parameter contains the vec3 of the target
		\param _constrainPitch
			This parameter contains the flag to set constrain pitch
		*/
		/******************************************************************************/
		/*-----------------------------------------------------------------------------------------------
		 *	Component Camera Functions
		 -----------------------------------------------------------------------------------------------*/
		void ProcessMouseMovement(Camera& _cam, float _xoffset, float _yoffset, glm::vec3 _target, GLboolean _constrainPitch = true)
		{
			float mouseSensitivity = _cam.GetCameraSensitivity();
			_xoffset *= mouseSensitivity;
			_yoffset *= mouseSensitivity;

			float yaw = _cam.GetCameraYaw();
			float pitch = _cam.GetCameraPitch();
			float zoom = _cam.GetCameraZoom();

			yaw += _xoffset;
			pitch += _yoffset;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (_constrainPitch)
			{
				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;
			}

			_cam.SetCameraYaw(yaw);
			_cam.SetCameraPitch(pitch);


			glm::vec3 camPosition;
			camPosition.x = _target.x + zoom * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
			camPosition.y = _target.y + zoom * sin(glm::radians(pitch));
			camPosition.z = _target.z + zoom * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

			_cam.SetCameraPosition(camPosition);
			_cam.SetCameraTarget(_target);
			//_cam.UpdateCameraPosition();
			//_cam.GetViewMatrix();
		}

		inline void SetScrollX(float _xoffset) { mScrollX = _xoffset; }
		inline void SetScrollY(float _yoffset) { mScrollY = _yoffset; }

		inline float GetScrollX() { return mScrollX; }
		inline float GetScrollY() { return mScrollY; }

	private:
		friend class Singleton<Input>;
		float xAxisDeadZone;
		float yAxisDeadZone;
		bool mouseleftClickHold;
		bool keystate_w;
		std::map<int, KeyTriggerState> keyTriggers;
		std::map<std::string, MappingInfo> mInputActionMap;

		float mScrollX;
		float mScrollY;
	};
}