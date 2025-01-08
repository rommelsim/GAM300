using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AeonScriptCore;

namespace AeonScripts
{
    public enum PauseMenuOption
    {
        None,
        Resume,
        Restart,
        Settings,
        Quit
    }

    public enum ConfirmOverlay
    {
        No,
        Yes
    }

    public class PauseMenu : AeonBehaviour
    {
        public PauseMenuOption currPoption { get; set; }
        public ConfirmOverlay quitConfirm {  get; set; }

        // pause menu
        public Entity resumeBtn = null;
        public Entity restartBtn = null;
        public Entity settingsBtn = null;
        public Entity quitBtn = null;

        public UI uiresumeBtn = null;
        public UI uirestartBtn = null;
        public UI uisettingsBtn = null;
        public UI uiquitBtn = null;
        public UI uibg = null;

        // confirm overlay
        public Entity overlay = null;
        public Entity yes = null;
        public Entity no = null;
        
        public UI overlayUI = null;
        public UI yesUI = null;
        public UI noUI = null;

        // game UI
        public Entity score = null;
        public Entity redScore = null;
        public Entity blueScore = null;
        public Entity time = null;
        public Entity ent1 = null;
        public Entity ent2 = null;
        public Entity child1 = null;
        public Entity child2 = null;
        public Entity permRed = null;
        public Entity permBlue = null;
        public Entity permControlsRed = null;
        public Entity permControlsBlue = null;
        public Entity collectRed = null;
        public Entity collectBlue = null;

        public UI scoreUI = null;
        public UI redUI = null;
        public UI blueUI = null;
        public UI timeUI = null;
        public UI ui1 = null;
        public UI ui2 = null;
        public UI child1UI = null;
        public UI child2UI = null;
        public UI permRedUI = null;
        public UI permBlueUI = null;
        public UI permControlsRedUI = null;
        public UI permControlsBlueUI = null;
        public UI collectRedUI = null;
        public UI collectBlueUI = null;

        // bgm
        public Entity bgm = null;
        public AudioComponent bgmSound = null;

        public static string selectUp = "select_Up";
        public static string selectDown = "select_Down";
        public static string altSelectUp = "alt_Select_Up";
        public static string altSelectDown = "alt_Select_Down";
        public static string selectLeftYes = "select_Left_Yes";
        public static string selectRightNo = "select_Right_No";
        public static string confirm_option = "confirm_option";
        public static string pause = "pause";

        private DateTime lastInputTime = DateTime.MinValue;
        private float inputCooldown = 0.3f;
        private float _timer = 0f;
        private int seconds = 0;

        private bool menuShown = false;
        private bool isPaused = false;
        private bool isOverlay = false;
        public static int gamepad;

