using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class ScoreMenuAnnouncer : AeonBehaviour
    {
        public AudioComponent audioComp = null;
        public override void OnCreate()
        {
            audioComp = entity.GetComponent<AudioComponent>();
            if (GameData.P1_Score > GameData.P2_Score)
            {
                PlayWinnerP1();
            }
            else if (GameData.P1_Score < GameData.P2_Score)
            {
                PlayWinnerP2();
            }
        }
        public void PlayWinnerP1()
        {
            int randNum = GameData.random.Next(1, 4);
            if (audioComp != null)
            {
                //audioComp.SoundFilepath = $".\\resources\\audios\\Red Cat_Win_{randNum}.wav";
                audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_12.wav";
                audioComp.ChannelType = Audio.ChannelType.Voice;
                audioComp.Volume = 0.6f;
                audioComp.Pan = -0.5f;
                audioComp.PlaySound();
            }
        }
        public void PlayWinnerP2()
        {
            int randNum = GameData.random.Next(1, 4);
            if (audioComp != null)
            {
                //audioComp.SoundFilepath = $".\\resources\\audios\\Blue Cat_Win_{randNum}.wav";
                audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_13.wav";
                audioComp.ChannelType = Audio.ChannelType.Voice;
                audioComp.Volume = 0.6f;
                audioComp.Pan = 0.5f;
                audioComp.PlaySound();
            }
        }
    }
}
