using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class WinnerBGM : AeonBehaviour
    {
        private AudioComponent playBGM;
        private bool isPlaying = false;

        public override void OnCreate()
        {
            playBGM = entity.GetComponent<AudioComponent>();
        }

        public override void Update()
        {
            if (GameData.PlayerHasWin && !isPlaying )
            {
                if (playBGM != null)
                {
                    playBGM.Volume = 0.5f;
                    playBGM.PlaySound();
                    playBGM.FadeIn(3.0f);
                    isPlaying = true;
                }
            }
        }
    }


}