        public override void OnCreate()
        {
            // pause menu
            resumeBtn = Entity.FindEntityByName("ResumeButton");
            restartBtn = Entity.FindEntityByName("RestartButton");
            settingsBtn = Entity.FindEntityByName("SettingsButton");
            quitBtn = Entity.FindEntityByName("QuitButton");

            if (resumeBtn != null) uiresumeBtn = resumeBtn.GetComponent<UI>();
            if (restartBtn != null) uirestartBtn = restartBtn.GetComponent<UI>();
            if (settingsBtn != null) uisettingsBtn = settingsBtn.GetComponent<UI>();
            if (quitBtn != null) uiquitBtn = quitBtn.GetComponent<UI>();
            if (entity != null) uibg = entity.GetComponent<UI>();

            // confirm overlay
            overlay = Entity.FindEntityByName("ConfirmOverlay");
            yes = Entity.FindEntityByName("ConfirmYes");
            no = Entity.FindEntityByName("ConfirmNo");

            if (overlay != null) overlayUI = overlay.GetComponent<UI>();
            if (yes != null) yesUI = yes.GetComponent<UI>();
            if (no != null) noUI = no.GetComponent<UI>();

            // game ui
            score = Entity.FindEntityByName("ScoreUI");
            redScore = Entity.FindEntityByName("CurrentRedPoint");
            blueScore = Entity.FindEntityByName("CurrentBluePoint");
            time = Entity.FindEntityByName("CurrentTimeInGame");
            ent1 = Entity.FindEntityByName("uiEntity");
            ent2 = Entity.FindEntityByName("uiEntity2");
            permRed = Entity.FindEntityByName("PermRedHammerUI");
            permBlue = Entity.FindEntityByName("PermBlueHammerUI");
            permControlsRed = Entity.FindEntityByName("PermControlsRedUI");
            permControlsBlue = Entity.FindEntityByName("PermControlsBlueUI");
            collectRed = Entity.FindEntityByName("CollectControlsRedUI");
            collectBlue = Entity.FindEntityByName("CollectControlsBlueUI");

            if (score != null) scoreUI = score.GetComponent<UI>();
            if (redScore != null) redUI = redScore.GetComponent<UI>();
            if (blueScore != null) blueUI = blueScore.GetComponent<UI>();
            if (time != null) timeUI = time.GetComponent<UI>();
            if (ent1 != null) ui1 = ent1.GetComponent<UI>();
            if (ent2 != null) ui2 = ent2.GetComponent<UI>();
            if (permRed != null) permRedUI = permRed.GetComponent<UI>();
            if (permBlue != null) permBlueUI = permBlue.GetComponent<UI>();
            if (collectRed != null) collectRedUI = collectRed.GetComponent<UI>();
            if (collectBlue != null) collectBlueUI = collectBlue.GetComponent<UI>();
            if (permControlsRed != null) permControlsRedUI = permControlsRed.GetComponent<UI>();
            if (permControlsBlue != null) permControlsBlueUI = permControlsBlue.GetComponent<UI>();

            bgm = Entity.FindEntityByName("InGameBGM");
            if (bgm != null) bgmSound = bgm.GetComponent<AudioComponent>();

            //Input.RegisterMapping(selectUp, Input.KBMInputType.Keyboard, Keys.KEY_P, Input.JoystickInput.Axes_LStick_Axis_Y);
            //Input.RegisterMapping(selectDown, Input.KBMInputType.Keyboard, Keys.KEY_L, Input.JoystickInput.Axes_LStick_Axis_Y);
            Input.RegisterMapping(altSelectUp, Input.KBMInputType.Keyboard, Keys.KEY_N, Input.JoystickInput.Button_Dpad_Up);
            Input.RegisterMapping(altSelectDown, Input.KBMInputType.Keyboard, Keys.KEY_M, Input.JoystickInput.Button_Dpad_Down);

            Input.RegisterMapping(selectLeftYes, Input.KBMInputType.Keyboard, Keys.KEY_J, Input.JoystickInput.Button_Dpad_Left);
            Input.RegisterMapping(selectRightNo, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_Dpad_Right);

            Input.RegisterMapping(confirm_option, Input.KBMInputType.Keyboard, Keys.KEY_ENTER, Input.JoystickInput.Button_X);

            Input.RegisterMapping(pause, Input.KBMInputType.Keyboard, Keys.KEY_ESCAPE, Input.JoystickInput.Button_Start);

            currPoption = PauseMenuOption.Resume;
            quitConfirm = ConfirmOverlay.No;
            NativeLog.Log("OnCreate");
        }

        public override void OnCachedSceneActive()
        {
            base.OnCachedSceneActive();
        }

        public override void OnCachedSceneInactive()
        {
            base.OnCachedSceneInactive();
        }

        public override void Update()
        {
            //bgmSound.ResumeSound();
            //NativeLog.Log("GameState.isPaused = " + GameState.IsPaused.ToString());
            //NativeLog.Log("GameEngine: " + GameState.GetEnginePauseState().ToString());
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
            _timer += Time.DeltaTime;
            seconds = (int)(_timer % 60);

            if (seconds > 4)
            {              
                if (Input.GetMappingKeyPressed(pause))
                {
                    isPaused = true;
                }
                else if (Input.GetMappingGamePadButtonPressed(pause, gamePadOne)) 
                {
                    gamepad = gamePadOne;
                    isPaused = true;
                }
                else if (Input.GetMappingGamePadButtonPressed(pause, gamePadTwo))
                {
                    gamepad = gamePadTwo;
                    isPaused = true;
                }
                //NativeLog.Log(GameState.GetEnginePauseState().ToString());

                if (isPaused)
                {
                    GameState.IsPaused = true;
                    GameState.SetEnginePauseState(true);
                    ShowPauseUI();
                    HideGameUI();
                    Audio.GamePauseAllSounds();
                    menuShown = true;
                }
                else
                {
                    GameState.IsPaused = false;
                    GameState.SetEnginePauseState(false);
                    HidePauseUI();
                    ShowGameUI();
                    Audio.GameUnpauseAllSounds();
                    menuShown = false;
                }
                // NativeLog.Log("Pause Menu activated");
            }
            else
            {
                return;
            }
            bgmSound.ResumeSound();

            //NativeLog.Log(menuShown.ToString());

            if (GameState.IsPaused && menuShown)
            {               
                if ((DateTime.Now - lastInputTime).TotalSeconds >= inputCooldown)
                {

                    //float upDownAxisOne = Input.GetMappingGamePadAxisValue(selectUp, gamePadOne);
                    //float upDownAxisTwo = Input.GetMappingGamePadAxisValue(selectUp, gamePadTwo);

                    //bool moveUpOne = false;
                    //bool moveDownOne = false;

                    //bool moveUpTwo = false;
                    //bool moveDownTwo = false;

                    //if ((isGamepadConnected && upDownAxisOne < 0) || (isGamepadConnected && upDownAxisTwo < 0))
                    //{
                    //    moveUpOne = true;
                    //    moveUpTwo = true;
                    //}
                    //else if ((isGamepadConnected && upDownAxisOne > 0) || (isGamepadConnected && upDownAxisTwo > 0))
                    //{
                    //    moveDownOne = true;
                    //    moveDownTwo = true;
                    //}
                    RunPause(gamepad);
                }
            }
        }

