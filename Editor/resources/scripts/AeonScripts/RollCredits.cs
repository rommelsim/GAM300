using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class RollCredits : AeonBehaviour
    {
        private Transform scrollTrans = null;

        // 2120
        private Entity creditsBG = null;
        private Transform creditsTrans = null;

        private Entity creditsTitle = null;
        private Transform titleTrans = null;

        private Vector3 endSpot;

        private bool isEnd = false;

        private float timer = 0;
        public float delayTimer = 2f;
        public float endTime = 0;

        public static string creditsButton = "to_credits";

        public override void OnCreate()
        {
            if (entity != null) { scrollTrans = entity.GetComponent<Transform>(); }

            creditsBG = Entity.FindEntityByName("CreditsBG");
            if (creditsBG != null ) { creditsTrans = creditsBG.GetComponent<Transform>(); }

            creditsTitle = Entity.FindEntityByName("Credits");
            if (creditsTitle != null ) {  titleTrans = creditsTitle.GetComponent<Transform>(); }

            Input.RegisterMapping(creditsButton, Input.KBMInputType.Keyboard, Keys.KEY_ENTER, Input.JoystickInput.Button_X);
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

            timer += Time.DeltaTime;

            if (timer >= delayTimer )
            {
                //Vector3 scroll = Vector3.Lerp(scrollTrans.Position, new Vector3 (scrollTrans.Position.X, -3660, scrollTrans.Position.Z), Time.DeltaTime * 0.1f);
                endSpot = new Vector3(scrollTrans.Position.X, -2450, scrollTrans.Position.Z);
                Vector3 scroll = new Vector3 (0, -200f * Time.DeltaTime, 0);

                if (scrollTrans.Position.Y > -2450)
                {
                    scrollTrans.Position += scroll;

                    if (scrollTrans.Position.Y <= -2450)
                    {
                        scrollTrans.Position = endSpot;
                        isEnd = true;
                    }
                }
                if (!isEnd)
                {
                    creditsTrans.Position += scroll;
                    titleTrans.Position += scroll;
                }

                if (isEnd)
                {
                    endTime += Time.DeltaTime;
                    if (endTime >= 1f)
                    {
                        SceneManager.RemoveSceneFromCache("V1_MAIN_MENU.scene");
                        SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene", false, true);
                    }

                }

                if (Input.GetMappingKeyPressed(creditsButton) || Input.GetMappingGamePadButtonPressed(creditsButton, gamePadOne) || Input.GetMappingGamePadButtonPressed(creditsButton, gamePadTwo))
                {
                    SceneManager.RemoveSceneFromCache("V1_MAIN_MENU.scene");
                    SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene", false, true);
                }
            }
        }
    }
}
