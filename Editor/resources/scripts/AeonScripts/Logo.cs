using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class Logo : AeonBehaviour
    {
        private Entity dotEnt = null;
        private UI dotUI = null;

        private UI logoUI = null;

        private float timer = 0;
        private bool secondLogo = false;


        private int tracker = 0;

        public static string skipButton = "skipLogo";

        // for lerping opacity
        private float dt = 0.05f;
        public float fadeTime = 2f;
        public float logoTime = 2f;
        private bool fadeIn = true;
        private bool fadeOut = false;

        private float delayTimer = 0;

        public override void OnCreate()
        {
            dotEnt = Entity.FindEntityByName("Dot");
            if (dotEnt != null) { dotUI = dotEnt.GetComponent<UI>(); }
            if (entity != null) { logoUI = entity.GetComponent<UI>(); }

            Input.RegisterMapping(skipButton, Input.KBMInputType.Keyboard, Keys.KEY_ENTER, Input.JoystickInput.Button_X);
        }

        public override void Update()
        {
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

            if (fadeIn)
            {
                if (tracker == 1)
                {
                    delayTimer += Time.DeltaTime;
                }
                if (tracker == 0 || (tracker == 1 && delayTimer >= 1f))
                {
                    FadeIn(gamePadOne, gamePadTwo);
                }

                if (timer >= fadeTime)
                {
                    fadeIn = false;
                    timer = 0;
                }
            }
            else if (!fadeOut)
            {
                timer += dt;

                if (timer >= logoTime)
                {
                    fadeIn = false;
                    timer = 0;
                    fadeOut = true;
                }
            }

            if (fadeOut)
            {
                FadeOut(gamePadOne);

                if (timer >= fadeTime)
                {
                    fadeIn = true;
                    timer = 0;
                    fadeOut = false;
                    logoUI.Texture = ".\\resources\\textures\\GameName.dds";
                    tracker += 1;
                }

                if (tracker == 2)
                {
                    SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene", false, true);
                }
            }

            //if (Input.GetMappingGamePadButtonPressed(skipButton, gamePadOne) || Input.GetMappingKeyPressed(skipButton)) 
            //{
            //    SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene");
            //}
        }

        public void FadeIn(int gamePadOne, int gamePadTwo)
        {
            dotUI.Opacity = Mathf.Lerp(1f, 0, timer / fadeTime);
            timer += dt;

            bool buttonPressed = false;

            if ((Input.GetMappingGamePadButtonPressed(skipButton, gamePadOne) || Input.GetMappingGamePadButtonPressed(skipButton, gamePadTwo) || Input.GetMappingKeyPressed(skipButton))
                && !buttonPressed)
            {
                dt += 0.1f;
                buttonPressed = true;
                NativeLog.Log("fade in add");
            }
        }

        public void FadeOut(int gamePad)
        {
            dotUI.Opacity = Mathf.Lerp(0, 1f, timer / fadeTime);
            timer += dt;

            bool buttonPressed = false;

            //if ((Input.GetMappingGamePadButtonPressed(skipButton, gamePad) || Input.GetMappingKeyPressed(skipButton)) && !buttonPressed)
            //{
            //    dt += 0.2f;
            //    buttonPressed = true;
            //    NativeLog.Log("fade out add");
            //}
        }

    }
    public class Mathf
    {
        public static float Lerp(float start, float end, float t)
        {
            return (1 - t) * start + t * end;
        }
    }
}