        public void ShowPauseUI()
        {
            uiresumeBtn.Opacity = 1.0f;
            uirestartBtn.Opacity = 1.0f;
            uisettingsBtn.Opacity = 1.0f;
            uiquitBtn.Opacity = 1.0f;
            uibg.Opacity = 1.0f;
        }

        public void HidePauseUI()
        {
            uiresumeBtn.Opacity = 0.0f;
            uirestartBtn.Opacity = 0.0f;
            uisettingsBtn.Opacity = 0.0f;
            uiquitBtn.Opacity = 0.0f;
            uibg.Opacity = 0.0f;
        }

        public void PauseUIHalf()
        {
            uiresumeBtn.Opacity = 0.7f;
            uirestartBtn.Opacity = 0.7f;
            uisettingsBtn.Opacity = 0.7f;
            uiquitBtn.Opacity = 0.7f;
            //uibg.Opacity = 0.5f;
        }

        public void ShowGameUI()
        {
            scoreUI.Opacity = 1.0f;
            redUI.Opacity = 1.0f;
            blueUI.Opacity = 1.0f;
            timeUI.Opacity = 1.0f;
            ui1.Opacity = 1.0f;
            ui2.Opacity = 1.0f;
            permRedUI.Opacity = 1.0f;
            permBlueUI.Opacity = 1.0f;
            permControlsRedUI.Opacity = 1.0f;
            permControlsBlueUI.Opacity = 1.0f;
            collectRedUI.Opacity = 1.0f;
            collectBlueUI.Opacity = 1.0f;
        }
        public void HideGameUI()
        {
            scoreUI.Opacity = 0.0f;
            redUI.Opacity = 0.0f;
            blueUI.Opacity = 0.0f;
            timeUI.Opacity = 0.0f;
            ui1.Opacity = 0.0f;
            ui2.Opacity = 0.0f;
            permRedUI.Opacity = 0.0f;
            permBlueUI.Opacity = 0.0f;
            permControlsRedUI.Opacity = 0.0f;
            permControlsBlueUI.Opacity = 0.0f;
            collectRedUI.Opacity = 0.0f;
            collectBlueUI.Opacity = 0.0f;
        }

