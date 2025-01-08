using AeonScriptCore;

namespace AeonScripts
{
    public class LoadScreenProgress : AeonBehaviour
    {
        UI uiComp = null;

        public override void OnCreate()
        {
            uiComp = entity.GetComponent<UI>();
            if (uiComp != null )
            {
                uiComp.Text = "0%";
                //NativeLog.LogWarn("Load UI:{0}", uiComp.Text);
            }
            NativeLog.LogWarn("LoadScreenProgress' OnCreate is running");
        }

        public override void Update()
        {
            if (uiComp != null)
            {
                uiComp.Text = $"{SceneManager.LoadingProgress}%";
                //NativeLog.LogWarn("Load UI:{0}", uiComp.Text);
            }
        }
    }
}
