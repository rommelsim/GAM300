using AeonScriptCore;
using System.Collections.Generic;
using System.Configuration;

namespace AeonScripts
{
    public class RespawnZoneTrigger : AeonBehaviour
    {
        private AudioComponent respawSFX = null;


        private Entity checkpoint = null;
        private string currentCheckPointName = "";
        private static Vector3 _respawnPos = new Vector3(-1.8f, 5.5f, 0);
        private static Vector3 _newRespawnPos = Vector3.ZERO;
        //public  Vector3 _newRespawnPos = Vector3.ZERO;
        private static List<Entity> _respawningEntities = new List<Entity>();
        private bool playerdieaudio = false;

        public Checkpoint checkpointScript = null;

        private Entity player1 = null;
        private Entity player2 = null;
        private BoxWithPoint box;

        private float delay = 1;
        private float currentTimer = 0.0f;

        public bool player1Death = false;
        public bool player2Death = false;

        MoveCharacter moveChara = null;

        //For light manager
        public bool player1respawn =false;
        public bool player2respawn = false;
        public bool boxrespawn = false;


        public override void OnCreate()
        {
            respawSFX = entity.GetComponent<AudioComponent>();
            checkpoint = Entity.FindEntityByName("Checkpoint2");
            player1 = Entity.FindEntityByName("Player 1");
            moveChara =player1.GetScript<MoveCharacter>();
            _respawningEntities.Clear();

        }
        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);

            // when the player and the box drops, minus the player's score

            if (otherEnt != null && otherEnt.GetScript<CollectPowerUp>() == null)
            {
                NativeLog.Log("Abyss's trigger enter detected {0}", otherEnt.Name);
                MoveCharacter movey = otherEnt.GetScript<MoveCharacter>();

                if (movey != null)
                {
                    if (movey.isPlayerOne)
                    {
                        GameData.P1_Score -= 2;
                        if (GameData.P1_Score < 0)
                            GameData.P1_Score = 0;
                        NativeLog.LogError("Did it enter here");
                        playerdieaudio = true;
                        player1Death = true;
                    }
                    else
                    {
                        GameData.P2_Score -= 2;
                        if (GameData.P2_Score < 0)
                            GameData.P2_Score = 0;
                        NativeLog.LogError("or maybe here");
                        playerdieaudio = true;
                        player2Death=true;
                    }
                    movey.DropPickable(true);
                    NativeLog.LogError("Please be here");

                    _respawningEntities.Add(otherEnt);
                }

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

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);

            // when the player and the box drops, minus the player's score

            if (otherEnt != null)
            {
                NativeLog.Log("Abyss's trigger exit detected {0}", otherEnt.Name);

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
                            NativeLog.LogError("Did it enter here");
                            playerdieaudio = true;
                        }
                        else
                        {
                            GameData.P2_Score -= 2;
                            if (GameData.P2_Score < 0)
                                GameData.P2_Score = 0;
                            NativeLog.LogError("or maybe here");
                            playerdieaudio = true;
                        }
                        movey.DropPickable(true);
                        NativeLog.LogError("Please be here");
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
            if (GameState.IsPaused) return;
           
            Checkpoint checkPointIndex = checkpoint.GetScript<Checkpoint>();
            currentCheckPointName = checkPointIndex.GetCheckpointName();
            Entity newCheckpoint = Entity.FindEntityByName(currentCheckPointName);
            if (newCheckpoint != null)
            {
                if (_respawningEntities.Count > 0)
                {
                    currentTimer += Time.DeltaTime;
                    if (currentTimer >= delay)
                    {

                        for (int i = _respawningEntities.Count - 1; i >= 0; i--)
                        {
                            Entity respawnEnt = _respawningEntities[i];
                            NativeLog.LogError("Respawned {0}", respawnEnt.Name);

                            if (currentCheckPointName == "Checkpoint1")
                            {
                                if (respawnEnt.Name == "Player 1")
                                {

                                    Entity checkpoint1red = Entity.FindEntityByName("Red.Checkpoint1");
                                    _newRespawnPos = new Vector3(checkpoint1red.GetComponent<Transform>().Position.X, checkpoint1red.GetComponent<Transform>().Position.Y + 20.0f, checkpoint1red.GetComponent<Transform>().Position.Z);
                                    player1respawn = true;
                                }
                                else if (respawnEnt.Name == "player2")
                                {
                                    Entity checkpoint1blue = Entity.FindEntityByName("Blue.Checkpoint1");
                                    _newRespawnPos = new Vector3(checkpoint1blue.GetComponent<Transform>().Position.X, checkpoint1blue.GetComponent<Transform>().Position.Y + 20.0f, checkpoint1blue.GetComponent<Transform>().Position.Z);
                                    player2respawn= true;
                                }
                                else
                                {
                                    _newRespawnPos = new Vector3(newCheckpoint.GetComponent<Transform>().Position.X, newCheckpoint.GetComponent<Transform>().Position.Y + 20.0f, newCheckpoint.GetComponent<Transform>().Position.Z);
                                    boxrespawn = true;
                                }
                            }
                            else if (currentCheckPointName == "Checkpoint2") {

                                if (respawnEnt.Name == "Player 1")
                                {

                                    Entity checkpoint2red = Entity.FindEntityByName("Checkpoint2.Red");
                                    _newRespawnPos = new Vector3(checkpoint2red.GetComponent<Transform>().Position.X, checkpoint2red.GetComponent<Transform>().Position.Y + 20.0f, checkpoint2red.GetComponent<Transform>().Position.Z);
                                    player1respawn = true;

                                }
                                else if (respawnEnt.Name == "player2")
                                {
                                    Entity checkpoint2blue = Entity.FindEntityByName("Checkpoint2.Blue");
                                    _newRespawnPos = new Vector3(checkpoint2blue.GetComponent<Transform>().Position.X, checkpoint2blue.GetComponent<Transform>().Position.Y + 20.0f, checkpoint2blue.GetComponent<Transform>().Position.Z);
                                    player2respawn = true;
                                }
                                else
                                {
                                    _newRespawnPos = new Vector3(newCheckpoint.GetComponent<Transform>().Position.X, newCheckpoint.GetComponent<Transform>().Position.Y + 20.0f, newCheckpoint.GetComponent<Transform>().Position.Z);
                                    boxrespawn = true;

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