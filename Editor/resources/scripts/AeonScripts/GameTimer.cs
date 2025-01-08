using AeonScriptCore;
using System.Threading;

namespace AeonScripts
{
    public class GameTimer : AeonBehaviour
    {
        //public Vector3 DayLightColor = new Vector3(1, 1, 1);
        //public Vector3 NightLightColor = new Vector3(0.6f, 0.6f, 0.6f);
        //public float dayEndTime = 45.0f;
        //public float nightStartTime = 60.0f;
        public Vector3 LastFewSecondsColor = new Vector3(1.0f, 0f, 0f);

        private UI uiComp = null;

        private AudioComponent audioComponent = null;
        private bool hasPlayedLastFewSecs = false;

        private bool hasAudienceClap = false;

        private bool uinull = false;

        //private float _dimDuration = 0.0f;
        //private bool _hasTeleportedSurpriseBox = false;

        public bool isLevel1 = true;

        public override void OnCreate()
        {
            uiComp = entity.GetComponent<UI>();
            audioComponent = entity.GetComponent<AudioComponent>();
        }

        public override void Update()
        {
            if (uiComp == null)
                uiComp = entity.GetComponent<UI>();
            if (audioComponent == null)
                audioComponent = entity.GetComponent<AudioComponent>();

            if (uiComp != null)
            {
                int minutes = (int)(GameData.GameTimer / 60.0f);
                int seconds = (int)(GameData.GameTimer % 60);

                uiComp.Text = (minutes < 10 ? "0" : "") + minutes + ":" + (seconds < 10 ? "0" : "") + seconds;

                if (minutes == 0 && seconds == 5)
                {
                    uiComp.TextColor = new Vector3(1, 0, 0);
                    //uiComp.TextColor = LastFewSecondsColor;
                }

                if (audioComponent != null && GameData.GameTimer <= 5.9f)
                {
                    if (!hasPlayedLastFewSecs && seconds > 1)
                    {
                        audioComponent.SoundFilepath = ".\\resources\\audios\\Timing going down.wav";
                        audioComponent.PlaySound();
                        hasPlayedLastFewSecs = true;
                    }
                    else if (hasPlayedLastFewSecs && GameData.GameTimer <= 0.5f)
                    {
                        hasPlayedLastFewSecs = false;
                        audioComponent.StopSound();
                        
                        audioComponent.SoundFilepath = ".\\resources\\audios\\TimeUpWistle.wav";
                        audioComponent.Volume = 0.5f;
                        audioComponent.PlaySound();

                        uinull = true;
                    }
                }

                
            }
            //if (audioComponent != null && GameData.CheerTimer <= 5.0f && uinull)
            //{
            //    if (!hasAudienceClap && GameData.CheerTimer >= 3.0f)
            //    {
            //        NativeLog.Log("play audio end game");
            //        audioComponent.SoundFilepath = ".\\resources\\audios\\EndGameAudienceCheer.wav";
            //        audioComponent.FadeIn(0.5f);
            //        audioComponent.PlaySound();
            //        hasAudienceClap = true;
            //    }
            //    else if (hasAudienceClap && GameData.CheerTimer >= 1.0f)
            //    {
            //        audioComponent.FadeOut(0.5f);
            //        hasAudienceClap = false;
            //    }
            //}

            if (GameState.IsPaused) return;

            GameData.GameTimer -= Time.DeltaTime;

            //if (GameData.GameTimer < 0f)
            //{
            //    GameState.IsPaused = true;
                
            //    if (!GameData.PlayerHasWon)
            //    {
            //        if (GameData.P1_Score >= GameData.MAX_SCORE)
            //        {
            //            GameData.ShowWinnerScreen(ref transComp, ref uiComponent, true);
            //        }
            //        else if (GameData.P2_Score >= GameData.MAX_SCORE)
            //        {
            //            GameData.ShowWinnerScreen(ref transComp, ref uiComponent, false);
            //        }
            //    }
            //    else
            //    {
            //        if (Input.GetKeyPressed(Keys.KEY_SPACE))
            //        {
            //            GameData.ResetGame();
            //            SceneManager.LoadScene(GameData.GameSceneFile);
            //        }
            //    }
            //}
            
            // between end of day and start of night, decrease color
            //if (_timer > dayEndTime && _timer <= nightStartTime)
            //{
            //    Graphics.EnvironmentLightColor = Vector3.Lerp(DayLightColor, NightLightColor, (_timer - dayEndTime)/_dimDuration);
            //}
        }
    }
}
