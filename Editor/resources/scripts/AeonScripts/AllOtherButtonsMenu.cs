using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class AllOtherButtonsMenu : AeonBehaviour
    {
        private AudioComponent HowToPlay = null;
        private bool hasPlayedSound = false;
        private bool hasStopedSound = false;
        private bool hasSpaceTriggeredX1 = false;
        private bool hasSpaceTriggeredX2 = false;
        private bool hasVTriggeredX1 = false;

        public override void OnCreate()
        {
            HowToPlay = entity.GetComponent<AudioComponent>();
        }
        public override void Update()
        {
            if (!hasPlayedSound)
            {
                if (Input.GetKeyPressed(Keys.KEY_SPACE) && !hasSpaceTriggeredX1 && !hasVTriggeredX1)
                {
                    hasSpaceTriggeredX1 = true;
                    if (Input.GetKeyPressed(Keys.KEY_V) && !hasSpaceTriggeredX2)
                    {
                        if (HowToPlay != null)
                        {
                            NativeLog.Log("We Played Menu Button!");
                            HowToPlay.PlaySound();
                            hasPlayedSound = true;
                            hasSpaceTriggeredX2 = true;
                        }
                    }
                    

                }
            }
        }
    }
}

