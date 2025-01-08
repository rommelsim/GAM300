using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class DrawBGM : AeonBehaviour
    {
        private AudioComponent playBGM;
        private bool isPlaying = false;

        public override void OnCreate()
        {
            playBGM = entity.GetComponent<AudioComponent>();
        }

        public override void Update()
        {
            if (GameData.PlayerHasDraw && !isPlaying)
            {
                if (playBGM != null)
                {
                    playBGM.PlaySound();
                    playBGM.FadeIn(3.0f);
                    NativeLog.Log("Play and Fade sound");
                    isPlaying = true;
                }
            }
        }
    }
}
