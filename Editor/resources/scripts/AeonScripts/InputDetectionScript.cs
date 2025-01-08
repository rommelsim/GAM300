using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class InputDetectionScript : AeonBehaviour
    {
        private bool hasPlayerOneSet = false;
        private bool hasPlayerTwoSet = false;
        public Transform trans1 = null;
        public Transform trans2 = null;
        private bool trigger = false;
        public static string Map_Join_Game = "Map_Join_Game";
        //public UI playerJoinButton = null;
        //public UI playerJoinButton2 = null;
        public int game_pad_one = 0;
        public int game_pad_two = -1;
        private bool podiumMoving = false;
        private bool podiumMoving2 = false;
        private Vector3 targetposition = Vector3.ZERO;
        private Vector3 targetposition2 = Vector3.ZERO;
        public Entity podium1 = null;
        public Entity podium2 = null;
        public Entity player1JoinButton = null;
        public Entity player2JoinButton = null;
        public bool firstPressPlayer1 = false;
        public bool firstPressPlayer2 = false;
        public UI btn1 = null;
        public UI btn2 = null;
        public Entity player1 = null;
        public Entity player2 = null;
        public Transform player1trans = null;
        public Transform player2trans = null;
        private Vector3 targetpositionCat1 = Vector3.ZERO;
        private Vector3 targetpositionCat2 = Vector3.ZERO;
        public bool playerOnefirst = true;
        public bool playerTwoNext = false;
        public bool playerOneReady = false;
        public bool playerTwoReady = false;
        public Entity platform1_top = null;
        public Transform trans_platform1_top = null;
        private bool loadStart = false;
        private Vector3 targetposition_top1 = Vector3.ZERO;
        private Vector3 targetposition_top2 = Vector3.ZERO;
        public Entity platform2_top = null;
        public Transform trans_platform2_top = null;
        public DateTime lastTime = DateTime.MinValue;

        private Lighting redLight = null;
        private Lighting blueLight = null;

        float delayAfterReadyRed = 0.5f;
        float delayAfterReadyBlue = 0.5f;
        bool readyPressedRed = false;
        bool readyPressedBlue = false;

        // AudioComponent Variable Declaration
        private AudioComponent readyButtonSFX = null;

        public override void OnCreate()
        {
            podium1 = Entity.FindEntityByName("Platform1");
            podium2 = Entity.FindEntityByName("Platform2");
            if (podium1 != null) trans1 = podium1.GetComponent<Transform>();
            if (podium2 != null) trans2 = podium2.GetComponent<Transform>();
            player1JoinButton = Entity.FindEntityByName("Player1JoinButton");
            player2JoinButton = Entity.FindEntityByName("Player2Join_Button");
            if (player1JoinButton != null) btn1 = player1JoinButton.GetComponent<UI>();
            if (player2JoinButton != null) btn2 = player2JoinButton.GetComponent<UI>();
            player1 = Entity.FindEntityByName("Cat1");
            player2 = Entity.FindEntityByName("Cat2");
            if (player1 != null) player1trans = player1.GetComponent<Transform>();
            if (player2 != null) player2trans = player2.GetComponent<Transform>();
            Input.RegisterMapping(Map_Join_Game, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_X);
            platform1_top = Entity.FindEntityByName("Platform1_Top");
            if (platform1_top != null) trans_platform1_top = platform1_top.GetComponent<Transform>();
            platform2_top = Entity.FindEntityByName("Platform2_Top");
            if (platform2_top != null) trans_platform2_top = platform2_top.GetComponent<Transform>();
            // Chuck Audio Data From Entity Into Variable
            readyButtonSFX = entity.GetComponent<AudioComponent>();

            redLight = Entity.FindEntityByName("RedSpotLight").GetComponent<Lighting>();
            blueLight = Entity.FindEntityByName("BlueSpotLight").GetComponent<Lighting>();

            GameState.IsPaused = false;
            GameInputs.PlayerOne_GamepadId = GameInputs.PlayerTwo_GamepadId = -1;
        }

        public override void Update()
        {

            bool isgame_padConnected = Input.GetConnectedGamepadIds().Length > 0;

            if (isgame_padConnected)
            {
                game_pad_one = Input.GetConnectedGamepadIds()[0];
                //NativeLog.Log($"GamePad 1 ID: {game_pad_one}");
                if (Input.GetConnectedGamepadIds().Length > 1) game_pad_two = Input.GetConnectedGamepadIds()[1];
                //NativeLog.Log($"GamePad 2 ID: {game_pad_two}");
            }

            bool joinButtonPressed_GamePadOne = Input.GetMappingGamePadButtonPressed(Map_Join_Game, game_pad_one);
            bool joinButtonPressed_GamePadTwo = Input.GetMappingGamePadButtonPressed(Map_Join_Game, game_pad_two);

            if (!hasPlayerOneSet)
            {
                //game_pad_one = Input.GetConnectedGamepadIds()[0];
                if (Input.GetKeyPressed(Keys.KEY_K) || joinButtonPressed_GamePadOne || joinButtonPressed_GamePadTwo)
                {
                    //GameData.PlayerOneInput = GameData.InputType.Controller1;
                    hasPlayerOneSet = true;
                    redLight.Intensity = 200;
                    player1.GetComponent<RigidBody>().UseGravity = true;

                    if (joinButtonPressed_GamePadOne)
                    {
                        GameInputs.PlayerOne_GamepadId = game_pad_one;
                        GameInputs.PlayerOneInput = GameInputs.InputType.Controller;
                    }
                    else if (joinButtonPressed_GamePadTwo)
                    {
                        GameInputs.PlayerOne_GamepadId = game_pad_two;
                        GameInputs.PlayerOneInput = GameInputs.InputType.Controller;
                    }
                    else if (Input.GetKeyPressed(Keys.KEY_K))
                    {
                        GameInputs.PlayerOneInput = GameInputs.InputType.Keyboard;
                    }

                    if (hasPlayerOneSet)
                    {

                        if (readyButtonSFX != null)
                        {
                            readyButtonSFX.PlaySound();
                            NativeLog.Log("Player 1 Joined!");
                        }
                        targetposition = new Vector3(trans1.Position.X, 38.0f, trans1.Position.Z);
                        targetpositionCat1 = new Vector3(player1trans.Position.X, 49.0f, player1trans.Position.Z);
                        targetposition_top1 = new Vector3(trans_platform1_top.Position.X, 38f, trans_platform1_top.Position.Z);
                        podiumMoving = true;
                        btn1.Texture = ".\\resources\\textures\\Ready LeftSide (Controller).dds";
                        firstPressPlayer1 = true;
                    }

                }

            }
            else if ((Input.GetKeyPressed(Keys.KEY_K) || GameInputs.IsPlayerGamepadButtonPressed(Map_Join_Game, true)) && firstPressPlayer1 && hasPlayerOneSet && player1.GetScript<LobbyCatTouchingPodiumCheck>().touchingPodium)
            {
                if (readyButtonSFX != null)
                {
                    readyButtonSFX.PlaySound();
                    NativeLog.Log("Player 1 Ready!");
                }
                Vector3 impulseForce = new Vector3(0, 5, 0);
                player1.GetComponent<RigidBody>().AddImpulseForce(ref impulseForce);
                readyPressedRed = true;
                btn1.Texture = ".\\resources\\textures\\Ready LeftSide (RedPlayer).dds";
                playerOneReady = true;
            }
            else if (!hasPlayerTwoSet && hasPlayerOneSet)
            {
                //if(Input.GetKeyPressed(Keys.KEY_K) || Input.GetMappingKeyPressed(Map_Join_Game))
                playerTwoNext = true;
                if (Input.GetKeyPressed(Keys.KEY_A)
                    || (GameInputs.PlayerOneInput == GameInputs.InputType.Keyboard && (joinButtonPressed_GamePadOne || joinButtonPressed_GamePadTwo))
                    || (GameInputs.PlayerOne_GamepadId == game_pad_one && joinButtonPressed_GamePadTwo)
                    || (GameInputs.PlayerOne_GamepadId == game_pad_two && joinButtonPressed_GamePadOne))
                {
                    hasPlayerTwoSet = true;
                    blueLight.Intensity = 200;
                    player2.GetComponent<RigidBody>().UseGravity = true;

                    if (GameInputs.PlayerOne_GamepadId == game_pad_one && joinButtonPressed_GamePadTwo)
                    {
                        GameInputs.PlayerTwo_GamepadId = game_pad_two;
                        GameInputs.PlayerTwoInput = GameInputs.InputType.Controller;
                    }
                    else if (GameInputs.PlayerOne_GamepadId == game_pad_two && joinButtonPressed_GamePadOne)
                    {
                        GameInputs.PlayerTwo_GamepadId = game_pad_one;
                        GameInputs.PlayerTwoInput = GameInputs.InputType.Controller;
                    }
                    else if (GameInputs.PlayerOneInput == GameInputs.InputType.Keyboard)
                    {
                        if (joinButtonPressed_GamePadOne)
                        {
                            GameInputs.PlayerTwo_GamepadId = game_pad_one;
                            GameInputs.PlayerTwoInput = GameInputs.InputType.Controller;
                        }
                        else if (joinButtonPressed_GamePadTwo)
                        {
                            GameInputs.PlayerTwo_GamepadId = game_pad_two;
                            GameInputs.PlayerTwoInput = GameInputs.InputType.Controller;
                        }
                        else if (Input.GetKeyPressed(Keys.KEY_A))
                        {
                            GameInputs.PlayerTwoInput = GameInputs.InputType.Keyboard;
                        }
                    }

                    if (hasPlayerTwoSet)
                    {

                        if (readyButtonSFX != null)
                        {
                            readyButtonSFX.PlaySound();
                            NativeLog.Log("Player 2 Joined!");
                        }

                        
                        targetposition2 = new Vector3(trans2.Position.X, 38.0f, trans2.Position.Z);
                        targetpositionCat2 = new Vector3(player2trans.Position.X, 49.0f, player2trans.Position.Z);
                        targetposition_top2 = new Vector3(trans_platform2_top.Position.X, 38f, trans_platform2_top.Position.Z);
                        podiumMoving2 = true;
                        btn2.Texture = ".\\resources\\textures\\Ready RightSide (Controller).dds";
                        firstPressPlayer2 = true;
                    }
                }

            }
            else if ((Input.GetKeyPressed(Keys.KEY_A) || GameInputs.IsPlayerGamepadButtonPressed(Map_Join_Game, false)) && firstPressPlayer2 && hasPlayerTwoSet && player2.GetScript<LobbyCatTouchingPodiumCheck>().touchingPodium)
            {
                if (readyButtonSFX != null)
                {
                    readyButtonSFX.PlaySound();
                    NativeLog.Log("Player 2 Ready!");
                }
                Vector3 impulseForce = new Vector3(0, 5, 0);
                player2.GetComponent<RigidBody>().AddImpulseForce(ref impulseForce);
                readyPressedBlue = true;
                btn2.Texture = ".\\resources\\textures\\Ready RightSide (BluePlayer).dds";
                playerTwoReady = true;
            }
            if (podiumMoving)
            {
                trans1.Position = Vector3.Lerp(trans1.Position, targetposition, 0.008f);
                //player1trans.Position = Vector3.Lerp(player1trans.Position, targetpositionCat1, 0.001f);
                trans_platform1_top.Position = Vector3.Lerp(trans_platform1_top.Position, targetposition_top1, 0.01f);
                trans1.Rotation += new Vector3(0, 40, 0) * Time.DeltaTime;
                if (Vector3.Dist(trans1.Position, targetposition) < 0.5f)
                {
                    podiumMoving = false;
                    trans_platform1_top.Position = new Vector3(trans_platform1_top.Position.X, 37.6f, trans_platform1_top.Position.Z);
                }
            }

            if (podiumMoving2)
            {
                trans2.Position = Vector3.Lerp(trans2.Position, targetposition2, 0.008f);
                //player2trans.Position = Vector3.Lerp(player2trans.Position, targetpositionCat2, 0.01f);
                trans_platform2_top.Position = Vector3.Lerp(trans_platform2_top.Position, targetposition_top2, 0.01f);
                trans2.Rotation += new Vector3(0, -40, 0) * Time.DeltaTime;
                if (Vector3.Dist(trans2.Position, targetposition2) < 0.5f)
                {
                    podiumMoving2 = false;
                    trans_platform2_top.Position = new Vector3(trans_platform2_top.Position.X, 37.6f, trans_platform2_top.Position.Z);
                }
            }

            if (playerOneReady && playerTwoReady && !loadStart)
            {
                lastTime = DateTime.Now;
                loadStart = true;
            }

            if (loadStart && (DateTime.Now - lastTime).TotalSeconds >= 1.5f)
            {
                if (BgmMainMenu.menuBGM != null)
                    BgmMainMenu.menuBGM.PauseSound();
                SceneManager.LoadScene(".\\resources\\scene\\V1_GAME_MODE.scene");

            }

            if (readyPressedRed)
            {
                delayAfterReadyRed -= Time.DeltaTime;
                if (delayAfterReadyRed <= 0.2f)
                {
                    player1JoinButton.GetComponent<Transform>().Scale = Vector3.Lerp(player1JoinButton.GetComponent<Transform>().Scale, new Vector3(0, 320, 0), 0.008f);
                    player1JoinButton.GetComponent<Transform>().Position = Vector3.Lerp(player1JoinButton.GetComponent<Transform>().Position, new Vector3(-1320, 480, 0), 0.008f);
                }
            }
            
            if (readyPressedBlue)
            {
                delayAfterReadyBlue -= Time.DeltaTime;
                if (delayAfterReadyBlue <= 0.2f)
                {
                    player2JoinButton.GetComponent<Transform>().Scale = Vector3.Lerp(player2JoinButton.GetComponent<Transform>().Scale, new Vector3(0, 320, 0), 0.008f);
                    player2JoinButton.GetComponent<Transform>().Position = Vector3.Lerp(player2JoinButton.GetComponent<Transform>().Position, new Vector3(1320, 480, 0), 0.008f);
                }
            }
        }
    }
}


