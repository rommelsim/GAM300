using AeonScriptCore;
using System.Collections.Generic;
using System.Configuration;

namespace AeonScripts.Level2
{
    public class RespawnZoneTriggerL2 : AeonBehaviour
    {
        private AudioComponent respawSFX = null;

        private string currentPlayer1CheckPointName = "";
        private string currentPlayer2CheckPointName = "";
        private static Vector3 _respawnPos = new Vector3(-1.8f, 5.5f, 0);
        private static Vector3 _newRespawnPos = Vector3.ZERO;
        private static List<Entity> _respawningEntities = new List<Entity>();
        private bool playerdieaudio = false;

        public Checkpoint checkpointScript = null;

        private Entity player1 = null;
        private Entity player2 = null;
        private Entity pointBox = null;
        private Vector3 pointboxPosition = Vector3.ZERO;
        private BoxWithPoint box;

        private float delay = 1;
        private float currentTimer = 0.0f;

        public bool player1Death = false;
        public bool player2Death = false;

        MoveCharacter player1CheckpointName = null;
        MoveCharacter player2CheckpointName = null;

        //For light manager
        public bool player1respawn2 = false;
        public bool player2respawn2 = false;
        public bool boxrespawn2 = false;
        public override void OnCreate()
        {
            respawSFX = entity.GetComponent<AudioComponent>();
            //checkpoint = Entity.FindEntityByName("Checkpoint2");
            player1 = Entity.FindEntityByName("Player 1");
            player2 = Entity.FindEntityByName("player2");
            player1CheckpointName = player1.GetScript<MoveCharacter>();
            player2CheckpointName = player2.GetScript<MoveCharacter>();
            pointBox = Entity.FindEntityByName("Box");
            if (pointBox != null)
            {
                pointboxPosition = pointBox.GetComponent<Transform>().Position;
            }
            _respawningEntities.Clear();

        }
        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);

            // when the player and the box drops, minus the player's score

