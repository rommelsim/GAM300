/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	Input.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines Input functions as well as
            keycodes and mouse buttons
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/
namespace AeonScriptCore
{
    public class Input
    {
        /// <summary>
        /// Returns true during the frame the user starts pressing down the key
        /// </summary>
        /// <param name="key">The desired key to get its pressed state for</param>
        /// <returns>A boolean, true or false</returns>
        public static bool GetKeyPressed(int key)
        {
            return InternalCalls.Input_GetKeyPressed(key);
        }

        /// <summary>
        /// Returns true while the user holds down the key
        /// </summary>
        /// <param name="key">The desired key to get its held state for</param>
        /// <returns>A boolean, true or false</returns>
        public static bool GetKeyHeld(int key)
        {
            return InternalCalls.Input_GetKeyHeld(key);
        }

        /// <summary>
        /// Returns true while the user holds down the mouse button
        /// </summary>
        /// <param name="key">The desired mouse to get its held state for</param>
        /// <returns>A boolean, true or false</returns>
        public static bool GetMouseButtonHeld(int key)
        {
            return InternalCalls.Input_GetMouseButtonHeld(key);
        }

        /// <summary>
        /// Register a key mapping, or "keybinding", to give a key a "meaning"
        /// </summary>
        /// <param name="mappingName">Mapping name</param>
        /// <param name="kbmInputType">The type of input for non-joystick, is it keyboard or mouse?</param>
        /// <param name="kbmCode">The key to assign to the mapping, can either be a Key's code or a Mouse's code</param>
        /// <param name="joystickInput"></param>
        public static void RegisterMapping(string mappingName, KBMInputType kbmInputType, int kbmCode, JoystickInput joystickInput)
        {
            InternalCalls.Input_RegisterMapping(mappingName, (int)kbmInputType, kbmCode, (int)joystickInput);
        }
        
        /// <summary>
        /// Returns true during the frame the user starts pressing down the mapping's keyboard or mouse input
        /// </summary>
        /// <param name="mappingName">The name of the key mapping</param>
        /// <returns>A boolean, true or false</returns>
        public static bool GetMappingKeyPressed(string mappingName)
        {
            return InternalCalls.Input_IsMappingKeyPressed(mappingName);
        }

        /// <summary>
        /// Returns true while the user holds down the mapping's key
        /// </summary>
        /// <param name="mappingName">The name of the key mapping</param>
        /// <returns>A boolean, true or false</returns>
        public static bool GetMappingKeyHeld(string mappingName)
        {
            return InternalCalls.Input_IsMappingKeyHeld(mappingName);
        }

        /// <summary>
        /// Returns true while the user holds down the mapping's joystick button. Otherwise false.<br/>
        /// If the mapping does not have a joystick button for it's joystick input, or, if the given joystick ID is not connected, false is returned.
        /// </summary>
        /// <param name="mappingName">The name of the mapping</param>
        /// <param name="gamePadId">An ID of the gamepad, obtainable from <see cref="GetConnectedGamepadIds">GetConnectedGamepadIds</see></param>
        /// <returns>A boolean, true or false</returns>
        public static bool GetMappingGamePadButtonPressed(string mappingName, int gamePadId)
        {
            return InternalCalls.Input_IsMappingJoystickButtonPressed(mappingName, gamePadId);
        }

        /// <summary>
        /// Returns a float value while the user moves a joystick control axis, the range is in -1.0f to 1.0f. If the given joystick ID is not connected, 0.0f is returned.<br/>
        /// For the thumbsticks' axes values, negative-Y is nudged forward, positive-Y is nudged backward. Positive-X is nudged rightward, negative-X is nudged leftward.
        /// </summary>
        /// <param name="mappingName">The name of the mapping</param>
        /// <param name="gamePadId">An ID of the gamepad, obtainable from <see cref="GetConnectedGamepadIds">GetConnectedGamepadIds</see></param>
        /// <returns>A float</returns>
        public static float GetMappingGamePadAxisValue(string mappingName, int gamePadId)
        {
            return InternalCalls.Input_GetMappingJoystickAxesValue(mappingName, gamePadId);
        }

        /// <summary>
        /// Returns true if a given gamepad ID is connected, otherwise false
        /// </summary>
        /// <param name="gamePadId">An ID of the gamepad, obtainable from <see cref="GetConnectedGamepadIds">GetConnectedGamepadIds</see></param>
        /// <returns>A boolean, true or false</returns>
        public static bool IsGamepadConnected(int gamePadId)
        {
            return InternalCalls.Input_IsJoystickConnected(gamePadId);
        }

        /// <summary>
        /// Get all the connected gamepad IDs. An ID is represented as an integer
        /// </summary>
        /// <returns>An integer array</returns>
        public static int[] GetConnectedGamepadIds()
        {
            return InternalCalls.Input_GetConnectedJoysticks();
        }

        /// <summary>
        /// For a valid gamepad id, the function returns true when the user presses on the joystick's button. Otherwise false.<br/>
        /// If the given gamepad id is not connected, false is returned.
        /// </summary>
        /// <param name="gamePadId">An ID of the gamepad, obtainable from <see cref="GetConnectedGamepadIds">GetConnectedGamepadIds</see></param>
        /// <param name="joystickInput">Only the joystick input buttons are valid, the axes are invalid and hence will return false.</param>
        /// <returns>A boolean, true or false</returns>
        public static bool IsGamePadButtonPressed(int gamePadId, JoystickInput joystickInput)
        {
            return InternalCalls.Input_IsJoystickButtonPressed(gamePadId, (int)joystickInput);
        }

