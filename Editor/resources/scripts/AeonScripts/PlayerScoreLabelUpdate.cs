using AeonScriptCore;

namespace AeonScripts
{
    public class PlayerScoreLabelUpdate : AeonBehaviour
    {
        private UI uiComp = null;
        private bool isPlayerOne = false;

        public override void OnCreate()
        {
            isPlayerOne = entity.Name.StartsWith("Player1");
            uiComp = entity.GetComponent<UI>();
        }

        public override void Update()
        {
            if (uiComp == null)
                uiComp = entity.GetComponent<UI>();

            if (uiComp != null)
            {
                if (isPlayerOne)
                {
                    if (GameData.P1_Score < 0)
                        GameData.P1_Score = 0;
                    uiComp.Text = GameData.P1_Score.ToString();
                }
                else
                {
                    if (GameData.P2_Score < 0)
                        GameData.P2_Score = 0;
                    uiComp.Text = GameData.P2_Score.ToString();
                }
            }
        }
    }
}
