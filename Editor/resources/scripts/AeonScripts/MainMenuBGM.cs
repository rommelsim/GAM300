using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class MainMenuBGM : AeonBehaviour
    {
        private AudioComponent playBGM = null;

        private bool hasPlayedSound = false;
        private bool hasStopedSound = false;

        private bool hasSpaceTriggeredX1 = false;
        private bool hasSpaceTriggeredX2 = false;

        public override void OnCreate()
        {
            playBGM = entity.GetComponent<AudioComponent>();
            playBGM.PlaySound();
            playBGM.FadeIn(3.0f);
            //if (!hasPlayedSound) 
            //{
            //    if (playBGM == null)
            //    {
            //        NativeLog.Log("We Played MENU BGM!");
            //        playBGM.PlaySound();
            //        hasPlayedSound = true;
            //    }
            //}
        }

        //public override void Update()
        //{
        //    if (!hasStopedSound) 
        //    {
        //        if (Input.GetKeyPressed(Keys.KEY_SPACE) && !hasSpaceTriggeredX1)
        //        {
        //            if (playBGM != null)
        //            {
                        
        //                hasSpaceTriggeredX1 = true;
        //                playBGM.FadeOut(5.0f);
        //                //playBGM.StopSound();
        //                NativeLog.Log("We Stoped MENU BGM!");
        //                hasStopedSound = true;
        //            }
        //            if (Input.GetKeyPressed(Keys.KEY_V) && !hasSpaceTriggeredX2 && hasSpaceTriggeredX1) 
        //            {
                        
        //                playBGM.StopSound();
        //                hasStopedSound = true;
        //            }
        //        }
        //    }
        //}
    }
}