        /// <summary>
        /// For a valid gamepad id, the function returns a float value while the user moves a joystick control axis, the range is in -1.0f to 1.0f. If the given joystick ID is not connected, 0.0f is returned.<br/>
        /// For the thumbsticks' axes values, negative-Y is nudged forward, positive-Y is nudged backward. Positive-X is nudged rightward, negative-X is nudged lefward.
        /// </summary>
        /// <param name="gamePadId">An ID of the gamepad, obtainable from <see cref="GetConnectedGamepadIds">GetConnectedGamepadIds</see></param>
        /// <param name="joystickInput">Only the joystick input axes are valid, the buttons are invalid and hence will return 0.0f</param>
        /// <returns>A float</returns>
        public static float GetGamePadAxisValue(int gamePadId, JoystickInput joystickInput)
        {
            return InternalCalls.Input_GetJoystickAxesValue(gamePadId, (int)joystickInput);
        }

        /// <summary>
        /// What is the input for Keyboard-Mouse setup? Is it Mouse or Keyboard? This is meant to prevent collision of values between the Keyboard codes and the Mouse button codes
        /// </summary>
        public enum KBMInputType
        {
            Mouse, Keyboard
        }

        /// <summary>
        /// What is the desired joystick input you wish to know? The names are styled after the Xbox One controller
        /// </summary>
        public enum JoystickInput
        {
		    Button_A, Button_B, Button_X, Button_Y,
		    Button_Left_Bumper, Button_Right_Bumper,
		    Button_Back, Button_Start,
		    Button_Left_Thumb, Button_Right_Thumb,
		    Button_Dpad_Up, Button_Dpad_Right, Button_Dpad_Down,
		    Button_Dpad_Left,

		    Axes_LStick_Axis_X, Axes_LStick_Axis_Y,
		    Axes_RStick_Axis_X, Axes_RStick_Axis_Y,
		    Axes_LTrigger, Axes_RTrigger
        }
    }

    public class Keys
    {
        public static int KEY_SPACE = 32;
        public static int KEY_APOSTROPHE = 39;
        public static int KEY_COMMA = 44;
        public static int KEY_MINUS = 45;
        public static int KEY_PERIOD = 46;
        public static int KEY_SLASH = 47;
        public static int KEY_0 = 48;
        public static int KEY_1 = 49;
        public static int KEY_2 = 50;
        public static int KEY_3 = 51;
        public static int KEY_4 = 52;
        public static int KEY_5 = 53;
        public static int KEY_6 = 54;
        public static int KEY_7 = 55;
        public static int KEY_8 = 56;
        public static int KEY_9 = 57;
        public static int KEY_Semicolon = 59;
        public static int KEY_Equal = 61;
        public static int KEY_A = 65;
        public static int KEY_B = 66;
        public static int KEY_C = 67;
        public static int KEY_D = 68;
        public static int KEY_E = 69;
        public static int KEY_F = 70;
        public static int KEY_G = 71;
        public static int KEY_H = 72;
        public static int KEY_I = 73;
        public static int KEY_J = 74;
        public static int KEY_K = 75;
        public static int KEY_L = 76;
        public static int KEY_M = 77;
        public static int KEY_N = 78;
        public static int KEY_O = 79;
        public static int KEY_P = 80;
        public static int KEY_Q = 81;
        public static int KEY_R = 82;
        public static int KEY_S = 83;
        public static int KEY_T = 84;
        public static int KEY_U = 85;
        public static int KEY_V = 86;
        public static int KEY_W = 87;
        public static int KEY_X = 88;
        public static int KEY_Y = 89;
        public static int KEY_Z = 90;
        public static int KEY_LEFT_BRACKET = 91;
        public static int KEY_BACKSLASH = 92;
        public static int KEY_RIGHT_BRACKET = 93;
        public static int KEY_GRAVE_ACCENT = 96;

        public static int KEY_ESCAPE = 256;
        public static int KEY_ENTER = 257;
        public static int KEY_TAB = 258;
        public static int KEY_BACKSPACE = 259;
        public static int KEY_INSERT = 260;
        public static int KEY_DELETE = 261;
        public static int KEY_RIGHT = 262;
        public static int KEY_LEFT = 263;
        public static int KEY_DOWN = 264;
        public static int KEY_UP = 265;
        public static int KEY_PAGE_UP = 266;
        public static int KEY_PAGE_DOWN = 267;
        public static int KEY_HOME = 268;
        public static int KEY_END = 269;
        public static int KEY_CAPS_LOCK = 280;
        public static int KEY_SCROLL_LOCK = 281;
        public static int KEY_NUM_LOCK = 282;
        public static int KEY_PRINT_SCREEN = 283;
        public static int KEY_PAUSE = 284;
        public static int KEY_F1 = 290;
        public static int KEY_F2 = 291;
        public static int KEY_F3 = 292;
        public static int KEY_F4 = 293;
        public static int KEY_F5 = 294;
        public static int KEY_F6 = 295;
        public static int KEY_F7 = 296;
        public static int KEY_F8 = 297;
        public static int KEY_F9 = 298;
        public static int KEY_F10 = 299;
        public static int KEY_F11 = 300;
        public static int KEY_F12 = 301;
        public static int KEY_LEFT_SHIFT = 340;
        public static int KEY_LEFT_CONTROL = 341;
        public static int KEY_LEFT_ALT = 342;
        public static int KEY_LEFT_SUPER = 343; // Windows key or Command key (mac)
        public static int KEY_RIGHT_SHIFT = 344;
        public static int KEY_RIGHT_CONTROL = 345;
        public static int KEY_RIGHT_ALT = 346;
        public static int KEY_RIGHT_SUPER = 347; // Windows key or Command key (mac)

        public static int MOUSE_BUTTON_LEFT = 0;
        public static int MOUSE_BUTTON_RIGHT = 1;
        public static int MOUSE_BUTTON_MIDDLE = 2;
    }
}
