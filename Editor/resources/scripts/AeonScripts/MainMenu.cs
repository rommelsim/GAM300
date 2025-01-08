using AeonScriptCore;
using System.Threading;
using System.Timers;

namespace AeonScripts
{
    //internal class Countdown
    //{
    //}
    public class MainMenu : AeonBehaviour
    {
        private UI uiComponent = null;
        private bool keypress = false;

        public override void OnCreate()
        {
            uiComponent = entity.GetComponent<UI>();
            // playSfx = entity.GetComponent<AudioComponent>();

        }
        public override void Update()
        {

			if (uiComponent == null)
			{
				uiComponent = entity.GetComponent<UI>();
			}
            if (uiComponent == null) return;
            if (Input.GetKeyPressed(Keys.KEY_SPACE))
            {
                if (keypress)
                {
                    SceneManager.LoadScene(GameData.GameLvl1SceneFile, true);
                }
                else
                {
                    uiComponent.Texture = ".\\resources\\textures\\Objectives1.dds";
                    keypress = true;
                }
            }

            if (Input.GetKeyPressed(Keys.KEY_V))
            {
                SceneManager.LoadScene(".\\resources\\scene\\Game_v2_3.scene");
            }

            //NativeLog.Log("Seconds" + seconds.ToString());
        }
    }
}