//using AeonScriptCore;
//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;

//namespace AeonScripts
//{
//    public class InputDetectionScript : AeonBehaviour
//    {
//        private bool hasPlayerOneSet = false;
//        private bool hasPlayerTwoSet = false;
//        public Transform trans1 = null;
//        public Transform trans2 = null;
//        private bool trigger = false;
//        public static string Map_Join_Game = "Map_Join_Game";
//        //public UI playerJoinButton = null;
//        //public UI playerJoinButton2 = null;
//        public int game_pad_one = 0;
//        public int game_pad_two = 0;
//        private bool podiumMoving = false;
//        private bool podiumMoving2 = false;
//        private Vector3 targetposition = Vector3.ZERO;
//        private Vector3 targetposition2 = Vector3.ZERO;
//        public Entity podium1 = null;
//        public Entity podium2 = null;
//        public Entity player1JoinButton = null;
//        public Entity player2JoinButton = null;
//        public bool firstPressPlayer1 = false;
//        public bool firstPressPlayer2 = false;
//        public UI btn1 = null;
//        public UI btn2 = null;
//        public Entity player1 = null;
//        public Entity player2 = null;
//        public Transform player1trans = null;
//        public Transform player2trans = null;
//        private Vector3 targetpositionCat1 = Vector3.ZERO;
//        private Vector3 targetpositionCat2 = Vector3.ZERO;
//        public bool playerOneReady = false;
//        public bool playerTwoReady = false;
//        public bool loadStart = false;

