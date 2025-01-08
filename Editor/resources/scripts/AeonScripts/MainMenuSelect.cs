using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public enum CheckButton
    {
        StartButton,
        OptionsButton,
        CreditsButton
    }
    public class MainMenuSelect : AeonBehaviour
    {
        public ConfirmOverlay quitConfirm { get; set; }
        // confirm overlay
        public Entity overlay = null;
        public Entity yes = null;
        public Entity no = null;

        public UI overlayUI = null;
        public UI yesUI = null;
        public UI noUI = null;

        public Entity start = null;
        public Entity end = null;
        public Entity credits = null;

        public Transform arrowTrans = null;
        public Transform startTrans = null;
        public Transform endTrans = null;
        public Transform creditsTrans = null;

        public UI startUI = null;
        public UI endUI = null;
        public UI creditsUI = null;

        // AudioComponent Variable Declaration
        private AudioComponent allButtonPressedSFX = null;
        // To delay 0.5 seconds to play the start audio
        private DateTime lastTime = DateTime.MinValue;
        private bool loadStart = false;
        private bool startPressed = false;

        public CheckButton currButton { get; set; }

        public static string selectLeft = "select_Left";
        public static string selectRight = "select_Right";
        public static string altSelectLeft = "alt_Select_Left";
        public static string altSelectRight = "alt_Select_Right";

        //public static string confirmSelectA = "confirm_SelectA";
        public static string confirmSelectX = "confirm_SelectX";
        //public static string confirmSelectY = "confirm_SelectY";
        public static string quitSelectB = "quit_SelectB";

        private DateTime lastInputTime = DateTime.MinValue;
        private float inputCooldown = 0.3f;

        public static int gamepad;
        private bool isOverlay = false;

        private Entity backButtonEnt = null;
        bool buttonPress = false;
        float buttonPressDelay = 0.8f;
        public override void OnCreate()
        {
            backButtonEnt = Entity.FindEntityByName("BackBox");
            start = Entity.FindEntityByName("StartButton");
            end = Entity.FindEntityByName("OptionsButton");
            credits = Entity.FindEntityByName("CreditsButton");

            if (start != null) startTrans = start.GetComponent<Transform>();
            if (end != null) endTrans = end.GetComponent<Transform>();
            if (credits != null) creditsTrans = credits.GetComponent<Transform>();
            if (start != null) startUI = start.GetComponent<UI>();
            if (end != null) endUI = end.GetComponent<UI>();
            if (credits != null) creditsUI = credits.GetComponent<UI>();

            // confirm overlay
            overlay = Entity.FindEntityByName("ConfirmOverlay");
            yes = Entity.FindEntityByName("ConfirmYes");
            no = Entity.FindEntityByName("ConfirmNo");

            if (overlay != null) overlayUI = overlay.GetComponent<UI>();
            if (yes != null) yesUI = yes.GetComponent<UI>();
            if (no != null) noUI = no.GetComponent<UI>();

            Input.RegisterMapping(selectLeft, Input.KBMInputType.Keyboard, Keys.KEY_A, Input.JoystickInput.Axes_LStick_Axis_X);
            Input.RegisterMapping(selectRight, Input.KBMInputType.Keyboard, Keys.KEY_D, Input.JoystickInput.Axes_LStick_Axis_X);

            Input.RegisterMapping(altSelectLeft, Input.KBMInputType.Keyboard, Keys.KEY_J, Input.JoystickInput.Button_Dpad_Left);
            Input.RegisterMapping(altSelectRight, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_Dpad_Right);

            //Input.RegisterMapping(confirmSelectA, Input.KBMInputType.Keyboard, Keys.KEY_ENTER, Input.JoystickInput.Button_A);
            Input.RegisterMapping(confirmSelectX, Input.KBMInputType.Keyboard, Keys.KEY_ENTER, Input.JoystickInput.Button_X);
            //Input.RegisterMapping(confirmSelectY, Input.KBMInputType.Keyboard, Keys.KEY_ENTER, Input.JoystickInput.Button_Y);
            Input.RegisterMapping(quitSelectB, Input.KBMInputType.Keyboard, Keys.KEY_ESCAPE, Input.JoystickInput.Button_B);


            currButton = CheckButton.StartButton;
            quitConfirm = ConfirmOverlay.No;
            NativeLog.Log("start Enum" + currButton);

            // Chuck Audio Data From Entity Into Variable
            allButtonPressedSFX = entity.GetComponent<AudioComponent>();
        }

        public override void Update()
        {
            if (BgmMainMenu.menuBGM != null && BgmMainMenu.menuBGM.IsPaused())
            {
                BgmMainMenu.menuBGM.ResumeSound();
            }

            if (startTrans == null)
            {
                startTrans = start.GetComponent<Transform>();
            }
            if (endTrans == null)
            {
                endTrans = end.GetComponent<Transform>();
            }
            if (startUI == null)
            {
                startUI = start.GetComponent<UI>();
            }
            if (endUI == null)
            {
                endUI = end.GetComponent<UI>();
            }

            bool isGamepadConnected = Input.GetConnectedGamepadIds().Length > 0;
            int gamePadOne = 0;
            int gamePadTwo = 0;
            if (isGamepadConnected)
            {
                gamePadOne = Input.GetConnectedGamepadIds()[0];

                if (Input.GetConnectedGamepadIds().Length > 1)
                {
                    gamePadTwo = Input.GetConnectedGamepadIds()[1];
                }
            }
            float leftRightAxisOne = Input.GetMappingGamePadAxisValue(selectLeft, gamePadOne);
            float leftRightAxisTwo = Input.GetMappingGamePadAxisValue(selectLeft, gamePadTwo);

            bool moveLeftOne = false;
            bool moveRightOne = false;

            bool moveLeftTwo = false;
            bool moveRightTwo = false;

            if ((isGamepadConnected && leftRightAxisOne < 0) || (isGamepadConnected && leftRightAxisTwo < 0))
            {
                moveLeftOne = true;
                moveLeftTwo = true;
            }
            else if ((isGamepadConnected && leftRightAxisOne > 0) || (isGamepadConnected && leftRightAxisTwo > 0))
            {
                moveRightOne = true;
                moveRightTwo = true;
            }
            if (!isOverlay)
            {
                overlayUI.Opacity = 0.0f;
                yesUI.Opacity = 0.0f;
                noUI.Opacity = 0.0f;
                if ((DateTime.Now - lastInputTime).TotalSeconds >= inputCooldown)
                {
                    if (moveLeftOne == true || moveLeftTwo == true || Input.GetMappingKeyPressed(altSelectLeft) || Input.GetMappingGamePadButtonPressed(altSelectLeft, gamePadOne)
                        || Input.GetMappingGamePadButtonPressed(altSelectLeft, gamePadTwo)) //((Input.GetKeyHeld(Keys.KEY_A) || Input.GetKeyHeld(Keys.KEY_D)) && TimeCooldown()) 
                    {
                        if (allButtonPressedSFX != null)
                        {
                            // Play SFX Audio When D-Pad and Joy-Pad Is Pressed
                            allButtonPressedSFX.SoundFilepath = ".\\resources\\audios\\MainMenuButtonPressOnly.wav";
                            allButtonPressedSFX.PlaySound();
                            NativeLog.Log("SFX is Played when D-Pad Button is Pressed");
                        }

                        NativeLog.Log("a button is pressed");
                        NativeLog.Log("Current Enum Value:" + currButton.ToString());
                        if (currButton == CheckButton.StartButton)
                        {
                            currButton = CheckButton.CreditsButton;
                            startUI.Texture = ".\\resources\\textures\\Start-Game-Button-_Unselected_.dds";

                            creditsUI.Texture = ".\\resources\\textures\\CreditButton_Selected_.dds";

                            backButtonEnt.GetComponent<Transform>().Position = creditsTrans.Position;

                            Vector3 newStartScale = startTrans.Scale;
                            Vector3 newEndScale = endTrans.Scale;

                            SetScale(ref newEndScale, ref newStartScale);
                            startTrans.Scale = newStartScale;
                            creditsTrans.Scale = newEndScale;
                        }
                        else if (currButton == CheckButton.OptionsButton)
                        {
                            currButton = CheckButton.StartButton;
                            startUI.Texture = ".\\resources\\textures\\Start-Game-Button-_Selected_.dds";

                            endUI.Texture = ".\\resources\\textures\\Options (Unselected).dds";

                            backButtonEnt.GetComponent<Transform>().Position = startTrans.Position;

                            Vector3 newStartScale = startTrans.Scale;
                            Vector3 newEndScale = endTrans.Scale;

                            SetScale(ref newStartScale, ref newEndScale);
                            startTrans.Scale = newStartScale;
                            endTrans.Scale = newEndScale;
                        }
                        lastInputTime = DateTime.Now;
                    }
                    else if (moveRightOne == true || moveRightTwo == true || Input.GetMappingKeyPressed(altSelectRight) || Input.GetMappingGamePadButtonPressed(altSelectRight, gamePadOne)
                        || Input.GetMappingGamePadButtonPressed(altSelectRight, gamePadTwo))
                    {
                        if (allButtonPressedSFX != null)
                        {
                            // Play SFX Audio When D-Pad and Joy-Pad Is Pressed
                            allButtonPressedSFX.PlaySound();
                            NativeLog.Log("SFX is Played when D-Pad Button is Pressed");
                        }

                        NativeLog.Log("a button is pressed");
                        NativeLog.Log("Current Enum Value:" + currButton.ToString());
                        if (currButton == CheckButton.StartButton)
                        {
                            currButton = CheckButton.OptionsButton;
                            startUI.Texture = ".\\resources\\textures\\Start-Game-Button-_Unselected_.dds";

                            endUI.Texture = ".\\resources\\textures\\Options (Selected).dds";

                            backButtonEnt.GetComponent<Transform>().Position = endTrans.Position;

                            Vector3 newStartScale = startTrans.Scale;
                            Vector3 newEndScale = endTrans.Scale;

                            SetScale(ref newEndScale, ref newStartScale);
                            startTrans.Scale = newStartScale;
                            endTrans.Scale = newEndScale;
                        }
                        else if (currButton == CheckButton.CreditsButton)
                        {
                            currButton = CheckButton.StartButton;
                            startUI.Texture = ".\\resources\\textures\\Start-Game-Button-_Selected_.dds";

                            creditsUI.Texture = ".\\resources\\textures\\CreditButton_Unselected_.dds";

                            backButtonEnt.GetComponent<Transform>().Position = startTrans.Position;

                            Vector3 newStartScale = startTrans.Scale;
                            Vector3 newEndScale = endTrans.Scale;

                            SetScale(ref newStartScale, ref newEndScale);
                            startTrans.Scale = newStartScale;
                            creditsTrans.Scale = newEndScale;
                        }
                        lastInputTime = DateTime.Now;
                    }
                }

                if (Input.GetMappingGamePadButtonPressed(confirmSelectX, gamePadOne))
                {
                    gamepad = gamePadOne;
                }
                else if (Input.GetMappingGamePadButtonPressed(confirmSelectX, gamePadTwo))
                {
                    gamepad = gamePadTwo;
                }

                if (Input.GetMappingKeyPressed(confirmSelectX) || Input.GetMappingGamePadButtonPressed(confirmSelectX, gamePadOne) || Input.GetMappingGamePadButtonPressed(confirmSelectX, gamePadTwo))
                {
                    buttonPress = true;

                    if (allButtonPressedSFX != null)
                    {
                        // Play SFX Audio When ABXY Is Pressed
                        allButtonPressedSFX.SoundFilepath = ".\\resources\\audios\\AllOtherButtonPress.wav";
                        allButtonPressedSFX.PlaySound();
                        NativeLog.Log("SFX is Played when Joy-Pad Button is Pressed");
                    }

                    NativeLog.Log("confirmSelect pressed");
                }

                if (Input.GetMappingGamePadButtonPressed(quitSelectB, gamePadOne) || Input.GetMappingKeyPressed(quitSelectB)
                        || Input.GetMappingGamePadButtonPressed(quitSelectB, gamePadTwo))
                {
                    isOverlay = true;
                    overlayUI.Opacity = 1.0f;
                    yesUI.Opacity = 1.0f;
                    noUI.Opacity = 1.0f;
                }
                if (!loadStart && startPressed)
                {
                    lastTime = DateTime.Now;
                    loadStart = true;
                    buttonPress = false;
                    buttonPressDelay = 0.8f;
                }
                if (loadStart && (DateTime.Now - lastTime).TotalSeconds >= 0.5f)
                {
                    SceneManager.LoadScene(".\\resources\\scene\\V1_LOBBY.scene");
                    loadStart = false;
                    lastTime = DateTime.MinValue;
                }

                if (buttonPress)
                {
                    buttonPressDelay -= Time.DeltaTime;
                    backButtonEnt.GetComponent<Transform>().Scale = Vector3.Lerp(backButtonEnt.GetComponent<Transform>().Scale, new Vector3(273, 300, 1), 0.12f);

                    if (currButton == CheckButton.StartButton && buttonPressDelay <= 0.5f)
                    {
                        startTrans.Scale = Vector3.Lerp(new Vector3(265, 265, 0), new Vector3(240, 240, 0), 0.5f);
                        backButtonEnt.GetComponent<Transform>().Scale = new Vector3(165, 200, 0);
                    }
                    else if (currButton == CheckButton.OptionsButton && buttonPressDelay <= 0.5f)
                    {
                        endTrans.Scale = Vector3.Lerp(new Vector3(270, 270, 0), new Vector3(250, 250, 0), 0.5f);
                        backButtonEnt.GetComponent<Transform>().Scale = new Vector3(165, 200, 0);
                    }
                    else if (currButton == CheckButton.CreditsButton && buttonPressDelay <= 0.5f)
                    {
                        creditsTrans.Scale = Vector3.Lerp(new Vector3(270, 270, 0), new Vector3(250, 250, 0), 0.5f);
                        backButtonEnt.GetComponent<Transform>().Scale = new Vector3(165, 200, 0);
                    }
                    else if (currButton == CheckButton.StartButton && buttonPressDelay <= 0.8f)
                    {
                        startTrans.Scale = new Vector3(265, 265, 0);
                    }
                    else if (currButton == CheckButton.OptionsButton && buttonPressDelay <= 0.8f)
                    {
                        endTrans.Scale = new Vector3(270, 270, 0);

                    }
                    else if (currButton == CheckButton.CreditsButton && buttonPressDelay <= 0.8f)
                    {
                        creditsTrans.Scale = new Vector3(270, 270, 0);
                    }
                }

                if (currButton == CheckButton.StartButton && !startPressed && buttonPressDelay <= 0f)
                {
                    NativeLog.Log("Before LoadScene");
                    //SceneManager.LoadScene(".\\resources\\scene\\V1_LOBBY.scene");
                    startPressed = true;

                }
                else if (currButton == CheckButton.OptionsButton && buttonPressDelay <= 0f)
                {
                    // quit game
                    //GameState.QuitGame();
                    SceneManager.LoadScene(".\\resources\\scene\\V1_OPTIONS_V3.scene");
                    buttonPress = false;
                    buttonPressDelay = 0.8f;
                }
                else if (currButton == CheckButton.CreditsButton && buttonPressDelay <= 0f)
                {
                    SceneManager.LoadScene(".\\resources\\scene\\V1_CREDITS.scene");
                    buttonPress = false;
                    buttonPressDelay = 0.8f;
                }
            }
            else if (isOverlay)
            {
                if ((DateTime.Now - lastInputTime).TotalSeconds >= inputCooldown)
                {
                    if (moveLeftOne == true || moveLeftTwo == true || Input.GetMappingKeyPressed(altSelectLeft) || Input.GetMappingGamePadButtonPressed(altSelectLeft, gamePadOne)
                        || Input.GetMappingGamePadButtonPressed(altSelectLeft, gamePadTwo))
                    {
                        if (quitConfirm == ConfirmOverlay.No)
                        {
                            quitConfirm = ConfirmOverlay.Yes;

                            noUI.Texture = ".\\resources\\textures\\Nounselected.dds";
                            yesUI.Texture = ".\\resources\\textures\\Yesselected.dds";
                        }
                    }
                    else if (moveRightOne == true || moveRightTwo == true || Input.GetMappingKeyPressed(altSelectRight) || Input.GetMappingGamePadButtonPressed(altSelectRight, gamePadOne)
                         || Input.GetMappingGamePadButtonPressed(altSelectRight, gamePadTwo))
                    {
                        if (quitConfirm == ConfirmOverlay.Yes)
                        {
                            quitConfirm = ConfirmOverlay.No;

                            noUI.Texture = ".\\resources\\textures\\NoSelected.dds";
                            yesUI.Texture = ".\\resources\\textures\\YesUnselected.dds";
                        }
                    }
                    if (Input.GetMappingGamePadButtonPressed(confirmSelectX, gamePadOne) || Input.GetMappingKeyPressed(confirmSelectX)
                        || Input.GetMappingGamePadButtonPressed(confirmSelectX, gamePadTwo))
                    {
                        if (quitConfirm == ConfirmOverlay.No)
                        {
                            isOverlay = false;
                        }
                        if (quitConfirm == ConfirmOverlay.Yes)
                        {
                            isOverlay = false;
                            GameState.QuitGame();
                        }
                    }
                }
            }
        }

        public void SetScale(ref Vector3 big, ref Vector3 small)
        {
            big.X = 250;
            big.Y = 250;
            small.X = 220;
            small.Y = 220;
        }
    }
}
