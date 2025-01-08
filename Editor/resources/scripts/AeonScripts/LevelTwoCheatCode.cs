using AeonScriptCore;

namespace AeonScripts
{
    public class LevelTwoCheatCode : AeonBehaviour
    {
        private bool hasPressed = false;

        public override void Update()
        {
            if (!hasPressed && Input.GetKeyPressed(Keys.KEY_P)) {
                SceneManager.LoadScene("V1_MAIN_MENU.scene");
                hasPressed = true;
            }
        }
    }
}