//        public DateTime lastTime = DateTime.MinValue;

//        public Entity entCam = null;
//        public Camera entCamCam = null;

//        // AudioComponent Variable Declaration
//        private AudioComponent readyButtonSFX = null;


//        public override void OnCreate()
//        {
//            podium1 = Entity.FindEntityByName("Platform1");
//            podium2 = Entity.FindEntityByName("Platform2");
//            if (podium1 != null) trans1 = podium1.GetComponent<Transform>();
//            if (podium2 != null) trans2 = podium2.GetComponent<Transform>();
//            player1JoinButton = Entity.FindEntityByName("Player1JoinButton");
//            player2JoinButton = Entity.FindEntityByName("Player2Join_Button");
//            if (player1JoinButton != null) btn1 = player1JoinButton.GetComponent<UI>();
//            if (player2JoinButton != null) btn2 = player2JoinButton.GetComponent<UI>();
//            player1 = Entity.FindEntityByName("Cat1");
//            player2 = Entity.FindEntityByName("Cat2");
//            if (player1 != null) player1trans = player1.GetComponent<Transform>();
//            if (player2 != null) player2trans = player2.GetComponent<Transform>();
//            entCam = Entity.FindEntityByName("camera_placeholder");
//            if (entCam != null) entCamCam = entCam.GetComponent<Camera>();

