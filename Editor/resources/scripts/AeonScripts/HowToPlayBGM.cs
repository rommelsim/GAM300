using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class HowToPlayBGM : AeonBehaviour
    {
        private AudioComponent HowToPlay = null;
        //private bool hasPlayedSound = false;
        //private bool hasStopedSound = false;
        //private bool hasSpaceTriggeredX1 = false;

        public override void OnCreate()
        {
            HowToPlay = entity.GetComponent<AudioComponent>();
            HowToPlay.PlaySound();
            HowToPlay.FadeIn(3.0f);
        }
        //public override void Update()
        //{
        //    if (!hasPlayedSound)
        //    {
        //        if (Input.GetKeyPressed(Keys.KEY_SPACE) && !hasSpaceTriggeredX1)
        //        {
        //            hasSpaceTriggeredX1 = true;
        //            HowToPlay.PlaySound();
        //            HowToPlay.FadeIn(3.0f);
        //            NativeLog.Log("We Played How To Play BGM!");
        //            hasPlayedSound = true;
        //        }
        //    }
        //}
    }
}
