using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AeonScriptCore;

namespace AeonScripts
{
    public class ScoreCheer : AeonBehaviour
    {
        private AudioComponent audioComp = null;

        public override void OnCreate()
        {
            audioComp = entity.GetComponent<AudioComponent>();

            if (audioComp != null)
            {
                audioComp.SoundFilepath = ".\\resources\\audios\\EndGameAudienceCheer.wav";
                audioComp.ChannelType = Audio.ChannelType.SFX;
                audioComp.Volume = 0.4f;
                audioComp.PlaySound();
            }
        }
    }
}