//            Input.RegisterMapping(Map_Join_Game, Input.KBMInputType.Keyboard, Keys.KEY_K, Input.JoystickInput.Button_A);

//            Vector3 camPos = entCamCam.Position;
//            camPos = new Vector3(20.0f, 53.0f, 170.0f);
//            entCamCam.Position = camPos;
//            entCamCam.Yaw = 0f;

//            // Chuck Audio Data From Entity Into Variable
//            readyButtonSFX = entity.GetComponent<AudioComponent>();

//            GameState.IsPaused = false;
//        }

//        public override void Update()
//        {

//            bool isgame_padConnected = Input.GetConnectedGamepadIds().Length > 0;

//            if (isgame_padConnected)
//            {
//                game_pad_one = Input.GetConnectedGamepadIds()[0];
//                //NativeLog.Log($"GamePad 1 ID: {game_pad_one}");
//                //  if (Input.GetConnectedGamepadIds().Length > 1) game_pad_two = Input.GetConnectedGamepadIds()[1];
//                //NativeLog.Log($"GamePad 2 ID: {game_pad_two}");
//            }

//            bool joinButtonPressed_GamePadOne = Input.GetMappingGamePadButtonPressed(Map_Join_Game, game_pad_one);
//            //bool joinButtonPressed_GamePadTwo = Input.GetMappingGamePadButtonPressed(Map_Join_Game, game_pad_two);

