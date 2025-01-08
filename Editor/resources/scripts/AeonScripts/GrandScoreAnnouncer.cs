using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AeonScriptCore;

namespace AeonScripts
{
    public class GrandScoreAnnouncer : AeonBehaviour
    {
        public AudioComponent audioComp = null;

        public override void OnCreate()
        {
            audioComp = entity.GetComponent<AudioComponent>();
            if ((GameData.P1_L2_Score + GameData.P1_L1_Score) > (GameData.P2_L1_Score + GameData.P2_L2_Score))
            {
                PlayWinnerP1();
            }
            else if ((GameData.P1_L2_Score + GameData.P1_L1_Score) < (GameData.P2_L1_Score + GameData.P2_L2_Score))
            {
                PlayWinnerP2();
            }
            else if ((GameData.P1_L2_Score + GameData.P1_L1_Score) == (GameData.P2_L1_Score + GameData.P2_L2_Score))
            {
                audioComp.SoundFilepath = ".\\resources\\audios\\WinnerScreenBGM.wav";
                audioComp.ChannelType = Audio.ChannelType.BGM;
                audioComp.Volume = 0.7f;
                audioComp.Pan = 0.0f;
                audioComp.PlaySound();
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
