using AeonScriptCore;

namespace AeonScripts.EqTests
{
    public class GameTest : AeonBehaviour
    {
        public override void Update()
        {
            if (Input.GetKeyPressed(Keys.KEY_ESCAPE))
            {
                GameState.IsPaused = !GameState.IsPaused;
                NativeLog.Log("Paused state: {0}", GameState.IsPaused);
            }
            else if (Input.GetKeyPressed(Keys.KEY_E) /*&& GameState.IsPaused*/)
            {
                SceneManager.LoadScene("eq_cacheGameTest_0.scene", false, true);
            }
            if (Input.GetKeyPressed(Keys.KEY_T))
            {
                SceneManager.RemoveSceneFromCache("eq_cacheGameTest_1.scene");
            }
            if (Input.GetKeyPressed(Keys.KEY_R)) {
                SceneManager.RemoveSceneFromCache(EqTestSceneSwap.GameTestSceneFile);
                SceneManager.LoadScene("EQ_cacheGameTest_1.scene", true, true);
            }
        }
    }
}