//            if (!hasPlayerOneSet)
//            {
//                //game_pad_one = Input.GetConnectedGamepadIds()[0];

//                // press K or use gamepad
//                if (Input.GetKeyPressed(Keys.KEY_K))
//                {
//                    GameData.PlayerOneInput = GameData.InputType.Keyboard;
//                    hasPlayerOneSet = true;
//                }
//                else if (joinButtonPressed_GamePadOne)
//                {
//                    GameData.PlayerOneInput = GameData.InputType.Controller;
//                    GameData.PlayerOne_GamepadId = game_pad_one;
//                    hasPlayerOneSet = true;
//                }

//                //hasPlayerOneSet = true;

//                /* if (joinButtonPressed_GamePadOne)
//                 {
//                     GameData.PlayerOne_GamepadId = game_pad_one;
//                 }*/
//                /*  else if (joinButtonPressed_GamePadTwo)
//                  {
//                      GameData.PlayerOne_GamepadId = game_pad_two;
//                  }*/

//                if (hasPlayerOneSet)
//                {

//                    if (readyButtonSFX != null)
//                    {
//                        readyButtonSFX.PlaySound();
//                        NativeLog.Log("Player 1 Joined!");
//                    }

//                    targetposition = new Vector3(trans1.Position.X, 35.0f, trans1.Position.Z);
//                    targetpositionCat1 = new Vector3(player1trans.Position.X, 50.0f, player1trans.Position.Z);
//                    podiumMoving = true;
//                    btn1.Texture = ".\\resources\\textures\\Ready LeftSide (Controller).dds";
//                    firstPressPlayer1 = true;

