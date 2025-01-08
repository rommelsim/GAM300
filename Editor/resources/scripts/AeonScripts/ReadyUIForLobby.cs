using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class ReadyUIForLobby : AeonBehaviour
    {
        public UI button = null;
        public static string btn1 = "Player1JoinButton";
        public static string btn2 = "Player2Join_Button";
        public bool firstpressplayer1 = false;
        public bool firstpressplayer2 = false;
        public static string Map_Join = "Map_Join";
        public override void OnCreate()
        {
            Input.RegisterMapping(Map_Join, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_A);
            if(button == null) button = entity.GetComponent<UI>();
        }

        public override void Update()
        {
            int gamepadOne = 0;
            bool isGamepadConnect = Input.GetConnectedGamepadIds().Length > 0;
            if(isGamepadConnect)
            {
                gamepadOne = Input.GetConnectedGamepadIds()[0];
            }

            if(GameInputs.PlayerOneInput == GameInputs.InputType.Controller)
            {
                // key press change to gamepad is a placeholder cuz i can't test
                if(Input.GetMappingGamePadButtonPressed(Map_Join, gamepadOne) && !firstpressplayer1)
                {
                    firstpressplayer1 = true;
                    if(entity.Name == btn1)
                    {
                        button.Texture = ".\\resources\\textures\\Ready LeftSide(Controller).dds";
                    }
                }
                // key press change to gamepad is a placeholder cuz i can't test
                else if (Input.GetMappingGamePadButtonPressed(Map_Join, gamepadOne) && firstpressplayer1)
                {
                    if(entity.Name == btn1)
                    {
                        button.Texture = ".\\resources\\textures\\Ready LeftSide (Keyboard).dds";
                    }
                }
            }

            else if (GameInputs.PlayerTwoInput == GameInputs.InputType.Controller)
            {
                // key press change to gamepad is a placeholder cuz i can't test
                if (Input.GetMappingGamePadButtonPressed(Map_Join, gamepadOne) && !firstpressplayer2)
                {
                    // if the player joins with gamepadbutton A, show the Ready Keyboard button
                    firstpressplayer2 = true;
                    if (entity.Name == btn2)
                    {
                        button.Texture = ".\\resources\\textures\\Ready RightSide(Controller).dds";
                    }
                }
                // key press change to gamepad is a placeholder cuz i can't test
                else if (Input.GetMappingGamePadButtonPressed(Map_Join, gamepadOne) && firstpressplayer2)
                {
                    // player2 presses gamepad button A again and the ready button changes to blue ver
                    if (entity.Name == btn2)
                    {
                        button.Texture = ".\\resources\\textures\\Ready RightSide (BluePlayer).dds";
                    }
                }
            }

        }
    }
}
