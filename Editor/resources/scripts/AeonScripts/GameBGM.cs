using AeonScriptCore;
using System;


namespace AeonScripts
{
    public class GameBGM : AeonBehaviour
    {
        private AudioComponent playBGM;
        private bool isPlaying = false;

        public override void OnCreate()
        {
            playBGM = entity.GetComponent<AudioComponent>();
            //if (playBGM != null ) { playBGM.PlaySound(); playBGM.FadeIn(4.0f); isPlaying = true; }
            playBGM.SoundFilepath = ".\\resources\\audios\\In-Game BGM.wav";
            playBGM.PlaySound();
            playBGM.FadeIn(4.0f);
        }

        public override void Update()
        {
            //check for playing winning
            if (GameData.PlayerHasWon && isPlaying)
            {
                // Stop the background music
                StopBackgroundMusic();
            }
        }
        private void StopBackgroundMusic()
        {
            if (playBGM != null)
            {
                playBGM.FadeOut(2.0f);
                //playBGM.StopSound();
                isPlaying = false;
            }
        }
    }
}
