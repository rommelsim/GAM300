using AeonScriptCore;
using System;
using System.Security.Cryptography.X509Certificates;

namespace AeonScripts
{
    public class GameData
    {
        public enum Player { Player1, Player2, None };
        public static Player PlayerHoldingBox = Player.None;

        public static Random random = new Random();

        public static readonly string GameLvl1SceneFile = ".\\resources\\scene\\Game_Level1_V2.0.scene";
        public static readonly string GameLvl2SceneFile = ".\\resources\\scene\\Game_Level2_NewLayout_V2.scene";

        public static int P1_Score = 0;
        public static int P2_Score = 0;
        public static int P1_L1_Score = 0;
        public static int P2_L1_Score = 0;
        public static int P1_L2_Score = 0;
        public static int P2_L2_Score = 0;

        public static float GivePointInterval = 2.0f;
        public static readonly int MAX_SCORE = 50;
        public static readonly int TOTAL_BOXES_IN_ZONES = 12;

        public static float MAX_GAME_TIMER { get; private set; } = 120.0f;
        public static float GameTimer = 0.0f; // Only GameTimer.cs can modify!!!

        public static bool PlayerHasWon = false; // if a player has won, then stop all other logic
        public static bool PlayerHasWin = false;
        public static bool PlayerHasDraw = false;

        public static bool RedWon = false;
        public static bool BlueWon = false;

        public static Vector3 voiceTransform;
        public static Vector3 sfxTransform;
        public static Vector3 bgmTransform;
        public static Vector3 gammaTransform;
        public static Vector3 masterTransform;

        public static bool isLevel2 = false;
        public static bool isLevel1 = false;
        public static bool isGrandWinner = false;
        public static bool level1Clear = false;
        public static bool level2Clear = false;
        public static bool fromPause = false;
        
        public static void ResetGame(float setMaxTimer=120.0f)
        {
            GameState.IsPaused = false;
            PlayerHoldingBox = Player.None;
            
            GameTimer = MAX_GAME_TIMER = setMaxTimer;
            if(isLevel2)
            {
                P1_L2_Score = P1_Score;
                P2_L2_Score = P2_Score;
                P1_Score = P2_Score = 0;
            }
            else if(!isLevel2 && isLevel1)
            {
                P1_L1_Score = P1_Score;
                P2_L1_Score = P2_Score;
                P1_Score = P2_Score = 0;
            }
            // if(not level 1 / level 2) reset all score
            
            PlayerHasWon = false;
            PlayerHasWin = false;
            PlayerHasDraw = false;
            RedWon = BlueWon = false;
        }

        public static void ShowWinnerScreen()
        {
            PlayerHasWon = true;
            PlayerHasWin = true;

            SceneManager.RemoveSceneFromCache(GameLvl1SceneFile);
            SceneManager.RemoveSceneFromCache(GameLvl2SceneFile);
            SceneManager.LoadScene(".\\resources\\scene\\V1_SCORE.scene");
        }

        public static void ShowWinnerScreen(ref Transform transformWinnerScreen, ref UI uiCompWinnerScreen, bool p1Won)
        {
            if (uiCompWinnerScreen != null)
            {
                //if (p1Won)
                //    uiCompWinnerScreen.Texture = ".\\resources\\textures\\Contestant1Winner_no-bg_.dds";
                //else
                //    uiCompWinnerScreen.Texture = ".\\resources\\textures\\Contestant2Winner_no-bg_.dds";
                //uiCompWinnerScreen.Opacity = 1.0f;
                //transformWinnerScreen.Scale = new Vector3(500.0f, 500.0f, 1.0f);
                //transformWinnerScreen.Position = new Vector3(0, 300, 0);
                
                PlayerHasWon = true;
                PlayerHasWin = true;

                // TODO: How to identify if level 1 or 2, then remove the appropriate scene cache?
                if (isLevel1)
                {
                    SceneManager.RemoveSceneFromCache(GameLvl1SceneFile);
                }
                else if (isLevel2)
                {
                    SceneManager.RemoveSceneFromCache(GameLvl2SceneFile);
                }
                SceneManager.LoadScene(".\\resources\\scene\\V1_SCORE.scene");
                //GameState.IsPaused = true;

            }
        }

        public static void ShowDrawScreen(ref Transform transformWinnerScreen, ref UI uiCompWinnerScreen)
        {
            if (uiCompWinnerScreen != null)
            {
               // uiCompWinnerScreen.Texture = ".\\resources\\textures\\itsadraw-new.dds";
                //uiCompWinnerScreen.Opacity = 1.0f;
                //transformWinnerScreen.Position = new Vector3(0, 320, 0);
                //transformWinnerScreen.Scale = new Vector3(400);

                PlayerHasWon = true;
                PlayerHasDraw = true;
                //GameState.IsPaused = true;
                if (isLevel1)
                {
                    SceneManager.RemoveSceneFromCache(GameLvl1SceneFile);
                }
                else if (isLevel2)
                {
                    SceneManager.RemoveSceneFromCache(GameLvl2SceneFile);
                }
                SceneManager.LoadScene(".\\resources\\scene\\V1_SCORE.scene");
			}
        }
    }
}