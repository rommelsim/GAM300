using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public enum Options
    {
        Voice,
        BGM,
        SFX,
        Gamma,
        Master
    }
    public class OptionsMenu : AeonBehaviour
    {
        public Options currOption { get; set; }

        public static AudioComponent bgmComp = null;

        // entities of texture UI
        public Entity bgmSlider = null;
        public Entity bgmButton = null;
        public Entity sfxSlider = null;
        public Entity sfxButton = null;
        public Entity voiceSlider = null;
        public Entity voiceButton = null;
        public Entity gammaSlider = null;
        public Entity gammaButton = null;
        public Entity masterSlider = null;
        public Entity masterButton = null;

        // UI of textures
        public UI bgmSliderUI = null;
        public UI bgmButtonUI = null;
        public UI sfxSliderUI = null;
        public UI sfxButtonUI = null;
        public UI voiceSliderUI = null;
        public UI voiceButtonUI = null;
        public UI gammaSliderUI = null;
        public UI gammaButtonUI = null;
        public UI masterSliderUI = null;
        public UI masterButtonUI = null;

        // entities of word UI
        public Entity bgmPercent = null;
        public Entity sfxPercent = null;
        public Entity voicePercent = null;
        public Entity gammaPercent = null;
        public Entity masterPercent = null;

        // UI of words
        public UI bgmPercentUI = null;
        public UI sfxPercentUI = null;
        public UI voicePercentUI = null;
        public UI gammaPercentUI = null;
        public UI masterPrrcentUI = null;

        // button positions
        public Transform bgmTrans = null;
        public Transform sfxTrans = null;
        public Transform voiceTrans = null;
        public Transform gammaTrans = null;
        public Transform masterTrans = null;

        // sounds
        public Entity bgm = null;
        public Entity sfx = null;
        public Entity voice = null;

        public AudioComponent bgmAudio = null;
        public AudioComponent sfxAudio = null;
        public AudioComponent voiceAudio = null;

        private DateTime lastInputTime = DateTime.MinValue;
        private float inputCooldown = 0.3f;

        public static string selectLeft = "select_Left";
        public static string selectRight = "select_Right";
        public static string altSelectLeft = "alt_Select_Left";
        public static string altSelectRight = "alt_Select_Right";

        public static string selectUp = "select_Up";
        public static string selectDown = "select_Down";
        public static string altSelectUp = "alt_Select_Up";
        public static string altSelectDown = "alt_Select_Down";

        public static string quitSelectB = "quit_SelectB";

        public static float volumeVoice; // 0.0f to 1.0f
        public static float volumeSFX;
        public static float volumeBGM;
        public static float volumeMaster;

        public static float gamma = 2.0f;

        private static readonly float max = 145f;
        private static readonly float min = -505f;
        private static readonly float length = max - min;

        public float sliderDist = (max - min) / 10f;
        public float gammaDist = (max - min) / 10f;

        public Audio audio;

        public override void OnCreate()
        {
            // texture UI entities
            bgmSlider = Entity.FindEntityByName("BGMSlider");
            bgmButton = Entity.FindEntityByName("BGMButton");
            sfxSlider = Entity.FindEntityByName("SFXSlider");
            sfxButton = Entity.FindEntityByName("SFXButton");
            voiceSlider = Entity.FindEntityByName("VoiceSlider");
            voiceButton = Entity.FindEntityByName("VoiceButton");
            gammaSlider = Entity.FindEntityByName("GammaSlider");
            gammaButton = Entity.FindEntityByName("GammaButton");
            masterSlider = Entity.FindEntityByName("MasterSlider");
            masterButton = Entity.FindEntityByName("MasterButton");

            if (bgmSlider != null) { bgmSliderUI = bgmSlider.GetComponent<UI>(); }
            if (sfxSlider != null) { sfxSliderUI = sfxSlider.GetComponent<UI>(); }
            if (voiceSlider != null) { voiceSliderUI = voiceSlider.GetComponent<UI>(); }
            if (gammaSlider != null) { gammaSliderUI = gammaSlider.GetComponent<UI>(); }
            if (masterSlider != null) { masterSliderUI = masterSlider.GetComponent<UI>(); }

            if (voiceButton != null)
            {
                voiceButtonUI = voiceButton.GetComponent<UI>();
                voiceTrans = voiceButton.GetComponent<Transform>();
            }
            if (bgmButton != null)
            { 
                bgmButtonUI = bgmButton.GetComponent<UI>(); 
                bgmTrans = bgmButton.GetComponent<Transform>();
            }
            if (sfxButton != null)
            {
                sfxButtonUI = sfxButton.GetComponent<UI>();
                sfxTrans = sfxButton.GetComponent<Transform>();
            }
            if (gammaButton != null)
            {
                gammaButtonUI = gammaButton.GetComponent<UI>();
                gammaTrans = gammaButton.GetComponent<Transform>();
            }
            if (masterButton != null)
            {
                masterButtonUI = masterButton.GetComponent<UI>();
                masterTrans = masterButton.GetComponent<Transform>();
            }

            // word UI entities
            bgmPercent = Entity.FindEntityByName("BGMPercent");
            sfxPercent = Entity.FindEntityByName("SFXPercent");
            voicePercent = Entity.FindEntityByName("VoicePercent");
            gammaPercent = Entity.FindEntityByName("GammaPercent");
            masterPercent = Entity.FindEntityByName("MasterPercent");

            if (bgmPercent != null) { bgmPercentUI = bgmPercent.GetComponent<UI>(); }
            if (voicePercent != null) { voicePercentUI = voicePercent.GetComponent<UI>(); }
            if (sfxPercent != null) { sfxPercentUI = sfxPercent.GetComponent<UI>(); }
            if (gammaPercent != null) {  gammaPercentUI = gammaPercent.GetComponent<UI>(); }
            if (masterPercent != null) { masterPrrcentUI = masterPercent.GetComponent<UI>(); }

            // sound entities
            bgm = Entity.FindEntityByName("BGMSound");
            sfx = Entity.FindEntityByName("SFXSound");
            voice = Entity.FindEntityByName("VoiceSound");

            if (bgm != null) bgmAudio = bgm.GetComponent<AudioComponent>();
            if (sfx != null) sfxAudio = sfx.GetComponent<AudioComponent>();
            if (voice != null) voiceAudio = voice.GetComponent<AudioComponent>();

            volumeVoice = Audio.GetChannelTypeVolume(Audio.ChannelType.Voice);
            volumeBGM = Audio.GetChannelTypeVolume(Audio.ChannelType.BGM);
            volumeSFX = Audio.GetChannelTypeVolume(Audio.ChannelType.SFX);
            volumeMaster = Audio.MasterVolume;

            if (!GameData.fromPause)
            {
                if (BgmMainMenu.menuBGM != null) BgmMainMenu.menuBGM.ResumeSound();
            }

            currOption = Options.Master;

            Input.RegisterMapping(selectLeft, Input.KBMInputType.Keyboard, Keys.KEY_J, Input.JoystickInput.Axes_LStick_Axis_X);
            Input.RegisterMapping(selectRight, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Axes_LStick_Axis_X);
            Input.RegisterMapping(altSelectLeft, Input.KBMInputType.Keyboard, Keys.KEY_A, Input.JoystickInput.Button_Dpad_Left);
            Input.RegisterMapping(altSelectRight, Input.KBMInputType.Keyboard, Keys.KEY_D, Input.JoystickInput.Button_Dpad_Right);

            Input.RegisterMapping(selectUp, Input.KBMInputType.Keyboard, Keys.KEY_N, Input.JoystickInput.Axes_LStick_Axis_Y);
            Input.RegisterMapping(selectDown, Input.KBMInputType.Keyboard, Keys.KEY_M, Input.JoystickInput.Axes_LStick_Axis_Y);
            Input.RegisterMapping(altSelectUp, Input.KBMInputType.Keyboard, Keys.KEY_W, Input.JoystickInput.Button_Dpad_Up);
            Input.RegisterMapping(altSelectDown, Input.KBMInputType.Keyboard, Keys.KEY_S, Input.JoystickInput.Button_Dpad_Down);

            Input.RegisterMapping(quitSelectB, Input.KBMInputType.Keyboard, Keys.KEY_ESCAPE, Input.JoystickInput.Button_B);

            // calculations
            GameData.voiceTransform.X = min + length * volumeVoice;
            GameData.voiceTransform.Y = voiceTrans.Position.Y;
            voiceTrans.Position = GameData.voiceTransform;

            GameData.sfxTransform.X = min + length * volumeSFX;
            GameData.sfxTransform.Y = sfxTrans.Position.Y;
            sfxTrans.Position = GameData.sfxTransform;

            GameData.bgmTransform.X = min + length * volumeBGM;
            GameData.bgmTransform.Y = bgmTrans.Position.Y;
            bgmTrans.Position = GameData.bgmTransform;

            // slider goes from 1.0f to 2.0f
            GameData.gammaTransform.X = min + length * ((Graphics.Gamma - 1));
            GameData.gammaTransform.Y = gammaTrans.Position.Y;
            gammaTrans.Position = GameData.gammaTransform;

            GameData.masterTransform.X = min + length * volumeMaster;
            GameData.masterTransform.Y = masterTrans.Position.Y;
            masterTrans.Position = GameData.masterTransform;

            GameState.IsPaused = false;
            GameState.SetEnginePauseState(false);
        }

        public override void Update()
        {
            if (bgmComp != null)
            {
                bgmComp.ResumeSound();
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

            if ((DateTime.Now - lastInputTime).TotalSeconds >= inputCooldown)
            {
                if (GameData.isLevel1)
                {
                    RunOptions(PauseMenu.gamepad);       
                }
                else if (GameData.isLevel2)
                {
                    RunOptions(PauseMenuLevel2.gamepad);
                }
                else
                {
                    RunOptions(MainMenuSelect.gamepad);
                } 
                //float leftRightAxisOne = Input.GetMappingGamePadAxisValue(selectLeft, gamePadOne);
                //float leftRightAxisTwo = Input.GetMappingGamePadAxisValue(selectLeft, gamePadTwo);

                //float upDownAxisOne = Input.GetMappingGamePadAxisValue(selectUp, gamePadOne);
                //float upDownAxisTwo = Input.GetMappingGamePadAxisValue(selectUp, gamePadTwo);

                //bool moveUpOne = false;
                //bool moveDownOne = false;

                //bool moveUpTwo = false;
                //bool moveDownTwo = false;

                //bool moveLeftOne = false;
                //bool moveRightOne = false;

                //bool moveLeftTwo = false;
                //bool moveRightTwo = false;

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
                //if ((isGamepadConnected && leftRightAxisOne < 0) || (isGamepadConnected && leftRightAxisTwo < 0))
                //{
                //    moveLeftOne = true;
                //    moveLeftTwo = true;
                //}
                //else if ((isGamepadConnected && leftRightAxisOne > 0) || (isGamepadConnected && leftRightAxisTwo > 0))
                //{
                //    moveRightOne = true;
                //    moveRightTwo = true;
                //}
            }
        }
        public void RunOptions(int gamepad)
        {
            if (Input.GetMappingKeyPressed(altSelectDown) || Input.GetMappingGamePadButtonPressed(altSelectDown, gamepad))
            {
                if (currOption == Options.Master)
                {
                    currOption = Options.BGM;
                    masterSliderUI.Texture = ".\\resources\\textures\\Unslected.dds";
                    masterButtonUI.Texture = ".\\resources\\textures\\Unselected_Indicator.dds";

                    bgmSliderUI.Texture = ".\\resources\\textures\\Selected.dds";
                    bgmButtonUI.Texture = ".\\resources\\textures\\Selected_Indicator.dds";
                }
                else if (currOption == Options.SFX)
                {
                    currOption = Options.Voice;

                    sfxSliderUI.Texture = ".\\resources\\textures\\Unslected.dds";
                    sfxButtonUI.Texture = ".\\resources\\textures\\Unselected_Indicator.dds";

                    voiceSliderUI.Texture = ".\\resources\\textures\\Selected.dds";
                    voiceButtonUI.Texture = ".\\resources\\textures\\Selected_Indicator.dds";
                }
                else if (currOption == Options.BGM)
                {
                    currOption = Options.SFX;

                    bgmSliderUI.Texture = ".\\resources\\textures\\Unslected.dds";
                    bgmButtonUI.Texture = ".\\resources\\textures\\Unselected_Indicator.dds";

                    sfxSliderUI.Texture = ".\\resources\\textures\\Selected.dds";
                    sfxButtonUI.Texture = ".\\resources\\textures\\Selected_Indicator.dds";
                }
                else if (currOption == Options.Voice)
                {
                    currOption = Options.Gamma;

                    voiceSliderUI.Texture = ".\\resources\\textures\\Unslected.dds";
                    voiceButtonUI.Texture = ".\\resources\\textures\\Unselected_Indicator.dds";

                    gammaSliderUI.Texture = ".\\resources\\textures\\Selected.dds";
                    gammaButtonUI.Texture = ".\\resources\\textures\\Selected_Indicator.dds";
                }
            }

            else if (Input.GetMappingKeyPressed(altSelectUp) || Input.GetMappingGamePadButtonPressed(altSelectUp, gamepad))
            {
                if (currOption == Options.Gamma)
                {
                    currOption = Options.Voice;

                    gammaSliderUI.Texture = ".\\resources\\textures\\Unslected.dds";
                    gammaButtonUI.Texture = ".\\resources\\textures\\Unselected_Indicator.dds";

                    voiceSliderUI.Texture = ".\\resources\\textures\\Selected.dds";
                    voiceButtonUI.Texture = ".\\resources\\textures\\Selected_Indicator.dds";
                }
                else if (currOption == Options.Voice)
                {
                    currOption = Options.SFX;

                    voiceSliderUI.Texture = ".\\resources\\textures\\Unslected.dds";
                    voiceButtonUI.Texture = ".\\resources\\textures\\Unselected_Indicator.dds";

                    sfxSliderUI.Texture = ".\\resources\\textures\\Selected.dds";
                    sfxButtonUI.Texture = ".\\resources\\textures\\Selected_Indicator.dds";
                }
                else if (currOption == Options.SFX)
                {
                    currOption = Options.BGM;

                    sfxSliderUI.Texture = ".\\resources\\textures\\Unslected.dds";
                    sfxButtonUI.Texture = ".\\resources\\textures\\Unselected_Indicator.dds";

                    bgmSliderUI.Texture = ".\\resources\\textures\\Selected.dds";
                    bgmButtonUI.Texture = ".\\resources\\textures\\Selected_Indicator.dds";
                }
                else if (currOption == Options.BGM)
                {
                    currOption = Options.Master;

                    masterSliderUI.Texture = ".\\resources\\textures\\Selected.dds";
                    masterButtonUI.Texture = ".\\resources\\textures\\Selected_Indicator.dds";

                    bgmSliderUI.Texture =".\\resources\\textures\\Unslected.dds";
                    bgmButtonUI.Texture = ".\\resources\\textures\\Unselected_Indicator.dds";
                }
            }
            else if (Input.GetMappingKeyPressed(altSelectLeft) || Input.GetMappingGamePadButtonPressed(altSelectLeft, gamepad))
            {
                if (currOption == Options.Voice)
                {
                    if (voiceTrans.Position.X > min)
                    {
                        GameData.voiceTransform = voiceTrans.Position;
                        GameData.voiceTransform.X -= sliderDist;
                        voiceTrans.Position = GameData.voiceTransform;

                        volumeVoice = (GameData.voiceTransform.X - min) / length;
                        Audio.SetChannelTypeVolume(Audio.ChannelType.Voice, volumeVoice);

                        voiceAudio.SoundFilepath = ".\\resources\\audios\\Anouncer_3.wav";
                        voiceAudio.ChannelType = Audio.ChannelType.Voice;
                        voiceAudio.Volume = 1.0f;
                        voiceAudio.PlaySound();
                    }
                }
                else if (currOption == Options.SFX)
                {
                    if (sfxTrans.Position.X > min)
                    {
                        GameData.sfxTransform = sfxTrans.Position;
                        GameData.sfxTransform.X -= sliderDist;
                        sfxTrans.Position = GameData.sfxTransform;

                        volumeSFX = (GameData.sfxTransform.X - min) / length;
                        Audio.SetChannelTypeVolume(Audio.ChannelType.SFX, volumeSFX);

                        sfxAudio.SoundFilepath = ".\\resources\\audios\\Jump1.wav";
                        sfxAudio.ChannelType = Audio.ChannelType.SFX;
                        sfxAudio.Volume = 1.0f;
                        sfxAudio.PlaySound();
                    }
                }
                else if (currOption == Options.BGM)
                {
                    if (bgmTrans.Position.X > min)
                    {
                        GameData.bgmTransform = bgmTrans.Position;
                        GameData.bgmTransform.X -= sliderDist;
                        bgmTrans.Position = GameData.bgmTransform;

                        volumeBGM = (GameData.bgmTransform.X - min) / length;
                        Audio.SetChannelTypeVolume(Audio.ChannelType.BGM, volumeBGM);
                        NativeLog.Log("bgm: {0}", volumeBGM);
                    }
                }
                else if (currOption == Options.Master)
                {
                    if (masterTrans.Position.X > min)
                    {
                        GameData.masterTransform = masterTrans.Position;
                        GameData.masterTransform.X -= sliderDist;
                        masterTrans.Position = GameData.masterTransform;

                        volumeMaster = (GameData.masterTransform.X - min) / length;
                        Audio.MasterVolume = volumeMaster;
                    }
                }
                else if (currOption == Options.Gamma)
                {
                    if (gammaTrans.Position.X > min)
                    {
                        GameData.gammaTransform = gammaTrans.Position;
                        GameData.gammaTransform.X -= gammaDist;
                        gammaTrans.Position = GameData.gammaTransform;

                        gamma = (GameData.gammaTransform.X - min) / length + 1;
                        Graphics.Gamma = gamma;//(float)Math.Round(gamma * 10f, 1);
                        NativeLog.Log($"Gamma: {Graphics.Gamma}");

                    }
                }
            }
            else if (Input.GetMappingKeyPressed(altSelectRight) || Input.GetMappingGamePadButtonPressed(altSelectRight, gamepad))
            {
                if (currOption == Options.Voice)
                {
                    if (voiceTrans.Position.X < max)
                    {
                        GameData.voiceTransform = voiceTrans.Position;
                        GameData.voiceTransform.X += sliderDist;
                        voiceTrans.Position = GameData.voiceTransform;

                        volumeVoice = (GameData.voiceTransform.X - min) / length;
                        Audio.SetChannelTypeVolume(Audio.ChannelType.Voice, volumeVoice);

                        voiceAudio.SoundFilepath = ".\\resources\\audios\\Anouncer_3.wav";
                        voiceAudio.ChannelType = Audio.ChannelType.Voice;
                        voiceAudio.Volume = 1.0f;
                        voiceAudio.PlaySound();
                    }
                }
                else if (currOption == Options.SFX)
                {
                    if (sfxTrans.Position.X < max)
                    {
                        GameData.sfxTransform = sfxTrans.Position;
                        GameData.sfxTransform.X += sliderDist;
                        sfxTrans.Position = GameData.sfxTransform;

                        volumeSFX = (GameData.sfxTransform.X - min) / length;
                        Audio.SetChannelTypeVolume(Audio.ChannelType.SFX, volumeSFX);

                        sfxAudio.SoundFilepath = ".\\resources\\audios\\Jump1.wav";
                        sfxAudio.ChannelType = Audio.ChannelType.SFX;
                        sfxAudio.Volume = 1.0f;
                        sfxAudio.PlaySound();
                    }
                }
                else if (currOption == Options.BGM)
                {
                    if (bgmTrans.Position.X < max)
                    {
                        GameData.bgmTransform = bgmTrans.Position;
                        GameData.bgmTransform.X += sliderDist;
                        bgmTrans.Position = GameData.bgmTransform;

                        volumeBGM = (GameData.bgmTransform.X - min) / length;
                        Audio.SetChannelTypeVolume(Audio.ChannelType.BGM, volumeBGM);
                        // NativeLog.Log("bgm: {0}", volumeBGM);
                    }
                }
                else if (currOption == Options.Master)
                {
                    if (masterTrans.Position.X < max)
                    {
                        GameData.masterTransform = masterTrans.Position;
                        GameData.masterTransform.X += sliderDist;
                        masterTrans.Position = GameData.masterTransform;

                        volumeMaster = (GameData.masterTransform.X - min) / length;
                        Audio.MasterVolume = volumeMaster;
                    }
                }
                else if (currOption == Options.Gamma)
                {
                    if (gammaTrans.Position.X < max)
                    {
                        GameData.gammaTransform = gammaTrans.Position;
                        GameData.gammaTransform.X += gammaDist;
                        gammaTrans.Position = GameData.gammaTransform;

                        gamma = (GameData.gammaTransform.X - min) / length + 1;
                        Graphics.Gamma = gamma;//(float)Math.Round(gamma * 1f, 1);
                        NativeLog.Log($"Gamma: {Graphics.Gamma}");
                    }
                }
            }
            voicePercentUI.Text = (volumeVoice * 10).ToString();
            sfxPercentUI.Text = (volumeSFX * 10).ToString();
            bgmPercentUI.Text = (volumeBGM * 10).ToString();
            gammaPercentUI.Text = Graphics.Gamma.ToString();
            masterPrrcentUI.Text = (volumeMaster * 10).ToString();
            //NativeLog.Log(GameData.fromPause.ToString());
            if (Input.GetMappingGamePadButtonPressed(quitSelectB, gamepad) || Input.GetMappingKeyPressed(quitSelectB))
            {
                // TODO: add a check to see whether options menu was accessed through game or main menu
                if (!GameData.fromPause)
                {
                    SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene");
                }
                else
                {
                    if (GameData.isLevel1)
                    {
                        GameData.fromPause = false;
                        GameState.IsPaused = true;
                        GameState.SetEnginePauseState(true);
                        SceneManager.LoadScene(GameData.GameLvl1SceneFile, false, true);
                    }
                    else if (GameData.isLevel2)
                    {
                        GameData.fromPause = false;
                        GameState.IsPaused = true;
                        GameState.SetEnginePauseState(true);
                        SceneManager.LoadScene(GameData.GameLvl2SceneFile, false, true);
                    }
                    //NativeLog.Log(GameData.fromPause.ToString());
                }
            }
        }
    }
}
