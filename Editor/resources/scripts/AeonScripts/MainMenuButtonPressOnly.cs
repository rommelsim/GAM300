using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class MainMenuButtonPressOnly : AeonBehaviour
    {
        private AudioComponent playButton = null;

        private bool hasPlayedSound = false;

        private bool hasSpaceTriggeredX1 = false;

        public override void OnCreate()
        {
            playButton = entity.GetComponent<AudioComponent>();
        }

        public override void Update()
        {
            //_timer += Time.DeltaTime;
            if (!hasPlayedSound)
            {
                if (Input.GetKeyPressed(Keys.KEY_SPACE) && !hasSpaceTriggeredX1)
                {
                    if (playButton != null) 
                    {
                        NativeLog.Log("We Played Menu Button!");
                        playButton.PlaySound();
                        //playButton.FadeIn(5.0f);
                        hasPlayedSound = true;
                        hasSpaceTriggeredX1 = true;
                    }
                }
            }
        }
    }
}
