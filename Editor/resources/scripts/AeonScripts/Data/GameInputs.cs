using AeonScriptCore;

namespace AeonScripts
{
    public class GameInputs
    {
        public enum InputType { Keyboard, Controller, /*Controller1, Controller2,*/ None };

        public static InputType PlayerOneInput = InputType.None;
        public static InputType PlayerTwoInput = InputType.None;

        public static int PlayerOne_GamepadId = -1;
        public static int PlayerTwo_GamepadId = -1;

        // ---- Movement Controls ----
        public static string Map_Forward = "Move_Forward";
        public static string Map_Backward = "Move_Backward";
        public static string Map_Left = "Move_Left";
        public static string Map_Right = "Move_Right";
        public static string Map_Jump = "Jump";
        public static string Map_Interact = "Interact";
        public static string Map_PermInteract = "PermInteract";

        public static string Alt_Map_Suffix = "_Alt";

        // static constructor, code inside here will run once
        static GameInputs()
        {
            Input.RegisterMapping(Map_Forward, Input.KBMInputType.Keyboard, Keys.KEY_W, Input.JoystickInput.Axes_LStick_Axis_Y);
            Input.RegisterMapping(Map_Backward, Input.KBMInputType.Keyboard, Keys.KEY_S, Input.JoystickInput.Axes_LStick_Axis_Y);
            Input.RegisterMapping(Map_Left, Input.KBMInputType.Keyboard, Keys.KEY_A, Input.JoystickInput.Axes_LStick_Axis_X);
            Input.RegisterMapping(Map_Right, Input.KBMInputType.Keyboard, Keys.KEY_D, Input.JoystickInput.Axes_LStick_Axis_X);
            Input.RegisterMapping(Map_Jump, Input.KBMInputType.Keyboard, Keys.KEY_SPACE, Input.JoystickInput.Button_A);
            Input.RegisterMapping(Map_Interact, Input.KBMInputType.Keyboard, Keys.KEY_J, Input.JoystickInput.Button_X);
            Input.RegisterMapping(Map_PermInteract, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_B);

            Input.RegisterMapping(Map_Forward + Alt_Map_Suffix, Input.KBMInputType.Keyboard, Keys.KEY_UP, Input.JoystickInput.Axes_LStick_Axis_Y);
            Input.RegisterMapping(Map_Backward + Alt_Map_Suffix, Input.KBMInputType.Keyboard, Keys.KEY_DOWN, Input.JoystickInput.Axes_LStick_Axis_Y);
            Input.RegisterMapping(Map_Left + Alt_Map_Suffix, Input.KBMInputType.Keyboard, Keys.KEY_LEFT, Input.JoystickInput.Axes_LStick_Axis_X);
            Input.RegisterMapping(Map_Right + Alt_Map_Suffix, Input.KBMInputType.Keyboard, Keys.KEY_RIGHT, Input.JoystickInput.Axes_LStick_Axis_X);
            Input.RegisterMapping(Map_Jump + Alt_Map_Suffix, Input.KBMInputType.Keyboard, Keys.KEY_B, Input.JoystickInput.Button_A);
            Input.RegisterMapping(Map_Interact + Alt_Map_Suffix, Input.KBMInputType.Keyboard, Keys.KEY_M, Input.JoystickInput.Button_X);
            Input.RegisterMapping(Map_PermInteract + Alt_Map_Suffix, Input.KBMInputType.Keyboard, Keys.KEY_N, Input.JoystickInput.Button_B);
        }

        /// <summary>
        /// For mapping, to check if a gamepad's button was pressed for a certain player. If the player's gamepad is not connected, false is returned.
        /// </summary>
        /// <returns>A boolean, true or false</returns>
        public static bool IsPlayerGamepadButtonPressed(string mappingName, bool isPlayerOne)
        {
            if (IsPlayerController(isPlayerOne))
                return Input.GetMappingGamePadButtonPressed(mappingName, isPlayerOne ? PlayerOne_GamepadId : PlayerTwo_GamepadId);
            return false;
        }

        /// <summary>
        /// For mapping, to check if a gamepad's button was pressed for a certain player. If the player's gamepad is not connected, 0.0f is returned.
        /// </summary>
        /// <returns>A float</returns>
        public static float GetPlayerGamepadAxisValue(string mappingName, bool isPlayerOne)
        {
            if (IsPlayerController(isPlayerOne))
                return Input.GetMappingGamePadAxisValue(mappingName, isPlayerOne ? PlayerOne_GamepadId : PlayerTwo_GamepadId);
            return 0.0f;
        }

        /// <summary>
        /// Check if a player is keyboard or controller. True for controller, false for keyboard. Also if a player's controller is not connected, it defaults to false
        /// </summary>
        /// <param name="isPlayerOne">A boolean flag that determines whether it's player one or player two</param>
        /// <returns>A boolean, true or false</returns>
        public static bool IsPlayerController(bool isPlayerOne)
        {
            if (isPlayerOne)
            {
                return PlayerOneInput == InputType.Controller && Input.IsGamepadConnected(PlayerOne_GamepadId);
            }
            else
            {
                return PlayerTwoInput == InputType.Controller && Input.IsGamepadConnected(PlayerTwo_GamepadId);
            }
        }
    }
}
