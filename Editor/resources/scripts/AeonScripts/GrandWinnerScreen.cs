using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class GrandWinnerScreen : AeonBehaviour
    {
        // Player 1

        public Entity player1 = null;
        public Transform trans1 = null;
        public Transform player1trans = null;
        private Vector3 targetpositionCat1 = Vector3.ZERO;
        private Vector3 p1_initial_pos = Vector3.ZERO;
        private Vector3 p1_jump_endpos = Vector3.ZERO;

        // Player 1 Score
        public Entity player1Points = null;
        public UI player1pts = null;
        //private double p1score = 12.0; // for actual use p1_score + p1_L2_score
        private double p1score = GameData.P1_Score + GameData.P1_L2_Score;
        private float p1startscore = 0f;
        private Vector3 targetposition_p1score = Vector3.ZERO;
        public Transform p1scoretrans = null;
        private Vector3 p1_score_initial_pos = Vector3.ZERO;

        // Player 2
        public Entity player2 = null;
        public Transform trans2 = null;
        public Transform player2trans = null;
        private Vector3 targetpositionCat2 = Vector3.ZERO;
        private Vector3 p2_jump_endpos = Vector3.ZERO;
        
        // Player 2 Score
        public Entity player2Points = null;
        public UI player2pts = null;
        //private double p2score = 18.0; // for actual use p2_score + p2_L2_score
        private double p2score = GameData.P2_Score + GameData.P2_L2_Score;
        private float p2startscore = 0f;
        private Vector3 targetposition_p2score = Vector3.ZERO;
        public Transform p2scoretrans = null;
        
        // Podium 1
        public Entity podium1 = null;
        private bool podiumMoving = true;
        private Vector3 targetposition = Vector3.ZERO;
        private Vector3 targetScalePodiumWinner = new Vector3(30f, 35f, 30f); // if winner
        public static double podium1_ycoord = 0.0;
        public Collider podium1Col = null;
        private Vector3 p1_podium_initial_pos = Vector3.ZERO;
        
        public Entity podium1Top = null;
        
        // Podium 2
        public Entity podium2 = null;
        private bool podiumMoving2 = true;
        private Vector3 targetposition2 = Vector3.ZERO;
        private Vector3 targetScalePodiumLose = new Vector3(12f,10,12f); // if loser
        public static double podium2_ycoord = 0.0;
        public Collider podium2Col = null;
        
        public Entity podium2Top = null;
        
        // Misc
        public DateTime lastTime = DateTime.MinValue;
        private float p1_timer = 0.0f;
        private float p2_timer = 0.0f;
        public static double maxScore = 100.0;
        private static double podiumHeight = 10.0;
        public int dir = 1;

        // Camera
        public Entity entCam = null;
        public Camera entCamCam = null;

        // Aspect Ratios
        float podiumToptoBottomAspectRatio = 0f;
        float catToPodiumTopAspectRatio = 0f;

        // Background
        public Entity background = null;
        public UI uibackground = null;

        private Vector3 p2_initial_pos = Vector3.ZERO;
        private Vector3 p2_podium_initial_pos = Vector3.ZERO;
        private Vector3 p2_score_initial_pos = Vector3.ZERO;
        public Transform transpodium1top = null;
        public Transform transpodium2top = null;
        public Entity goCredits = null;
        public UI uigoCredits = null;
        public static string goCreditsBtn = "CreditsButton";
        private Vector3 targetScalePodiumTop1 = new Vector3(20f, 25f, 20f);
        private float posAspectRatioCatToPodiumTop = 0f;
        private float p1_scale_timer = 0f;
        private float p2_scale_timer = 0f;
        private float movement_timer = 0f;
        private float movement_timer2 = 0f;
        private float p1_jump_timer = 0f;
        private float p2_jump_timer = 0f;
        private Vector3 p1_jump_start = Vector3.ZERO;
        private Vector3 p2_jump_start = Vector3.ZERO;
        public Collider podium1TopCollider = null;
        public static bool isDraw = false;
        bool p1Wins = false;
        bool p2Wins = false;
        private float delay = 0f;
        private int seconds = 0;
        private float delayFade = 1.5f;
        private float delayFadetimer = 0f;
        private float delayFade2 = 1.5f;
        private float delayFadetimer2 = 0f;

        public static bool grandWinner = false;

        // Draw Scene Stuff
        public Entity draw = null;
        public UI uiDraw = null;

        public override void OnCreate()
        {

            // Check Score Condition
            //draw = Entity.Fomd
            
            background = Entity.FindEntityByName("Background");
            if (background != null) uibackground = background.GetComponent<UI>();

            goCredits = Entity.FindEntityByName("CreditsButton");
            if (goCredits != null) uigoCredits = goCredits.GetComponent<UI>();
            uigoCredits.Opacity = 0f;

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
            if (podium1Top != null)
            {
                transpodium1top = podium1Top.GetComponent<Transform>();
                podium1TopCollider = podium1Top.GetComponent<Collider>();
            }
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
            //podium1_ycoord = ((float)GameData.P1_Score / (float)GameData.MAX_SCORE) * podiumHeight;
            podium1_ycoord = (p1score / 100f) * podiumHeight;

            targetposition = new Vector3(trans1.Position.X, trans1.Position.Y + (float)podium1_ycoord, trans1.Position.Z);
            targetpositionCat1 = new Vector3(player1trans.Position.X, (float)(player1trans.Position.Y + podium1_ycoord), player1trans.Position.Z);
            targetposition_p1score = new Vector3(p1scoretrans.Position.X, (float)(p1scoretrans.Position.Y + 2 * podium1_ycoord), p1scoretrans.Position.Z);
            //podium2_ycoord = ((float)GameData.P2_Score / (float)GameData.MAX_SCORE) * podiumHeight;
            podium2_ycoord = ((float)p2score / 100f) * podiumHeight;


            targetposition2 = new Vector3(trans2.Position.X, trans2.Position.Y + (float)podium2_ycoord, trans2.Position.Z);
            targetpositionCat2 = new Vector3(player2trans.Position.X, (float)(player2trans.Position.Y + podium2_ycoord), player2trans.Position.Z);
            targetposition_p2score = new Vector3(p2scoretrans.Position.X, (float)(p2scoretrans.Position.Y + 2 * podium2_ycoord), p2scoretrans.Position.Z);

            Input.RegisterMapping(goCreditsBtn, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_X);

            p1_initial_pos = player1trans.Position;
            p1_podium_initial_pos = trans1.Position;
            p1_score_initial_pos = p1scoretrans.Position;

            p2_initial_pos = player2trans.Position;
            p2_podium_initial_pos = trans2.Position;
            p2_score_initial_pos = p2scoretrans.Position;

            podiumToptoBottomAspectRatio = transpodium1top.Scale.X / trans1.Scale.X;
            catToPodiumTopAspectRatio = player1trans.Scale.X / transpodium1top.Scale.X;
            posAspectRatioCatToPodiumTop = player1trans.Position.Y / transpodium1top.Position.Y;

            if (p1score > p2score)
            {
                p1Wins = true;

                p2Wins = false;
                isDraw = false;
            }
            else if (p2score > p1score)
            {
                p2Wins = true;

                p1Wins = false;
                isDraw = false;

            }
            else if (p1score == p2score)
            {
                isDraw = true;

                p1Wins = false;
                p2Wins = false;
                

            }

            grandWinner = true;
        }

        public override void Update()
        {


            bool isGamepadConnected = Input.GetConnectedGamepadIds().Length > 0;
            int gamePadOne = 0;
            if (isGamepadConnected)
            {
                gamePadOne = GameInputs.PlayerOne_GamepadId;
            }

            // both podium rises
            // winner podium scales larger & loser podium scales smaller
            // winner podium moves to the centre
            // loser podium moves to the side (left for blue cat, right for red cat) Fast transition
            // add spotlight shine on winner or make the winner brighter
            // winner cat jump + confetti
            // after awhile fade to black to show credit screen


            // calculate Y coord for P2
            // podium2_ycoord = (p2score / maxScore) * podiumHeight;

            if (podiumMoving)
            {
                p1_timer += Time.DeltaTime;
                if (p1_timer > 2.0f) p1_timer = 2.0f;

                
                float progress = (trans1.Position.Y - p1_podium_initial_pos.Y) / (targetposition.Y - p1_podium_initial_pos.Y) /*MathHelper.Clamp( Vector3.Dist(trans1.Position, targetposition) / podiumMovingThreshold, 0f, 1f)*/;
                int interpolatedp1score = (int)(p1startscore + (p1score - p1startscore) * progress);
                //int interpolatedp1score = (int)(p1startscore + (GameData.P1_Score - p1startscore) * progress);
                if (interpolatedp1score <= 0) interpolatedp1score = 0;
                player1pts.Text = interpolatedp1score.ToString();

                // interpolates the podium, podium top + player 1 smoothly.
                trans1.Position = Vector3.Lerp(p1_podium_initial_pos, targetposition, p1_timer / 2.0f);
                player1trans.Position = Vector3.Lerp(p1_initial_pos, targetpositionCat1, p1_timer / 2.0f);
                transpodium1top.Position = Vector3.Lerp(p1_podium_initial_pos + 0.55f, targetposition + 0.55f, p1_timer / 2.0f);
                p1scoretrans.Position = Vector3.Lerp(p1_score_initial_pos, targetposition_p1score, p1_timer / 0.75f);

               
                if (Vector3.Dist(trans1.Position, targetposition) < 0.1f)
                {
                    // P1 Wins - move towards centre
                    if (p1Wins)
                    {
                        p1_scale_timer += Time.DeltaTime;
                        if (p1_scale_timer > 2.0f) p1_scale_timer = 2.0f;
                        movement_timer += Time.DeltaTime * 2.0f;
                        if (movement_timer > 0.5f) movement_timer = 0.5f;

                        // Enlarging Cat + Podium + Podium Top (Scaling)
                        Vector3 targetpodiumtop = new Vector3(targetScalePodiumWinner.X * podiumToptoBottomAspectRatio, targetScalePodiumWinner.Y * podiumToptoBottomAspectRatio, targetScalePodiumWinner.Z * podiumToptoBottomAspectRatio);
                        trans1.Scale = Vector3.Lerp(trans1.Scale, targetScalePodiumWinner, p1_scale_timer);
                        transpodium1top.Scale = Vector3.Lerp(transpodium1top.Scale, targetpodiumtop, p1_scale_timer);
                        Vector3 targetCat = new Vector3(targetpodiumtop.X * catToPodiumTopAspectRatio, targetpodiumtop.Y * catToPodiumTopAspectRatio, targetpodiumtop.Z * catToPodiumTopAspectRatio);
                        player1trans.Scale = Vector3.Lerp(player1trans.Scale, targetCat, p1_scale_timer);

                        // Moving them to the center of the screen as  winners
                        Vector3 targetCatPos = new Vector3(player1trans.Position.X + 30f, transpodium1top.Position.Y + 32f, player1trans.Position.Z);
                        
                        //player1trans.Position = Vector3.Lerp(player1trans.Position, new Vector3(player1trans.Position.X, /*player1trans.Position.Y + (transpodium1top.Position.Y * catToPodiumTopAspectRatio)*/65f, player1trans.Position.Z), 0.03f);
                        //player1trans.Position = new Vector3(player1trans.Position.X, 65f, player1trans.Position.Z);
                        player1trans.Position = Vector3.Lerp(player1trans.Position, targetCatPos, movement_timer / 0.8f);
                        transpodium1top.Position = Vector3.Lerp(transpodium1top.Position, new Vector3(transpodium1top.Position.X + 30f, transpodium1top.Position.Y, transpodium1top.Position.Z), movement_timer / 0.8f);
                        p1scoretrans.Position = Vector3.Lerp(p1scoretrans.Position, new Vector3(p1scoretrans.Position.X + 135f, player1trans.Position.Y + 60f, p1scoretrans.Position.Z), movement_timer / 0.8f);
                        trans1.Position = Vector3.Lerp(trans1.Position, new Vector3(trans1.Position.X + 30f, trans1.Position.Y, trans1.Position.Z), movement_timer / 0.8f);

                        // Check if they reach the centre
                        if(movement_timer >= 0.5f)
                        //if (Vector3.Dist(player1trans.Position, targetCatPos) < 0.1f)
                        {
                            if (p1_jump_start == Vector3.ZERO)
                            {
                                p1_jump_start = player1trans.Position;
                                p1_jump_endpos = new Vector3(p1_jump_start.X, p1_jump_start.Y + 10f, p1_jump_start.Z);
                            }

                            //if (p1_jump_timer < (MathHelper.PI / 2.0f)
                            p1_jump_timer += Time.DeltaTime * 2.0f;
                            if (p1_jump_timer >= MathHelper.PI)
                            {
                                p1_jump_timer = 0.0f;
                            }
                            // jump
                            //NativeLog.Log("jumpppppp");
                            Vector3 target = curMoveTgt();
                            Vector3 jumpPos = Vector3.Lerp(p1_jump_start, p1_jump_endpos, MathHelper.Sin(p1_jump_timer));
                            if (jumpPos.Y < p1_jump_start.Y)
                            {
                                jumpPos.Y = p1_jump_start.Y;
                            }
                            player1trans.Position = jumpPos;
                            float distance = (target - (Vector3)player1trans.Position).GetMagnitude();

                            if(distance <= 0.1f)
                            {
                                dir *= -1;
                            }
                        }
                    }
                    // P1 Loses - move away from centre
                    if(p2Wins)
                    {
                        p1_scale_timer += Time.DeltaTime;
                        if (p1_scale_timer > 2.0f) p1_scale_timer = 2.0f;
                        movement_timer += Time.DeltaTime * 2.0f;
                        if (movement_timer > 0.5f) movement_timer = 0.5f;
                        delayFade += Time.DeltaTime;

                        // Scale down the cat , podium + podium top
                        trans1.Scale = Vector3.Lerp(trans1.Scale, targetScalePodiumLose, p1_scale_timer);
                        Vector3 targetpodiumtop1 = new Vector3(targetScalePodiumLose.X * podiumToptoBottomAspectRatio, targetScalePodiumLose.Y * podiumToptoBottomAspectRatio, targetScalePodiumLose.Z * podiumToptoBottomAspectRatio);
                        transpodium1top.Scale = Vector3.Lerp(transpodium1top.Scale, targetpodiumtop1, p1_scale_timer);
                        Vector3 targetCat2 = new Vector3(targetpodiumtop1.X * catToPodiumTopAspectRatio, targetpodiumtop1.Y * catToPodiumTopAspectRatio, targetpodiumtop1.Z * catToPodiumTopAspectRatio);
                        //player1trans.Scale = Vector3.Lerp(player1trans.Scale, targetCat2, p1_scale_timer);

                        // Move them aside
                       trans1.Position = Vector3.Lerp(trans1.Position, new Vector3(trans1.Position.X - 15f, trans1.Position.Y, trans1.Position.Z), movement_timer / 0.8f);
                       transpodium1top.Position = Vector3.Lerp(transpodium1top.Position, new Vector3(transpodium1top.Position.X - 15f, transpodium1top.Position.Y, transpodium1top.Position.Z), movement_timer / 0.8f);
                       player1trans.Position = Vector3.Lerp(player1trans.Position, new Vector3(player1trans.Position.X - 15f, player1trans.Position.Y - 3f, player1trans.Position.Z), movement_timer / 0.8f);
                       p1scoretrans.Position = Vector3.Lerp(p1scoretrans.Position, new Vector3(p1scoretrans.Position.X - 80f, p1scoretrans.Position.Y, p1scoretrans.Position.Z), movement_timer / 0.8f);


                        //if (delayFade > 2f)
                        //{
                            delayFadetimer += Time.DeltaTime;
                            if (delayFadetimer > 2.0f) delayFadetimer = 2.0f;
                            
                            trans1.Position = Vector3.Lerp(trans1.Position, new Vector3(trans1.Position.X, trans1.Position.Y - 50f, trans1.Position.Z), delayFadetimer / 0.3f);
                            transpodium1top.Position = Vector3.Lerp(transpodium1top.Position, new Vector3(transpodium1top.Position.X, transpodium1top.Position.Y - 50f, transpodium1top.Position.Z), delayFadetimer );
                            player1trans.Position = Vector3.Lerp(player1trans.Position, new Vector3(player1trans.Position.X, player1trans.Position.Y - 50f, player1trans.Position.Z), delayFadetimer );
                            p1scoretrans.Position = Vector3.Lerp(p1scoretrans.Position, new Vector3(p1scoretrans.Position.X, p1scoretrans.Position.Y - 80f, p1scoretrans.Position.Z), delayFadetimer/0.5f );
                        //}
/*                        delayFadetimer += Time.DeltaTime;

                        if (delayFadetimer >= delayFade)
                        {
                            Vector3 addTrans = player1trans.Position;
                            Vector3 addTransPodiumTop = transpodium1top.Position;
                            Vector3 addTransPodium = trans1.Position;
                            addTrans.Y -= 0.5f * Time.DeltaTime;
                            addTransPodiumTop -= 0.5f * Time.DeltaTime;
                            addTransPodium -= 0.5f * Time.DeltaTime;

                            if(player1trans.Position.Y <= -50f)
                            {
                                delayFadetimer = 0f;
                            }

                            trans1.Position = addTransPodium;
                            transpodium1top.Position = addTransPodiumTop;
                            player1trans.Position = addTrans;

                        }
*/


                    }
                    if (isDraw)
                    {
                        p1_scale_timer += Time.DeltaTime;
                        if (p1_scale_timer > 2.0f) p1_scale_timer = 2.0f;
                        movement_timer += Time.DeltaTime * 2.0f;
                        if (movement_timer > 0.5f) movement_timer = 0.5f;

                        // Enlarging Cat + Podium + Podium Top (Scaling)
                        Vector3 targetpodiumtop = new Vector3(targetScalePodiumWinner.X * podiumToptoBottomAspectRatio, targetScalePodiumWinner.Y * podiumToptoBottomAspectRatio, targetScalePodiumWinner.Z * podiumToptoBottomAspectRatio);
                        trans1.Scale = Vector3.Lerp(trans1.Scale, targetScalePodiumWinner, p1_scale_timer);
                        transpodium1top.Scale = Vector3.Lerp(transpodium1top.Scale, targetpodiumtop, p1_scale_timer);
                        Vector3 targetCat = new Vector3(targetpodiumtop.X * catToPodiumTopAspectRatio, targetpodiumtop.Y * catToPodiumTopAspectRatio, targetpodiumtop.Z * catToPodiumTopAspectRatio);
                        player1trans.Scale = Vector3.Lerp(player1trans.Scale, targetCat, p1_scale_timer);

                        // Moving them to the center of the screen as  winners
                        Vector3 targetCatPos = new Vector3(player1trans.Position.X + 10f, transpodium1top.Position.Y + 32f, player1trans.Position.Z);

                        ////player1trans.Position = Vector3.Lerp(player1trans.Position, new Vector3(player1trans.Position.X, /*player1trans.Position.Y + (transpodium1top.Position.Y * catToPodiumTopAspectRatio)*/65f, player1trans.Position.Z), 0.03f);
                        ////player1trans.Position = new Vector3(player1trans.Position.X, 65f, player1trans.Position.Z);
                        player1trans.Position = Vector3.Lerp(player1trans.Position, targetCatPos, movement_timer / 0.8f);
                        transpodium1top.Position = Vector3.Lerp(transpodium1top.Position, new Vector3(transpodium1top.Position.X + 10f, transpodium1top.Position.Y, transpodium1top.Position.Z), movement_timer / 0.8f);
                        p1scoretrans.Position = Vector3.Lerp(p1scoretrans.Position, new Vector3(p1scoretrans.Position.X + 50f, player1trans.Position.Y + 50f, p1scoretrans.Position.Z), movement_timer / 0.8f);
                        trans1.Position = Vector3.Lerp(trans1.Position, new Vector3(trans1.Position.X + 10f, trans1.Position.Y, trans1.Position.Z), movement_timer / 0.8f);
                        if (movement_timer >= 0.5f)
                        //if (Vector3.Dist(player1trans.Position, targetCatPos) < 0.1f)
                        {
                            if (p1_jump_start == Vector3.ZERO)
                            {
                                p1_jump_start = player1trans.Position;
                                p1_jump_endpos = new Vector3(p1_jump_start.X, p1_jump_start.Y + 10f, p1_jump_start.Z);
                            }

                            //if (p1_jump_timer < (MathHelper.PI / 2.0f)
                            p1_jump_timer += Time.DeltaTime * 2.0f;
                            if (p1_jump_timer >= MathHelper.PI)
                            {
                                p1_jump_timer = 0.0f;
                            }
                            // jump
                            //NativeLog.Log("jumpppppp");
                            Vector3 target = curMoveTgt();
                            Vector3 jumpPos = Vector3.Lerp(p1_jump_start, p1_jump_endpos, MathHelper.Sin(p1_jump_timer));
                            if (jumpPos.Y < p1_jump_start.Y)
                            {
                                jumpPos.Y = p1_jump_start.Y;
                            }
                            player1trans.Position = jumpPos;
                            float distance = (target - (Vector3)player1trans.Position).GetMagnitude();

                            if (distance <= 0.1f)
                            {
                                dir *= -1;
                            }
                        }
                    }
                }
                

            }

            if (podiumMoving2)
            {
                p2_timer += Time.DeltaTime;
                if (p2_timer > 2.0f) p2_timer = 2.0f;

                float progress2 = (trans2.Position.Y - p2_podium_initial_pos.Y) / (targetposition2.Y - p2_podium_initial_pos.Y);//MathHelper.Clamp(Vector3.Dist(trans2.Position, targetposition2) / podiumMovingThreshold, 0f, 1f);
                //int interpolatedp2score = (int)(p2startscore + (GameData.P2_Score - p2startscore) * progress2);
                int interpolatedp2score = (int)(p2startscore + (p2score - p2startscore) * progress2);

                if (interpolatedp2score < 0) interpolatedp2score = 0;
                player2pts.Text = interpolatedp2score.ToString();
                transpodium2top.Position = Vector3.Lerp(p2_podium_initial_pos + 0.55f, targetposition2 + 0.55f, p2_timer / 2.0f);
                trans2.Position = Vector3.Lerp(p2_podium_initial_pos, targetposition2, p2_timer / 2.0f);
                player2trans.Position = Vector3.Lerp(p2_initial_pos, targetpositionCat2, p2_timer / 2.0f);
                p2scoretrans.Position = Vector3.Lerp(p2_score_initial_pos, targetposition_p2score, p2_timer / 0.75f);
                
                if (Vector3.Dist(trans2.Position, targetposition2) < 0.1f)
                {
                    // P2 Loses - turn small and move aside
                    if (p1Wins)
                    {
                        //podiumMoving2 = false;
                        p2_scale_timer += Time.DeltaTime;
                        if (p2_scale_timer > 2.0f) p2_scale_timer = 2.0f;
                        movement_timer2 += Time.DeltaTime * 2.0f;
                        if (movement_timer2 > 0.5f) movement_timer2 = 0.5f;
                        delayFade2 += Time.DeltaTime;
                        Vector3 targetpodiumtop2 = new Vector3(targetScalePodiumLose.X * podiumToptoBottomAspectRatio, targetScalePodiumLose.Y * podiumToptoBottomAspectRatio, targetScalePodiumLose.Z * podiumToptoBottomAspectRatio);
                        trans2.Scale = Vector3.Lerp(trans2.Scale, targetScalePodiumLose, p2_scale_timer);
                        //transpodium1top.Scale = Vector3.Lerp(transpodium1top.Scale, targetScalePodium1, 0.01f);
                        transpodium2top.Scale = Vector3.Lerp(transpodium2top.Scale, targetpodiumtop2, 0.01f);
                        Vector3 targetCat2 = new Vector3(targetpodiumtop2.X * catToPodiumTopAspectRatio, targetpodiumtop2.Y * catToPodiumTopAspectRatio, targetpodiumtop2.Z * catToPodiumTopAspectRatio);
                        //player2trans.Scale = Vector3.Lerp(player2trans.Scale, targetCat2, movement_timer2);

                        trans2.Position = Vector3.Lerp(trans2.Position, new Vector3(trans2.Position.X + 35f, trans2.Position.Y, trans2.Position.Z), movement_timer / 0.8f);
                        transpodium2top.Position = Vector3.Lerp(transpodium2top.Position, new Vector3(transpodium2top.Position.X + 20f, transpodium2top.Position.Y, transpodium2top.Position.Z), movement_timer2 / 0.8f);
                        player2trans.Position = Vector3.Lerp(player2trans.Position, new Vector3(player2trans.Position.X + 20f, player2trans.Position.Y - 3f, player2trans.Position.Z), movement_timer2 / 0.8f);
                        p2scoretrans.Position = Vector3.Lerp(p2scoretrans.Position, new Vector3(p2scoretrans.Position.X + 80f, p2scoretrans.Position.Y, p2scoretrans.Position.Z), movement_timer2 / 0.8f);

                        //if (delayFade2 > 2f)
                        //{
                            //NativeLog.Log("BLue LOST");
                            delayFadetimer2 += Time.DeltaTime;
                            if (delayFadetimer2 > 2.0f) delayFadetimer2 = 2.0f;
                            trans2.Position = Vector3.Lerp(trans2.Position, new Vector3(trans2.Position.X, trans2.Position.Y - 50f, trans2.Position.Z), delayFadetimer2 / 0.3f);
                            transpodium2top.Position = Vector3.Lerp(transpodium2top.Position, new Vector3(transpodium2top.Position.X, transpodium2top.Position.Y - 50f, transpodium2top.Position.Z), delayFadetimer2);
                            player2trans.Position = Vector3.Lerp(player2trans.Position, new Vector3(player2trans.Position.X, player2trans.Position.Y - 50f, player2trans.Position.Z), delayFadetimer2);
                            p2scoretrans.Position = Vector3.Lerp(p2scoretrans.Position, new Vector3(p2scoretrans.Position.X, p2scoretrans.Position.Y - 80f, p2scoretrans.Position.Z), delayFadetimer2 / 0.5f);


                        //}
                    }
                    // P2 Wins - turns bigger and moves to the centre
                    else if (p2Wins)
                    {
                        p2_scale_timer += Time.DeltaTime;
                        if (p2_scale_timer > 2.0f) p2_scale_timer = 2.0f;
                        movement_timer2 += Time.DeltaTime * 2.0f;
                        if (movement_timer2 > 0.5f) movement_timer2 = 0.5f;

                        // Enlarging Cat + Podium + Podium Top (Scaling)
                        Vector3 targetpodiumtop = new Vector3(targetScalePodiumWinner.X * podiumToptoBottomAspectRatio, targetScalePodiumWinner.Y * podiumToptoBottomAspectRatio, targetScalePodiumWinner.Z * podiumToptoBottomAspectRatio);
                        trans2.Scale = Vector3.Lerp(trans2.Scale, targetScalePodiumWinner, p2_scale_timer);
                        transpodium2top.Scale = Vector3.Lerp(transpodium2top.Scale, targetpodiumtop, p2_scale_timer);
                        Vector3 targetCat = new Vector3(targetpodiumtop.X * catToPodiumTopAspectRatio, targetpodiumtop.Y * catToPodiumTopAspectRatio, targetpodiumtop.Z * catToPodiumTopAspectRatio);
                        player2trans.Scale = Vector3.Lerp(player2trans.Scale, targetCat, p2_scale_timer);

                        //if ((targetScalePodiumWinner - trans2.Scale).GetMagnitudeSq() < 0.01f)
                        //{
                        //    // Scaling is very close to target, set the scale directly to the target
                        //    trans2.Scale = targetScalePodiumWinner;
                        //    transpodium2top.Scale = targetpodiumtop;
                        //    player2trans.Scale = targetCat;

                        //}

                        // Moving them to the center of the screen as  winners
                        Vector3 targetCatPos = new Vector3(player2trans.Position.X - 15f, transpodium2top.Position.Y + 32f, player2trans.Position.Z);
                        //player1trans.Position = Vector3.Lerp(player1trans.Position, new Vector3(player1trans.Position.X, /*player1trans.Position.Y + (transpodium1top.Position.Y * catToPodiumTopAspectRatio)*/65f, player1trans.Position.Z), 0.03f);
                        //player1trans.Position = new Vector3(player1trans.Position.X, 65f, player1trans.Position.Z);
                        player2trans.Position = Vector3.Lerp(player2trans.Position, targetCatPos, movement_timer2 / 0.8f);
                        transpodium2top.Position = Vector3.Lerp(transpodium2top.Position, new Vector3(transpodium2top.Position.X - 15f, transpodium2top.Position.Y, transpodium2top.Position.Z), movement_timer2 / 0.8f);
                        p2scoretrans.Position = Vector3.Lerp(p2scoretrans.Position, new Vector3(p2scoretrans.Position.X - 80f, player2trans.Position.Y + 60f, p2scoretrans.Position.Z), movement_timer2 / 0.8f);
                        trans2.Position = Vector3.Lerp(trans2.Position, new Vector3(trans2.Position.X - 15f, trans2.Position.Y, trans2.Position.Z), movement_timer2 / 0.8f);

                        if (movement_timer2 >= 0.5f)
                        //if (Vector3.Dist(player1trans.Position, targetCatPos) < 0.1f)
                        {
                            if (p2_jump_start == Vector3.ZERO)
                            {
                                p2_jump_start = player2trans.Position;
                                p2_jump_endpos = new Vector3(p2_jump_start.X, p2_jump_start.Y + 10f, p2_jump_start.Z);
                            }

                            //if (p1_jump_timer < (MathHelper.PI / 2.0f)
                            p2_jump_timer += Time.DeltaTime * 2.0f;
                            if (p2_jump_timer >= MathHelper.PI)
                            {
                                p2_jump_timer = 0.0f;
                            }
                            // jump
                            //NativeLog.Log("jumpppppp");
                            Vector3 target = curMoveTgt();
                            Vector3 jumpPos = Vector3.Lerp(p2_jump_start, p2_jump_endpos, MathHelper.Sin(p2_jump_timer));
                            if (jumpPos.Y < p2_jump_start.Y)
                            {
                                jumpPos.Y = p2_jump_start.Y;
                            }
                            player2trans.Position = jumpPos;
                            float distance = (target - (Vector3)player2trans.Position).GetMagnitude();

                            if (distance <= 0.1f)
                            {
                                dir *= -1;
                            }


                        }
                    }
                    else if (isDraw)
                    {
                        p2_scale_timer += Time.DeltaTime;
                        if (p2_scale_timer > 2.0f) p2_scale_timer = 2.0f;
                        movement_timer2 += Time.DeltaTime * 2.0f;
                        if (movement_timer2 > 0.5f) movement_timer2 = 0.5f;

                        // Enlarging Cat + Podium + Podium Top (Scaling)
                        Vector3 targetpodiumtop = new Vector3(targetScalePodiumWinner.X * podiumToptoBottomAspectRatio, targetScalePodiumWinner.Y * podiumToptoBottomAspectRatio, targetScalePodiumWinner.Z * podiumToptoBottomAspectRatio);
                        trans2.Scale = Vector3.Lerp(trans2.Scale, targetScalePodiumWinner, p2_scale_timer);
                        transpodium2top.Scale = Vector3.Lerp(transpodium2top.Scale, targetpodiumtop, p2_scale_timer);
                        Vector3 targetCat = new Vector3(targetpodiumtop.X * catToPodiumTopAspectRatio, targetpodiumtop.Y * catToPodiumTopAspectRatio, targetpodiumtop.Z * catToPodiumTopAspectRatio);
                        player2trans.Scale = Vector3.Lerp(player2trans.Scale, targetCat, p2_scale_timer);

                        if ((targetScalePodiumWinner - trans2.Scale).GetMagnitudeSq() < 0.01f)
                        {
                            // Scaling is very close to target, set the scale directly to the target
                            trans2.Scale = targetScalePodiumWinner;
                            transpodium2top.Scale = targetpodiumtop;
                            player2trans.Scale = targetCat;

                        }

                        //// Moving them to the center of the screen as  winners
                        Vector3 targetCatPos = new Vector3(player2trans.Position.X + 55f, transpodium2top.Position.Y + 32f, player2trans.Position.Z);
                        ////player1trans.Position = Vector3.Lerp(player1trans.Position, new Vector3(player1trans.Position.X, /*player1trans.Position.Y + (transpodium1top.Position.Y * catToPodiumTopAspectRatio)*/65f, player1trans.Position.Z), 0.03f);
                        ////player1trans.Position = new Vector3(player1trans.Position.X, 65f, player1trans.Position.Z);
                        player2trans.Position = Vector3.Lerp(player2trans.Position, targetCatPos, movement_timer2 / 0.8f);
                        transpodium2top.Position = Vector3.Lerp(transpodium2top.Position, new Vector3(transpodium2top.Position.X + 50f, transpodium2top.Position.Y, transpodium2top.Position.Z), movement_timer2 / 0.8f);
                        p2scoretrans.Position = Vector3.Lerp(p2scoretrans.Position, new Vector3(p2scoretrans.Position.X + 250f, player2trans.Position.Y + 50f, p2scoretrans.Position.Z), movement_timer2 / 0.8f);
                        trans2.Position = Vector3.Lerp(trans2.Position, new Vector3(trans2.Position.X + 50f, trans2.Position.Y, trans2.Position.Z), movement_timer2 / 0.8f);

                        if (movement_timer >= 0.5f)
                        //if (Vector3.Dist(player1trans.Position, targetCatPos) < 0.1f)
                        {
                            if (p2_jump_start == Vector3.ZERO)
                            {
                                p2_jump_start = player2trans.Position;
                                p2_jump_endpos = new Vector3(p2_jump_start.X, p2_jump_start.Y + 10f, p2_jump_start.Z);
                            }

                            //if (p1_jump_timer < (MathHelper.PI / 2.0f)
                            p2_jump_timer += Time.DeltaTime * 2.0f;
                            if (p2_jump_timer >= MathHelper.PI)
                            {
                                p2_jump_timer = 0.0f;
                            }
                            // jump
                            //NativeLog.Log("jumpppppp");
                            Vector3 target = curMoveTgt();
                            Vector3 jumpPos = Vector3.Lerp(p2_jump_start, p2_jump_endpos, MathHelper.Sin(p2_jump_timer));
                            if (jumpPos.Y < p2_jump_start.Y)
                            {
                                jumpPos.Y = p2_jump_start.Y;
                            }
                            player2trans.Position = jumpPos;
                            float distance = (target - (Vector3)player2trans.Position).GetMagnitude();

                            if (distance <= 0.1f)
                            {
                                dir *= -1;
                            }
                        }
                    }
                }
            }

            delay += Time.DeltaTime;
            seconds = (int)(delay % 60);
            if ((seconds > 3) && uigoCredits.Opacity == 0f)
            {
                uigoCredits.Opacity = 1.0f;
            }
            if ((uigoCredits.Opacity == 1.0f) && Input.GetMappingKeyPressed(goCreditsBtn) || GameInputs.IsPlayerGamepadButtonPressed(goCreditsBtn, true) || GameInputs.IsPlayerGamepadButtonPressed(goCreditsBtn, false) || Input.GetKeyPressed(Keys.KEY_M))
            {
                GameData.level1Clear = false;
                GameData.level2Clear = false;
                SceneManager.LoadScene(".\\resources\\scene\\V1_CREDITS.scene");
                if (grandWinner)
                {
                    grandWinner = false;
                }
                //SceneManager.LoadScene(".\\resources\\scene\\V1_MAIN_MENU.scene");
                
            }
        }
        Vector3 curMoveTgt()
        {
            if (dir == 1f)
            {
                if (p1Wins)
                {
                    return player1trans.Position;
                }
                else if (p2Wins)
                {
                    return player2trans.Position;
                }
            }
            else
            {
                if (p1Wins)
                {
                    return p1_jump_endpos;
                }
                else if (p2Wins)
                {
                    return p2_jump_endpos;
                }

            }
            return Vector3.ONE;
        }
    
    }

}