//                }

//            }
//            else if (GameData.PlayerOneInput == GameData.InputType.Keyboard && Input.GetKeyPressed(Keys.KEY_K) || ((GameData.PlayerOne_GamepadId == game_pad_one && joinButtonPressed_GamePadOne)) && firstPressPlayer1 && hasPlayerOneSet)
//            {
//                if (readyButtonSFX != null)
//                {
//                    readyButtonSFX.PlaySound();
//                    NativeLog.Log("Player 1 Ready!");
//                }
//                playerOneReady = true;
//                btn1.Texture = ".\\resources\\textures\\Ready LeftSide (RedPlayer).dds";
//            }
//            else if (!hasPlayerTwoSet && hasPlayerOneSet)
//            {

//                if (GameData.PlayerOneInput == GameData.InputType.Keyboard && joinButtonPressed_GamePadOne)
//                {
//                    GameData.PlayerTwoInput = GameData.InputType.Controller;
//                    GameData.PlayerTwo_GamepadId = game_pad_one;
//                    hasPlayerTwoSet = true;
//                }
//                else if (GameData.PlayerOneInput == GameData.InputType.Controller && Input.GetKeyPressed(Keys.KEY_K))
//                {
//                    GameData.PlayerTwoInput = GameData.InputType.Keyboard;
//                    hasPlayerTwoSet = true;
//                }

//                if (hasPlayerTwoSet)
//                {

//                    if (readyButtonSFX != null)
//                    {
//                        readyButtonSFX.PlaySound();
//                        NativeLog.Log("Player 2 Joined!");
//                    }


//                    targetposition2 = new Vector3(trans2.Position.X, 35.0f, trans2.Position.Z);
//                    targetpositionCat2 = new Vector3(player2trans.Position.X, 50.0f, player2trans.Position.Z);
//                    podiumMoving2 = true;
//                    btn2.Texture = ".\\resources\\textures\\Ready RightSide (Controller).dds";
//                    firstPressPlayer2 = true;
//                }

//            }
//            else if (Input.GetKeyPressed(Keys.KEY_K) && GameData.PlayerTwoInput == GameData.InputType.Keyboard || ((GameData.PlayerTwo_GamepadId == game_pad_one && joinButtonPressed_GamePadOne) && firstPressPlayer2 && hasPlayerTwoSet))
//            {
//                if (readyButtonSFX != null)
//                {
//                    readyButtonSFX.PlaySound();
//                    NativeLog.Log("Player 2 Ready!");
//                }
//                btn2.Texture = ".\\resources\\textures\\Ready RightSide (BluePlayer).dds";
//                playerTwoReady = true;
//            }
//            if (podiumMoving)
//            {
//                trans1.Position = Vector3.Lerp(trans1.Position, targetposition, 0.01f);
//                player1trans.Position = Vector3.Lerp(player1trans.Position, targetpositionCat1, 0.01f);
//                if (Vector3.Dist(trans1.Position, targetposition) < 0.1f)
//                {
//                    podiumMoving = false;
//                }
//            }

//            if (podiumMoving2)
//            {
//                trans2.Position = Vector3.Lerp(trans2.Position, targetposition2, 0.01f);
//                player2trans.Position = Vector3.Lerp(player2trans.Position, targetpositionCat2, 0.01f);
//                if (Vector3.Dist(trans2.Position, targetposition2) < 0.01f)
//                {
//                    podiumMoving2 = false;
//                }
//            }

//            if (playerOneReady && playerTwoReady && !loadStart)
//            {
//                lastTime = DateTime.Now;
//                loadStart = true;
//            }

//            if (loadStart && (DateTime.Now - lastTime).TotalSeconds >= 1.5f)
//            {

//                SceneManager.LoadScene(".\\resources\\scene\\V1_GAME_MODE.scene");

//            }
//        }
//    }
//}