        public void RunPause(int gamepad)
        {
            if (!isOverlay)
            {
                ShowPauseUI();
                overlayUI.Opacity = 0.0f;
                yesUI.Opacity = 0.0f;
                noUI.Opacity = 0.0f;
                if (Input.GetMappingKeyPressed(altSelectUp) || Input.GetMappingGamePadButtonPressed(altSelectUp, gamepad))
                {
                    if (currPoption == PauseMenuOption.Quit)
                    {
                        currPoption = PauseMenuOption.Settings;
                        NativeLog.Log("Quit to Settings");
                        uiquitBtn.Texture = ".\\resources\\textures\\mainmenubutton(unselected).dds";
                        uisettingsBtn.Texture = ".\\resources\\textures\\SettingsButton(Selected).dds";

                    }
                    else if (currPoption == PauseMenuOption.Settings)
                    {
                        currPoption = PauseMenuOption.Restart;
                        NativeLog.Log("Settings to Restart");
                        uisettingsBtn.Texture = ".\\resources\\textures\\SettingsButton(Unselected).dds";
                        uirestartBtn.Texture = ".\\resources\\textures\\RestartButton(Selected).dds";

                    }
                    else if (currPoption == PauseMenuOption.Restart)
                    {
                        currPoption = PauseMenuOption.Resume;
                        NativeLog.Log("Restart to Resume");
                        uirestartBtn.Texture = ".\\resources\\textures\\RestartButton(Unselected).dds";
                        uiresumeBtn.Texture = ".\\resources\\textures\\ResumeButton(selected).dds";
                    }

                }
                else if (Input.GetMappingKeyPressed(altSelectDown) || Input.GetMappingGamePadButtonPressed(altSelectDown, gamepad))
                {
                    if (currPoption == PauseMenuOption.Resume)
                    {
                        currPoption = PauseMenuOption.Restart;
                        NativeLog.Log("Resume to Restart");
                        uiresumeBtn.Texture = ".\\resources\\textures\\ResumeButton(Unselected).dds";
                        uirestartBtn.Texture = ".\\resources\\textures\\RestartButton(Selected).dds";

                    }
                    else if (currPoption == PauseMenuOption.Restart)
                    {
                        currPoption = PauseMenuOption.Settings;
                        NativeLog.Log("Restart to Settings");
                        uirestartBtn.Texture = ".\\resources\\textures\\RestartButton(Unselected).dds";
                        uisettingsBtn.Texture = ".\\resources\\textures\\SettingsButton(Selected).dds";

                    }
                    else if (currPoption == PauseMenuOption.Settings)
                    {
                        currPoption = PauseMenuOption.Quit;
                        NativeLog.Log("Settings to Quit");
                        uisettingsBtn.Texture = ".\\resources\\textures\\SettingsButton(Unselected).dds";
                        uiquitBtn.Texture = ".\\resources\\textures\\mainmenubutton(selected).dds";
                    }
                }

                if (Input.GetMappingGamePadButtonPressed(confirm_option, gamepad) || Input.GetMappingKeyPressed(confirm_option))
                {
                    if (currPoption == PauseMenuOption.Resume)
                    {
                        GameState.IsPaused = false;
                        GameState.SetEnginePauseState(false);
                        menuShown = false;
                        isPaused = false;
                        HidePauseUI();
                        ShowGameUI();
                        Audio.GameUnpauseAllSounds();
                    }

                    if (currPoption == PauseMenuOption.Settings)
                    {
                        GameData.fromPause = true;
                        OptionsMenu.bgmComp = bgmSound;
                        SceneManager.LoadScene(".\\resources\\scene\\V1_OPTIONS_V3.scene");
                    }

                    if (currPoption == PauseMenuOption.Restart)
                    {
                        GameState.SetEnginePauseState(false);
                        SceneManager.RemoveSceneFromCache(GameData.GameLvl1SceneFile);
                        SceneManager.LoadScene(GameData.GameLvl1SceneFile, true, true);
                        menuShown = false;
                        isPaused = false;
                    }

                    if (currPoption == PauseMenuOption.Quit)
                    {
                        //PauseUIHalf();
                        //currPoption = PauseMenuOption.None;
                        isOverlay = true;
                        overlayUI.Opacity = 1.0f;
                        yesUI.Opacity = 1.0f;
                        noUI.Opacity = 1.0f;

                        //GameState.QuitGame();
                    }
                }
            } 
            else if (isOverlay)
            {
                //PauseUIHalf();
                if (Input.GetMappingKeyPressed(selectLeftYes) || Input.GetMappingGamePadButtonPressed(selectLeftYes, gamepad))
                {
                    if (quitConfirm == ConfirmOverlay.No)
                    {
                        quitConfirm = ConfirmOverlay.Yes;

                        noUI.Texture = ".\\resources\\textures\\Nounselected.dds";
                        yesUI.Texture = ".\\resources\\textures\\Yesselected.dds";
                    }
                }
                else if (Input.GetMappingKeyPressed(selectRightNo) || Input.GetMappingGamePadButtonPressed(selectRightNo, gamepad))
                {
                    if (quitConfirm == ConfirmOverlay.Yes)
                    {
                        quitConfirm = ConfirmOverlay.No;

                        noUI.Texture = ".\\resources\\textures\\NoSelected.dds";
                        yesUI.Texture = ".\\resources\\textures\\YesUnselected.dds";
                    }
                }

                if (Input.GetMappingGamePadButtonPressed(confirm_option, gamepad) || Input.GetMappingKeyPressed(confirm_option))
                {
                    if (quitConfirm == ConfirmOverlay.No)
                    {
                        isOverlay = false;
                    }
                    if (quitConfirm == ConfirmOverlay.Yes)
                    {
                        isOverlay = false;
                        GameState.IsPaused = false;
                        GameState.SetEnginePauseState(false);
                        SceneManager.RemoveSceneFromCache(GameData.GameLvl1SceneFile);
                        SceneManager.RemoveSceneFromCache(".\\resources\\scene\\V1_MAIN_MENU.scene");
                        SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene", false, true);
                    }
                }
            }
        }
    }
}
