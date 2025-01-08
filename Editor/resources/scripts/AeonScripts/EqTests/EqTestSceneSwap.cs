using AeonScriptCore;

namespace AeonScripts
{
    public class EqTestSceneSwap : AeonBehaviour
    {
        public int NextSceneIdx = 0;

        // ---- Camera Stuffs ----
        public bool RunCamLogic = false;
        public string Cam1_Name = "";
        public string Cam2_Name = "";
        private Entity cam1 = null;
        private Entity cam2 = null;
        private int activeCam = 1;

        // ---- UI Data saved for cached scene ----
        private UI uiComp = null;
        private int counter = 0;

        // Cache Game Test datas ----
        public bool forGameTest = false;
        public static readonly string GameTestSceneFile = "eq_cacheGameTest_1.scene";

        public override void OnCreate()
        {
            base.OnCreate();

            cam1 = Entity.FindEntityByName(Cam1_Name);
            cam2 = Entity.FindEntityByName(Cam2_Name);

            uiComp = entity.GetComponent<UI>();

            if (uiComp != null)
            {
                uiComp.Text = counter.ToString();
            }
        }

        public override void Update()
        {
            if (Input.GetKeyPressed(Keys.KEY_E))
            {
                if (forGameTest)
                    SceneManager.LoadScene("eq_cacheGameTest_1.scene", true, true);
                else
                    SceneManager.LoadScene($"eq_scene_{NextSceneIdx}.scene", false, true);
            }
            else if (forGameTest)
            {
                if (Input.GetKeyPressed(Keys.KEY_T))
                {
                    SceneManager.RemoveSceneFromCache(GameTestSceneFile);
                }
                if (Input.GetKeyPressed(Keys.KEY_ESCAPE))
                {
                    GameState.IsPaused = !GameState.IsPaused;
                    NativeLog.Log("Paused state: {0}", GameState.IsPaused);
                }
            }

            if (RunCamLogic && Input.GetKeyPressed(Keys.KEY_V))
            {
                if (activeCam == 1)
                {
                    NativeLog.Log("Cam {0}", cam2.Id);
                    Graphics.RequestSwitchCamera(cam2.Id);
                    activeCam = 2;
                }
                else if (activeCam == 2)
                {
                    NativeLog.Log("Cam {0}", cam1.Id);
                    Graphics.RequestSwitchCamera(cam1.Id);
                    activeCam = 1;
                }
            }

            if (uiComp != null)
            {
                if (Input.GetKeyPressed(Keys.KEY_I))
                {
                    uiComp.Text = (++counter).ToString();
                }
            }
        }

        public override void OnCachedSceneActive()
        {
            NativeLog.Log("OnCachedSceneActive");
        }

        public override void OnCachedSceneInactive()
        {
            NativeLog.Log("OnCachedSceneInactive");
        }
    }
}
