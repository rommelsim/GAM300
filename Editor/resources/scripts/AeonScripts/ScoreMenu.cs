using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class ScoreMenu : AeonBehaviour
    {
        public Transform trans1 = null;
        public Transform trans2 = null;
        private bool podiumMoving = true;
        private bool podiumMoving2 = true;
        private Vector3 targetposition = Vector3.ZERO;
        private Vector3 targetposition2 = Vector3.ZERO;
        public Entity podium1 = null;
        public Entity podium2 = null;
        public Entity player1 = null;
        public Entity player2 = null;
        public Transform player1trans = null;
        public Transform player2trans = null;
        private Vector3 targetpositionCat1 = Vector3.ZERO;
        private Vector3 targetpositionCat2 = Vector3.ZERO;
        private Vector3 targetScalePodium1 = Vector3.ZERO;
        private Vector3 targetScalePodium2 = Vector3.ZERO;
        public DateTime lastTime = DateTime.MinValue;
        public Entity player1Points = null;
        public Entity player2Points = null;
        public UI player1pts = null;
        public UI player2pts = null;
        public Entity entCam = null;
        public Camera entCamCam = null;
        //private double p1score = 80.0;

        //private double p2score = 20.0;
        public static double maxScore = 50.0;
        public static double podiumHeight = 40.0;
        public static double podium1_ycoord = 0.0, podium2_ycoord = 0.0;
        private float p1startscore = 0f;
        private float p2startscore = 0f;
        public Collider podium1Col = null;
        public Collider podium2Col = null;
        private Vector3 targetposition_p1score = Vector3.ZERO;
        private Vector3 targetposition_p2score = Vector3.ZERO;
        public Transform p1scoretrans = null;
        public Transform p2scoretrans = null;
        public Entity podium1Top = null;
        public Entity podium2Top = null;
        private float p1_timer = 0.0f;
        private Vector3 p1_initial_pos = Vector3.ZERO;
        private Vector3 p1_podium_initial_pos = Vector3.ZERO;
        private Vector3 p1_score_initial_pos = Vector3.ZERO;
        private float p2_timer = 0.0f;
        private Vector3 p2_initial_pos = Vector3.ZERO;
        private Vector3 p2_podium_initial_pos = Vector3.ZERO;
        private Vector3 p2_score_initial_pos = Vector3.ZERO;
        public Transform transpodium1top = null;
        public Transform transpodium2top = null;
        public Entity goBacktoLobby = null;
        public UI uiGoBacktoLobby = null;
        public static string goBackButton = "ReturnButton";
        private float delay = 0f;
        private int seconds = 0;

        public override void OnCreate()
        {
            goBacktoLobby = Entity.FindEntityByName("ReturnButton");
            if (goBacktoLobby != null) uiGoBacktoLobby = goBacktoLobby.GetComponent<UI>();
            uiGoBacktoLobby.Opacity = 0f;
            podium1 = Entity.FindEntityByName("Platform1");
            podium2 = Entity.FindEntityByName("Platform2");
            if (podium1 != null)
            {
                trans1 = podium1.GetComponent<Transform>();
                podium1Col = podium1.GetComponent<Collider>();
            }

            if (podium2 != null)
            {
                trans2 = podium2.GetComponent<Transform>();
                podium2Col = podium2.GetComponent<Collider>();
            }
            podium1Top = Entity.FindEntityByName("Platform1Top");
            podium2Top = Entity.FindEntityByName("Platform2Top");
            if (podium1Top != null) transpodium1top = podium1Top.GetComponent<Transform>();
            if (podium2Top != null) transpodium2top = podium2Top.GetComponent<Transform>();
            player1 = Entity.FindEntityByName("Cat1");
            player2 = Entity.FindEntityByName("Cat2");
            if (player1 != null) player1trans = player1.GetComponent<Transform>();
            if (player2 != null) player2trans = player2.GetComponent<Transform>();
            entCam = Entity.FindEntityByName("camera_placeholder");
            if (entCam != null) entCamCam = entCam.GetComponent<Camera>();
            player1Points = Entity.FindEntityByName("Player1Points");
            player2Points = Entity.FindEntityByName("Player2Points");
            if (player1Points != null)
            {
                player1pts = player1Points.GetComponent<UI>();
                p1scoretrans = player1Points.GetComponent<Transform>();
            }
            if (player2Points != null)
            {
                player2pts = player2Points.GetComponent<UI>();
                p2scoretrans = player2Points.GetComponent<Transform>();
            }

            Vector3 camPos = entCamCam.Position;
            camPos = new Vector3(20.0f, 53.0f, 170.0f);
            entCamCam.Position = camPos;
            entCamCam.Yaw = 0f;

            //if (GameData.P1_Score <= 0) GameData.P1_Score = 0;

            // calculate Y coord
            podium1_ycoord = ((float)GameData.P1_Score / (float)GameData.MAX_SCORE) * podiumHeight;

            targetposition = new Vector3(trans1.Position.X, trans1.Position.Y + (float)podium1_ycoord, trans1.Position.Z);
            targetpositionCat1 = new Vector3(player1trans.Position.X, (float)(player1trans.Position.Y + podium1_ycoord), player1trans.Position.Z);
            targetposition_p1score = new Vector3(p1scoretrans.Position.X, (float)(p1scoretrans.Position.Y + 2* podium1_ycoord), p1scoretrans.Position.Z);
            podium2_ycoord = ((float)GameData.P2_Score / (float)GameData.MAX_SCORE) * podiumHeight;

            targetposition2 = new Vector3(trans2.Position.X, trans2.Position.Y + (float)podium2_ycoord, trans2.Position.Z);
            targetpositionCat2 = new Vector3(player2trans.Position.X, (float)(player2trans.Position.Y + podium2_ycoord), player2trans.Position.Z);
            targetposition_p2score = new Vector3(p2scoretrans.Position.X, (float)(p2scoretrans.Position.Y + 2* podium2_ycoord), p2scoretrans.Position.Z);

            Input.RegisterMapping(goBackButton, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_X);

            p1_initial_pos = player1trans.Position;
            p1_podium_initial_pos = trans1.Position;
            p1_score_initial_pos = p1scoretrans.Position;

            p2_initial_pos = player2trans.Position;
            p2_podium_initial_pos = trans2.Position;
            p2_score_initial_pos = p2scoretrans.Position;

            

        }

        public override void Update()
        {
            

            bool isGamepadConnected = Input.GetConnectedGamepadIds().Length > 0;
            int gamePadOne = 0;
            if (isGamepadConnected)
            {
                gamePadOne = GameInputs.PlayerOne_GamepadId;
            }

            /* Code for scaling version (can't retrieve cube extents so not usable)
            podium1_ycoord = (p1score / maxScore) * podiumHeight;
            //targetposition = new Vector3(trans1.Position.X, (float)podium1_ycoord, trans1.Position.Z);
            targetScalePodium1 = new Vector3(trans1.Scale.X, (float)podium1_ycoord, trans1.Scale.Z);
            targetpositionCat1 = new Vector3(player1trans.Position.X, targetposition.Y + 15f, player1trans.Position.Z);

            podium2_ycoord = (p2score / maxScore) * podiumHeight;
            targetScalePodium2 = new Vector3(trans2.Scale.X, (float)podium2_ycoord, trans2.Scale.Z);
            //targetposition2 = new Vector3(trans2.Position.X, (float)podium2_ycoord, trans2.Position.Z);
            targetpositionCat2 = new Vector3(player2trans.Position.X, targetposition2.Y + 15f, player2trans.Position.Z);
            */

            //NativeLog.Log(podium1_ycoord.ToString());
            /*if (Input.GetKeyPressed(Keys.KEY_K))
            {
                podiumMoving = true;
                p1_initial_pos = player1trans.Position;
                p1_podium_initial_pos = trans1.Position;

                podiumMoving2 = true;
                p2_initial_pos = player2trans.Position;
                p2_podium_initial_pos = trans2.Position;
                
            //}*/

            // calculate Y coord for P2
            // podium2_ycoord = (p2score / maxScore) * podiumHeight;

            //if (Input.GetKeyPressed(Keys.KEY_B)) podiumMoving2 = true;

            if (podiumMoving)
            {
                p1_timer += Time.DeltaTime;
                if (p1_timer > 2.0f) p1_timer = 2.0f;

                // lerp is start * (1-moving 
                float progress = (trans1.Position.Y - p1_podium_initial_pos.Y) / (targetposition.Y - p1_podium_initial_pos.Y) /*MathHelper.Clamp( Vector3.Dist(trans1.Position, targetposition) / podiumMovingThreshold, 0f, 1f)*/;
                //NativeLog.Log(progress.ToString());
                int interpolatedp1score = (int)(p1startscore + (GameData.P1_Score - p1startscore) * progress);
                if (interpolatedp1score <= 0) interpolatedp1score = 0;
                player1pts.Text = interpolatedp1score.ToString();

                //trans1.Scale = Vector3.Lerp(trans1.Scale, targetScalePodium1, 0.01f);
                
                trans1.Position = Vector3.Lerp(p1_podium_initial_pos, targetposition, p1_timer / 2.0f);
                player1trans.Position = Vector3.Lerp(p1_initial_pos, targetpositionCat1, p1_timer / 2.0f);
                transpodium1top.Position = Vector3.Lerp(p1_podium_initial_pos+0.55f, targetposition + 0.55f, p1_timer / 2.0f);
                p1scoretrans.Position = Vector3.Lerp(p1_score_initial_pos, targetposition_p1score, p1_timer / 0.75f);
                if (Vector3.Dist(trans1.Position, targetposition) < 0.1f)
                //if (Vector3.Dist(trans1.Scale, targetScalePodium1) < 0.1f)
                {
                    //podiumMoving = false;
                }

            }

            if (podiumMoving2)
            {
                p2_timer += Time.DeltaTime;
                if (p2_timer > 2.0f) p2_timer = 2.0f;
                float progress2 = (trans2.Position.Y - p2_podium_initial_pos.Y) / (targetposition2.Y - p2_podium_initial_pos.Y);//MathHelper.Clamp(Vector3.Dist(trans2.Position, targetposition2) / podiumMovingThreshold, 0f, 1f);
                int interpolatedp2score = (int)(p2startscore + (GameData.P2_Score - p2startscore) * progress2);
                if (interpolatedp2score < 0) interpolatedp2score = 0;
                player2pts.Text = interpolatedp2score.ToString();
                //trans2.Scale = Vector3.Lerp(trans2.Scale, targetScalePodium2, 0.01f);
                transpodium2top.Position = Vector3.Lerp(p2_podium_initial_pos + 0.55f, targetposition2 + 0.55f, p2_timer / 2.0f);
                trans2.Position = Vector3.Lerp(p2_podium_initial_pos, targetposition2, p2_timer / 2.0f);
                player2trans.Position = Vector3.Lerp(p2_initial_pos, targetpositionCat2, p2_timer / 2.0f);
                p2scoretrans.Position = Vector3.Lerp(p2_score_initial_pos, targetposition_p2score, p2_timer / 0.75f);
                //if (Vector3.Dist(trans2.Scale, targetScalePodium2) < 0.01f)
                if (Vector3.Dist(trans2.Position, targetposition2) < 0.01f)
                {
                    //podiumMoving2 = false;
                }
            }

            
/*            if(GameInputs.PlayerOneInput == GameInputs.InputType.Controller && GameInputs.PlayerTwoInput == GameInputs.InputType.Controller)
            {
                uiGoBacktoLobby.Texture = ".\\resources\\textures\\ReturnController.dds";
            }
            if(GameInputs.PlayerOneInput == GameInputs.InputType.Keyboard && GameInputs.PlayerTwoInput == GameInputs.InputType.Keyboard)
            {
                uiGoBacktoLobby.Texture = ".\\resources\\textures\\Returnkeyboard.dds";
            }
            if((GameInputs.PlayerOneInput == GameInputs.InputType.Controller && GameInputs.PlayerTwoInput == GameInputs.InputType.Keyboard) || (GameInputs.PlayerOneInput == GameInputs.InputType.Keyboard && GameInputs.PlayerTwoInput == GameInputs.InputType.Controller))
            {
                uiGoBacktoLobby.Texture = ".\\resources\\textures\\ReturnKeyboardandController.dds";
            }*/

            if(GameData.isLevel2)
            {
                //uiGoBacktoLobby.Texture = ".\\resources\\textures\\BonusLevelButton.dds";

                uiGoBacktoLobby.Texture = ".\\resources\\textures\\ResultsButtonlvl2winscreen.dds";
            }
/*            else if(GameData.isLevel2 & GameData.isGrandWinner)
            {
                uiGoBacktoLobby.Texture = ".\\resources\\textures\\ChampionButton.dds";

            }
*/


            delay += Time.DeltaTime;
            seconds = (int)(delay % 60);

            if ((seconds > 3) && uiGoBacktoLobby.Opacity == 0f)
            {

                uiGoBacktoLobby.Opacity = 1.0f;
            }

            if ((uiGoBacktoLobby.Opacity == 1.0f) && Input.GetMappingKeyPressed(goBackButton) || GameInputs.IsPlayerGamepadButtonPressed(goBackButton, true) || GameInputs.IsPlayerGamepadButtonPressed(goBackButton, false) || Input.GetKeyPressed(Keys.KEY_M))
            {
                if (GameData.isLevel1)
                {
                    GameData.level1Clear = true;
                    //GameData.isLevel2 = true;
                    GameData.isLevel1 = false;
                    //SceneManager.LoadScene(".\\resources\\scene\\V1_GRAND_WINNER_MENU.scene");
                    SceneManager.LoadScene(".\\resources\\scene\\V1_GAME_MODE.scene");
                }
                else if (GameData.isLevel2)
                {
                    //NativeLog.Log("rooooooooooo");
                    //GameData.isLevel2 = false;
                    //GameData.isLevel1 = true;
                    GameData.level2Clear = true;
                    // go to bonus level
                    GameData.isGrandWinner = true;
                    SceneManager.LoadScene(".\\resources\\scene\\V1_GAME_MODE.scene");
                }
/*                else if(GameData.isGrandWinner)
                {
                    GameData.isGrandWinner = false;
                    SceneManager.LoadScene(".\\resources\\scene\\V1_GRAND_WINNER_MENU.scene");
                }
*/                //SceneManager.LoadScene(".\\resources\\scene\\V1_GRAND_WINNER_SCREEN.scene");
                //SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene");
            }
           // }

        }
    }
}