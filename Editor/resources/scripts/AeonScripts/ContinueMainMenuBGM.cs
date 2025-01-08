using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    internal class ContinueMainMenuBGM : AeonBehaviour
    {
        private bool hasResume = false;

        public override void Update()
        {
            if (BgmMainMenu.menuBGM == null || hasResume) return;

            BgmMainMenu.menuBGM.ResumeSound();
            hasResume = true;
        }
    }
}
