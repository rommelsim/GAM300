using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public enum Level
    {
        KnockOff,
        PackedTracks
    }
    public class GameMode : AeonBehaviour
    {
        public Level currLevel { get; set; }

        public UI levelOneUI = null;
        public Entity levelTwo = null;
        public UI levelTwoUI = null;

        public Entity redWin = null;
        public Entity blueWin = null;
        public UI redWinUI = null;
        public UI blueWinUI = null;

        public UI redWin2UI = null;
        public UI blueWin2UI = null;

        public UI buttonUI = null;
        public Transform buttonTrans = null;

        // AudioComponent Variable Declaration
        private AudioComponent allButtonPressed = null;
        private bool startPressed = false;
        private bool loadStart = false;
        private DateTime lastTime = DateTime.MinValue;

        public static string selectLeft = "select_Left";
        public static string selectRight = "select_Right";
        public static string altSelectLeft = "alt_Select_Left";
        public static string altSelectRight = "alt_Select_Right";

        public static string confirmSelect = "confirm_Select";
        public static string backButton = "return_Back";
        public static bool unlockedLevel2 = false;
        private static bool redLv1Win = false;
        private static bool blueLv1Win = false;

        private DateTime lastInputTime = DateTime.MinValue;
        private float inputCooldown = 0.3f;
        public override void OnCreate()
        {
            currLevel = Level.KnockOff;
            if (entity != null) { levelOneUI = entity.GetComponent<UI>(); }
            levelTwo = Entity.FindEntityByName("Level2");
            if (levelTwo != null) { levelTwoUI = levelTwo.GetComponent<UI>(); }

            redWin = Entity.FindEntityByName("RedWinner");
            blueWin = Entity.FindEntityByName("BlueWinner");
            if (redWin != null) redWinUI = redWin.GetComponent<UI>();
            if (blueWin != null) blueWinUI = blueWin.GetComponent<UI>();

            buttonUI = Entity.FindEntityByName("StartController").GetComponent<UI>();
            buttonTrans = Entity.FindEntityByName("StartController").GetComponent<Transform>();

            redWin2UI = Entity.FindEntityByName("RedWinner2").GetComponent<UI>();
            blueWin2UI = Entity.FindEntityByName("BlueWinner2").GetComponent<UI>();

            Input.RegisterMapping(confirmSelect, Input.KBMInputType.Keyboard, Keys.KEY_ENTER, Input.JoystickInput.Button_X);

            Input.RegisterMapping(selectLeft, Input.KBMInputType.Keyboard, Keys.KEY_A, Input.JoystickInput.Axes_LStick_Axis_X);
            Input.RegisterMapping(selectRight, Input.KBMInputType.Keyboard, Keys.KEY_D, Input.JoystickInput.Axes_LStick_Axis_X);

            Input.RegisterMapping(altSelectLeft, Input.KBMInputType.Keyboard, Keys.KEY_J, Input.JoystickInput.Button_Dpad_Left);
            Input.RegisterMapping(altSelectRight, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_Dpad_Right);

            //Input.RegisterMapping(backButton, Input.KBMInputType.Keyboard, Keys.KEY_BACKSPACE, Input.JoystickInput.Button_X);

            // Chuck Audio Data From Entity Into Variable
            allButtonPressed = entity.GetComponent<AudioComponent>();
        }
        public override void Update()
        {
            //if ((DateTime.Now - lastInputTime).TotalSeconds >= inputCooldown)
            //{
            //    float p1LeftAxis = GameInputs.GetPlayerGamepadAxisValue(selectRight, true);
            //    float p2LeftAxis = GameInputs.GetPlayerGamepadAxisValue(selectRight, false);

            //    bool moveLeft = false;
            //    bool moveRight = false;

            //    if (p1LeftAxis < 0 || p2LeftAxis < 0)
            //    {
            //        moveLeft = true;
            //    }
            //    else if (p1LeftAxis > 0 || p2LeftAxis > 0)
            //    {
            //        moveRight = true;
            //    }

            //    if (moveLeft == true || moveRight == true || Input.GetMappingKeyPressed(altSelectLeft) || Input.GetMappingKeyPressed(altSelectRight)
            //        || GameInputs.IsPlayerGamepadButtonPressed(altSelectLeft, true) || GameInputs.IsPlayerGamepadButtonPressed(altSelectRight, true) || GameInputs.IsPlayerGamepadButtonPressed(altSelectLeft, false) || GameInputs.IsPlayerGamepadButtonPressed(altSelectRight, false)) //((Input.GetKeyHeld(Keys.KEY_A) || Input.GetKeyHeld(Keys.KEY_D)) && TimeCooldown()) 
            //    {
            //        if (allButtonPressed != null)
            //        {
            //            allButtonPressed.PlaySound();
            //            NativeLog.Log("Selection Audio!");
            //        }

            //        NativeLog.Log("a button is pressed");
            //        NativeLog.Log("Current Enum Value:" + currLevel.ToString());
            //        if (currLevel == Level.KnockOff)
            //        {
            //            currLevel = Level.PackedTracks;
            //            levelOneUI.Texture = ".\\resources\\textures\\TheKnockOff_Unselected.dds";

            //            levelTwoUI.Texture = ".\\resources\\textures\\PackedTracks1Selected.dds";
            //        }
            //        else if (currLevel == Level.PackedTracks)
            //        {
            //            currLevel = Level.KnockOff;
            //            levelOneUI.Texture = ".\\resources\\textures\\TheKnockOff_Selected.dds";

            //            levelTwoUI.Texture = ".\\resources\\textures\\PackedTracks1Unselected.dds";
            //        }
            //        lastInputTime = DateTime.Now;
            //    }
            //}
            if (GameData.level1Clear)
            {
                levelOneUI.Texture = ".\\resources\\textures\\TheKnockOffCompleted.dds";


                levelTwoUI.Texture = ".\\resources\\textures\\PackedTracks1Selected.dds";
                currLevel = Level.PackedTracks;
                // if (redWinUI.Opacity == 0.0f && blueWinUI.Opacity == 0.0f)
                //{
                if (!GameData.isLevel2)
                {
                    redLv1Win = GameData.RedWon;
                    blueLv1Win = GameData.BlueWon;
                }

                if (redLv1Win)
                {
                    redWinUI.Opacity = 1.0f;
                }
                else if (blueLv1Win)
                {
                    redWinUI.Texture = ".\\resources\\textures\\BlueWinner.dds";
                    redWinUI.Opacity = 1.0f;
                }
                else if (!redLv1Win && !blueLv1Win)
                {
                    redWinUI.Opacity = 1.0f;
                    blueWinUI.Opacity = 1.0f;
                }
                //}
            }

            if(GameData.level2Clear)
            {
                //levelTwoUI.Texture = ".\\resources\\textures\\PackedTracks1Selected.dds";
                levelOneUI.Texture = ".\\resources\\textures\\TheKnockOffCompleted.dds";

                levelTwoUI.Texture = ".\\resources\\textures\\PackedTracksCompleted.dds";
                buttonTrans.Position = new Vector3(600f, buttonTrans.Position.Y, buttonTrans.Position.Z);
                buttonUI.Texture = ".\\resources\\textures\\ChampionButton.dds";

                //if (redLv1Win)
                //{
                //    redWinUI.Opacity = 1f;
                //}
                //else if (GameData.P2_L1_Score == GameData.P1_L1_Score)
                //{
                //    redWinUI.Opacity = 1f;
                //    blueWinUI.Opacity = 1f;
                //}
                //else if (blueLv1Win)
                //{
                //    blueWinUI.Opacity = 1f;

                //}


                if (GameData.RedWon)
                {
                    redWin2UI.Opacity = 1.0f;
                }
                else if (GameData.BlueWon)
                {
                    redWin2UI.Texture = ".\\resources\\textures\\BlueWinner.dds";
                    redWin2UI.Opacity = 1.0f;
                }
                else if (!GameData.RedWon && !GameData.BlueWon)
                {
                    redWin2UI.Opacity = 1.0f;
                    blueWin2UI.Opacity = 1.0f;
                }

/*                if (GameData.RedWon || redLv1Win)
                {
                    redWinUI.Opacity = 1.0f;
                    redLv1Win = true;
                }
                else if (GameData.BlueWon || blueLv1Win)
                {
                    redWinUI.Texture = ".\\resources\\textures\\BlueWinner.dds";
                    redWinUI.Opacity = 1.0f;
                    blueLv1Win = true;
                }
                else if (!GameData.RedWon && !GameData.BlueWon)
                {
                    redWinUI.Opacity = 1.0f;
                    blueWinUI.Opacity = 1.0f;
                }
*/            }

            

            if (Input.GetMappingKeyPressed(confirmSelect) || GameInputs.IsPlayerGamepadButtonPressed(confirmSelect, true) || GameInputs.IsPlayerGamepadButtonPressed(confirmSelect, false)) //(Input.GetKeyPressed(Keys.KEY_J) || Input.GetKeyPressed(Keys.KEY_K))
            {
                if (currLevel == Level.KnockOff && !startPressed)
                {
                    GameData.isLevel1 = true;
                    if (allButtonPressed != null)
                    {
                        allButtonPressed.SoundFilepath = ".\\resources\\audios\\MainMenuButtonPressOnly.wav";
                        allButtonPressed.PlaySound();
                    }

                    //SceneManager.LoadScene(".\\resources\\scene\\V1_HOW_TO_PLAY.scene");
                    startPressed = true;
                }
                else if (currLevel == Level.PackedTracks && !startPressed)
                {
                    GameData.isLevel2 = true;
                    unlockedLevel2 = true;
                    startPressed = true;
                }
            }
            if (startPressed && !loadStart)
            {
                lastTime = DateTime.Now;
                loadStart = true;
            }
            if (!GameData.level1Clear || !GameData.level2Clear)
            {
                if (loadStart && (DateTime.Now - lastTime).TotalSeconds >= 0.5f)
                {
                    SceneManager.LoadScene(".\\resources\\scene\\V1_HOW_TO_PLAY.scene");
                }
            }
            if(loadStart && GameData.level2Clear)
            {
                GameData.isLevel2 = false;
                SceneManager.LoadScene(".\\resources\\scene\\V1_GRAND_WINNER_MENU.scene");

            }
            //if (Input.GetMappingKeyPressed(backButton) || Input.GetMappingGamePadButtonPressed(backButton, gamePadOne))
            //{
            //    SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene");
            //}
        }
    }
}