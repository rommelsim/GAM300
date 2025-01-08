using AeonScriptCore;

namespace AeonScripts
{
    public class TextureChange : AeonBehaviour
    {
        private UI uiComp = null;
        private bool switchFlag = false;

        public override void OnCreate()
        {
            uiComp = entity.GetComponent<UI>();
        }

        public override void Update()
        {
            if (uiComp == null) {
                uiComp = entity.GetComponent<UI>();
            }

            if (uiComp != null)
            {
                if (Input.GetKeyPressed(Input.KEY_F))
                {
                    if (switchFlag)
                    {
                        uiComp.Texture = "./resources/textures/container.dds";
                    }
                    else
                    {
                        uiComp.Texture = "./resources/textures/Cat_2.dds";
                    }
                    switchFlag = !switchFlag;
                }
            }
        }
    }
}
