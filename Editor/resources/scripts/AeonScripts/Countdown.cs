using AeonScriptCore;

namespace AeonScripts
{
    public class Countdown : AeonBehaviour
    {
        public string Player1UIName = "CurrentRedPoint";
        public string Player2UIName = "CurrentBluePoint";
        public float MaxTimer = 120.0f;

        private Transform transComp = null;
        private UI uiComp = null;
        private AudioComponent audioComp = null;
        private bool hasPlayedStart = false;
        private bool hasPlayedThis = false;
        private bool hasPlayedMidScore = false;
        private bool hasPlayedNearEnd = false;

        private float gametimmer2 = 0.0f;

        private Entity player1score = null;
        private Entity player2score = null;
        private UI uiPlayer1 = null;
        private UI uiPlayer2 = null;
        private int score1 = 0;
        private int score2 = 0;

        private float _timer = 0f;
        private float scoreTimer = 0f;
        private int prevSecs = -1;
        private int seconds = 0;
        private bool isCompleted = false;

        private bool hascountdown3played = false;

        private float delayTimer = 0f;

        private Entity scoreBox = null;
        private BoxWithPoint scoreBoxScript = null;

        public override void OnCreate()
        {
            transComp = entity.GetComponent<Transform>();
            uiComp = entity.GetComponent<UI>();
            audioComp = entity.GetComponent<AudioComponent>();

            player1score = Entity.FindEntityByName(Player1UIName);
            player2score = Entity.FindEntityByName(Player2UIName);
            if (player1score != null) uiPlayer1 = player1score.GetComponent<UI>();
            if (player2score != null) uiPlayer2 = player2score.GetComponent<UI>();
            
            GameData.ResetGame(MaxTimer);
            GameState.IsPaused = true;

            scoreBox = Entity.FindEntityByName("Box");
            if (scoreBox != null)
                scoreBoxScript = scoreBox.GetScript<BoxWithPoint>();
        }
        public override void Update()
        {
            //if (GameState.GetEnginePauseState())
            //    GameState.SetEnginePauseState(false);

            if (uiComp == null)
            {
                uiComp = entity.GetComponent<UI>();
            }
            if (uiComp == null) return;

            if (audioComp == null)
            {
                audioComp = entity.GetComponent<AudioComponent>();
            }

            if (audioComp == null) return;

            if (!isCompleted)
            {
                _timer += Time.DeltaTime;
                seconds = (int)(_timer % 60);

                // run once, that is when the curr vs prev time is different
                if (prevSecs != seconds)
                {
                    //audioComp.StopSound();
                    //audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_2.wav";
                    if (seconds < 1)
                    {
                        //if (!hascountdown3played)
                        {
                            audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_2.wav";
                            audioComp.Volume = 0.6f;
                            audioComp.ChannelType = Audio.ChannelType.Voice;
                            audioComp.PlaySound();
                            //hascountdown3played = true;
                        }
                        
                    }
                    if (seconds >= 1 && seconds < 2)
                    {
                        //audioComp.SoundFilepath = ".\\resources\\audios\\2.wav";
                        //audioComp.PlaySound();

                        uiComp.Texture = ".\\resources\\textures\\2(nospace).dds";
                    }
                    if (seconds >= 2 && seconds < 3)
                    {
                        //audioComp.SoundFilepath = ".\\resources\\audios\\1.wav";
                        //audioComp.PlaySound();

                        uiComp.Texture = ".\\resources\\textures\\1(nospace).dds";
                    }
                    if (seconds >= 3 && seconds < 4)
                    {
                        audioComp.SoundFilepath = ".\\resources\\audios\\StartWhistle.wav";
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        audioComp.PlaySound();

                        uiComp.Texture = ".\\resources\\textures\\start-new.dds";
                        transComp.Position = new Vector3(0, 180, 0);
                        transComp.Scale = new Vector3(250);
                    }
                    if (seconds >= 4 && seconds < 5)
                    {
                        audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_4.wav";
                        audioComp.ChannelType = Audio.ChannelType.Voice;
                        audioComp.Volume = 0.7f;
                        audioComp.PlaySound();
                    }
                }
                if (seconds >= 4)
                {
                    uiComp.Opacity = 0.0f;
                    uiComp.Texture = "";
                    GameState.IsPaused = false;
                    isCompleted = true;
                    transComp.Scale = Vector3.ZERO;
                }

                prevSecs = seconds;
                //NativeLog.Log("Seconds" + seconds.ToString());
            }
            else
            {
                if (!GameData.PlayerHasWon)
                {
                    gametimmer2 += Time.DeltaTime;
                    if (GameData.GameTimer > 0.0f)
                    {
                        uiPlayer1.Text = GameData.P1_Score.ToString();
                        uiPlayer2.Text = GameData.P2_Score.ToString();

                        if (GameState.IsPaused) return;
                        if (!GameData.RedWon && !GameData.BlueWon)
                        {
                            if (GameData.PlayerHoldingBox != GameData.Player.None)
                                scoreTimer += Time.DeltaTime;
                            else
                                scoreTimer = 0.0f;
                            if (scoreTimer >= 1.0f)
                            {

                                NativeLog.Log("Scoretime: ", scoreTimer.ToString());
                                // Player one picks up the box, start increasing the score.
                                if (GameData.PlayerHoldingBox == GameData.Player.Player1)
                                {
                                    // delayTimer += Time.DeltaTime;
                                    //if (delayTimer >= 1f)
                                    //{
                                    GameData.P1_Score += 1;
                                    //score1 += 1;
                                    NativeLog.LogWarn(GameData.P1_Score.ToString());
                                    scoreTimer = 0.0f;

                                    if (scoreBoxScript != null)
                                    {
                                        scoreBoxScript.PlayScoreSFX(".\\resources\\audios\\PickupBox_Red.wav");
                                        scoreBoxScript.ScoreIncreaseVisuals(new Vector3(7f, 7f, 7f));
                                    }
                                }
                                else if (GameData.PlayerHoldingBox == GameData.Player.Player2)
                                {
                                    // 
                                    GameData.P2_Score += 1;
                                    //score2 += 1;
                                    scoreTimer = 0.0f;

                                    if (scoreBoxScript != null)
                                    {
                                        scoreBoxScript.PlayScoreSFX(".\\resources\\audios\\PickupBox_Blue.wav");
                                        scoreBoxScript.ScoreIncreaseVisuals(new Vector3(7f, 7f, 7f));
                                    }
                                }
                                //scoreTimer -= 1.0f;
                            }
                        }
                        uiPlayer1.Text = GameData.P1_Score.ToString();
                        uiPlayer2.Text = GameData.P2_Score.ToString();

                        if (!hasPlayedStart)
                        {
                            //NativeLog.Log("Has Played Start?");
                            if (gametimmer2 >= 5.0f && gametimmer2 <= 30.0f)
                            {
                               // NativeLog.Log("Has Played Start?");
                                if (audioComp != null)
                                {
                                    if (GameData.P1_Score > 3)
                                    {
                                        audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_5.wav";
                                        audioComp.ChannelType = Audio.ChannelType.Voice;
                                        audioComp.Volume = 0.8f;
                                        audioComp.PlaySound();
                                        hasPlayedStart = true;
                                    }
                                    else if (GameData.P2_Score > 3)
                                    {
                                        audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_6.wav";
                                        audioComp.ChannelType = Audio.ChannelType.Voice;
                                        audioComp.Volume = 0.8f;
                                        audioComp.PlaySound();
                                        hasPlayedStart = true;
                                    }
                                }
                            }
                        }
                        if (gametimmer2 >= 35.0f && gametimmer2 <= 40 && !hasPlayedThis)
                        {
                            if (audioComp != null)
                            {
                                if (GameData.P1_Score < 30 && GameData.P2_Score < 30)
                                {
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_10.wav";
                                    audioComp.ChannelType = Audio.ChannelType.Voice;
                                    audioComp.Volume = 0.7f;
                                    audioComp.PlaySound();
                                    hasPlayedThis = true;
                                }
                            }
                        }
                        if (gametimmer2 > 70.0f && gametimmer2 < 80.0f && !hasPlayedMidScore)
                        {
                            if (audioComp != null)
                            {
                                if (GameData.P1_Score == GameData.P2_Score)
                                {
                                    // its a close match folks
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_9.wav";
                                    audioComp.ChannelType = Audio.ChannelType.Voice;
                                    audioComp.Volume = 0.7f;
                                    audioComp.PlaySound();
                                    hasPlayedMidScore = true;
                                }
                                else if (GameData.P1_Score > GameData.P2_Score)
                                {
                                    // red sets the pace
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_7.wav";
                                    audioComp.ChannelType = Audio.ChannelType.Voice;
                                    audioComp.Volume = 0.7f;
                                    audioComp.PlaySound();
                                    hasPlayedMidScore = true;
                                }
                                else if (GameData.P1_Score < GameData.P2_Score)
                                {
                                    // blue set the pace
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_8.wav";
                                    audioComp.ChannelType = Audio.ChannelType.Voice;
                                    audioComp.Volume = 0.7f;
                                    audioComp.PlaySound();
                                    hasPlayedMidScore = true;
                                }
                            }
                            
                        }
                        if (gametimmer2 > 90.0f && gametimmer2 < 100.0f && !hasPlayedNearEnd)
                        {
                            if (audioComp != null)
                            {
                                if (GameData.P1_Score == GameData.P2_Score)
                                {
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_10.wav";
                                    audioComp.ChannelType = Audio.ChannelType.Voice;
                                    audioComp.Volume = 0.7f;
                                    audioComp.PlaySound();
                                    hasPlayedNearEnd = true;
                                }
                                else if (GameData.P1_Score > GameData.P2_Score)
                                {
                                    // red sets the pace
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_7.wav";
                                    audioComp.ChannelType = Audio.ChannelType.Voice;
                                    audioComp.Volume = 0.7f;
                                    audioComp.PlaySound();
                                    hasPlayedNearEnd = true;
                                }
                                else if (GameData.P1_Score < GameData.P2_Score)
                                {
                                    // blue set the pace
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Anouncer_8.wav";
                                    audioComp.ChannelType = Audio.ChannelType.Voice;
                                    audioComp.Volume = 0.7f;
                                    audioComp.PlaySound();
                                    hasPlayedNearEnd = true;
                                }
                            }
                        }

                        if (GameData.P1_Score >= GameData.MAX_SCORE)
                        {
                            GameData.RedWon = true;
                            GameData.BlueWon = false;
                            GameData.ShowWinnerScreen(ref transComp, ref uiComp, true);
                            //SceneManager.LoadScene(".\\resources\\scene\\V1_SCORE.scene");
                        }
                        else if (GameData.P2_Score >= GameData.MAX_SCORE)
                        {
                            GameData.BlueWon = true;
                            GameData.RedWon = false;
                            //SceneManager.LoadScene(".\\resources\\scene\\V1_SCORE.scene");
                            GameData.ShowWinnerScreen(ref transComp, ref uiComp, false);
                        }
                    }
                    else
                    {

                        // If not a draw, means one player scored higher than the other
                        if (GameData.P1_Score != GameData.P2_Score)
                        {
                            if (GameData.P1_Score > GameData.P2_Score)
                            {
                                GameData.RedWon = true;
                                GameData.BlueWon = false;
                            }
                            else if (GameData.P2_Score > GameData.P1_Score)
                            {
                                GameData.BlueWon = true;
                                GameData.RedWon = false;
                            }
                            GameData.ShowWinnerScreen(ref transComp, ref uiComp, GameData.P1_Score > GameData.P2_Score);
                        }
                        // is a draw 
                        else
                        {
                            GameData.RedWon = false;
                            GameData.BlueWon = false;
                            GameData.ShowDrawScreen(ref transComp, ref uiComp);
                        }
                    }
                }
                else
                {
                    if (Input.GetKeyPressed(Keys.KEY_SPACE))
                    {
                        GameData.ResetGame();
                        //SceneManager.LoadScene(GameData.GameSceneFile);
                        // TODO: SceneManager.RemoveSceneFromCache()
                        SceneManager.LoadScene(".\\resources\\scene\\V1_SCORE.scene");
                    }
                }
            }
        }
    }
}
