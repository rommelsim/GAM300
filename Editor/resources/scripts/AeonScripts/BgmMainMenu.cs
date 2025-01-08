using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AeonScriptCore;

namespace AeonScripts
{
    internal class BgmMainMenu : AeonBehaviour
    {
        public static AudioComponent menuBGM { get; private set; }

        BgmMainMenu() : base() {
            menuBGM = null;
        }

        public override void OnCreate()
        {
            menuBGM = entity.GetComponent<AudioComponent>();
            if (menuBGM != null )
            {
                menuBGM.PlaySound();
                menuBGM.FadeIn(3.0f);
            }
        }
    }
}