            if (otherEnt != null)
            {
                MoveCharacter movey = otherEnt.GetScript<MoveCharacter>();

                if (movey != null)
                {
                    if (movey.isPlayerOne)
                    {
                        GameData.P1_Score -= 2;
                        if (GameData.P1_Score < 0)
                            GameData.P1_Score = 0;
                        playerdieaudio = true;
                        player1Death = true;
                    }
                    else
                    {
                        GameData.P2_Score -= 2;
                        if (GameData.P2_Score < 0)
                            GameData.P2_Score = 0;
                        playerdieaudio = true;
                        player2Death = true;
                    }
                    movey.DropPickable(true);
                    _respawningEntities.Add(otherEnt);
                }

                if (otherEnt.GetScript<CollectPowerUp>() != null)
                {
                    if (otherEnt.GetScript<CollectPowerUp>().isPlacedDownbanana)
                    {
                        Entity.Destroy(otherEnt);
                    }
                }

                if (otherEnt.GetScript<RangeBallCollide>() != null)
                {
                    otherEnt.GetScript<RangeBallCollide>().canReset = true;
                }

                if (otherEnt.GetScript<BoxWithPoint>() != null)
                {
                    _respawningEntities.Add(otherEnt);
                }
            }

        }

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);

            // when the player and the box drops, minus the player's score

            if (otherEnt != null)
            {

                bool foundEnt = false;
                foreach (Entity ent in _respawningEntities)
                {
                    if (ent.Id == triggerEntityID)
                        foundEnt = true;
                }

                if (!foundEnt)
                {
                    MoveCharacter movey = otherEnt.GetScript<MoveCharacter>();
                    if (movey != null)
                    {
                        if (movey.isPlayerOne)
                        {
                            GameData.P1_Score -= 2;
                            if (GameData.P1_Score < 0)
                                GameData.P1_Score = 0;
                            playerdieaudio = true;
                        }
                        else
                        {
                            GameData.P2_Score -= 2;
                            if (GameData.P2_Score < 0)
                                GameData.P2_Score = 0;
                            playerdieaudio = true;
                        }
                        movey.DropPickable(true);
                        _respawningEntities.Add(otherEnt);
                    }

                    if (otherEnt.GetScript<CollectPowerUp>() != null)
                    {
                        if (otherEnt.GetScript<CollectPowerUp>().isPlacedDownbanana)
                        {
                            Entity.Destroy(otherEnt);
                        }
                    }

                    if (otherEnt.GetScript<RangeBallCollide>() != null)
                    {
                        otherEnt.GetScript<RangeBallCollide>().canReset = true;
                    }

                    if (otherEnt.GetScript<BoxWithPoint>() != null)
                    {
                        _respawningEntities.Add(otherEnt);
                    }

                }
            }
        }

        public override void Update()
        {
            if (playerdieaudio)
            {
                if (respawSFX != null)
                {
                    respawSFX.SoundFilepath = ".\\resources\\audios\\sfx_respawn_wehhh_sound.wav";
                    respawSFX.Volume = 0.5f;
                    respawSFX.PlaySound();
                    playerdieaudio = false;
                }
            }
            //Checkpoint checkPointIndex = checkpoint.GetScript<Checkpoint>();
            currentPlayer1CheckPointName = player1CheckpointName.currentCheckpoint;
            currentPlayer2CheckPointName = player2CheckpointName.currentCheckpoint;
            Entity newPlayer1Checkpoint = Entity.FindEntityByName(currentPlayer1CheckPointName);
            Entity newPlayer2Checkpoint = Entity.FindEntityByName(currentPlayer2CheckPointName);

            if (newPlayer1Checkpoint != null && newPlayer2Checkpoint != null)
            {
                // _newRespawnPos = new Vector3(newCheckpoint.GetComponent<Transform>().Position.X, newCheckpoint.GetComponent<Transform>().Position.Y + 20.0f, newCheckpoint.GetComponent<Transform>().Position.Z);
                //NativeLog.Log("Current: {0}, Position: {1},{2},{3}", currentCheckPointName, newCheckpoint.GetComponent<Transform>().Position.X, newCheckpoint.GetComponent<Transform>().Position.Y, newCheckpoint.GetComponent<Transform>().Position.Z);


                // delay for X amt of sec
                if (_respawningEntities.Count > 0)
                {
                    currentTimer += Time.DeltaTime;
                    if (currentTimer >= delay)
                    {

                        for (int i = _respawningEntities.Count - 1; i >= 0; i--)
                        {
                            Entity respawnEnt = _respawningEntities[i];
                            NativeLog.LogError("Respawned {0}", respawnEnt.Name);


                            if (respawnEnt.Name == "Player 1")
                            {

                                _newRespawnPos = new Vector3(newPlayer1Checkpoint.GetComponent<Transform>().Position.X, newPlayer1Checkpoint.GetComponent<Transform>().Position.Y + 20.0f, newPlayer1Checkpoint.GetComponent<Transform>().Position.Z);
                                NativeLog.LogError("Respawned Player 1: {0},{1},{2}", newPlayer1Checkpoint.GetComponent<Transform>().Position.X, newPlayer1Checkpoint.GetComponent<Transform>().Position.Y + 10.0f, newPlayer1Checkpoint.GetComponent<Transform>().Position.Z);
                                player1respawn2 = true;
                            }
                            else if (respawnEnt.Name == "player2")
                            {
                                _newRespawnPos = new Vector3(newPlayer2Checkpoint.GetComponent<Transform>().Position.X, newPlayer2Checkpoint.GetComponent<Transform>().Position.Y + 20.0f, newPlayer2Checkpoint.GetComponent<Transform>().Position.Z);
                                NativeLog.LogError("Respawned Player 2: {0},{1},{2}", newPlayer2Checkpoint.GetComponent<Transform>().Position.X, newPlayer2Checkpoint.GetComponent<Transform>().Position.Y + 10.0f, newPlayer2Checkpoint.GetComponent<Transform>().Position.Z);
                                player2respawn2 = true;
                            }
                            else
                            {
                                //respawn BOX
                                // Respawn middle of player
                                //Vector3 mid = (player1.GetComponent<Transform>().Position + player2.GetComponent<Transform>().Position)/2.0f;
                                //_newRespawnPos = mid;

                                //Respawn to specific platform
                                if (currentPlayer1CheckPointName != "Checkpoint1.Red" || currentPlayer2CheckPointName != "Checkpoint1.Blue")
                                {
                                    Vector3 newPos = Entity.FindEntityByName(currentPlayer1CheckPointName + ".Box").GetComponent<Transform>().Position;
                                    //_newRespawnPos = new Vector3(newPos.X, newPos.Y + 20.0f, newPos.Z);
                                    _newRespawnPos = new Vector3(newPos.X, newPos.Y, newPos.Z);
                                    boxrespawn2= true;
                                    /*if (currentPlayer1CheckPointName == "Red.Checkpoint3")
                                    {
                                        Vector3 newPos3 = Entity.FindEntityByName("Checkpoint.Box3").GetComponent<Transform>().Position;
                                        _newRespawnPos = new Vector3(newPos3.X, newPos3.Y + 20.0f, newPos3.Z);
                                    }

                                    if (currentPlayer2CheckPointName == "Blue.Checkpoint3")
                                    {
                                        Vector3 newPos3 = Entity.FindEntityByName("Checkpoint.Box3").GetComponent<Transform>().Position;
                                        _newRespawnPos = new Vector3(newPos3.X, newPos3.Y + 20.0f, newPos3.Z);
                                    }*/
                                }
                                else
                                {
                                    //original position
                                    _newRespawnPos = pointboxPosition;
                                    boxrespawn2 = true;

                                }
                            }


                            respawnEnt.GetComponent<Transform>().Position = _newRespawnPos;
                            respawnEnt.GetComponent<RigidBody>().Velocity = Vector3.ZERO;
                            player1Death = false;
                            player2Death = false;

                            if (respawSFX != null)
                            {
                                if (_respawningEntities[i].GetScript<MoveCharacter>() != null)
                                {
                                    //respawSFX.SoundFilepath = ".\\resources\\audios\\sfx_respawn_wehhh_sound.wav";
                                    //respawSFX.PlaySound();
                                    //NativeLog.LogWarn("has it enter here for respawn");
                                }
                                else
                                {
                                    respawSFX.SoundFilepath = ".\\resources\\audios\\Player Touch Enemy.wav";
                                    respawSFX.Volume = 3.0f;
                                    respawSFX.PlaySound();
                                }
                            }
                        }
                        _respawningEntities.Clear();
                    }
                }
                else
                {
                    currentTimer = 0.0f;
                }

            }
        }
    }
}