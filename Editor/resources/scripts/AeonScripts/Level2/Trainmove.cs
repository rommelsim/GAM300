using AeonScriptCore;
using AeonScriptCore.Physics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts.Level2
{
    public class Trainmove : AeonBehaviour
    {
            //public float TrainXTransform;
            //public float TrainYTransform;
            //public float TrainZTransform;

            //private Transform TrainNewTransform = null;
            private Transform TrainOldTransform = null;

            public float TrainResetLocation = 0.0f;

            public MoveCharacter player1MoveScript = null;
            public MoveCharacter player2MoveScript = null;

            public AudioComponent audioComp = null;

            public Transform playerTransform = null;

            private Entity trainSmoke = null;
            private Transform trainSmokeTransform = null;

            private bool shouldFlatten = false;
            private bool shouldP2Flatten = false;
            private bool hasTrainFallen = false;

            private bool trainFall = false;
            private bool tmz = false;

            public float stunDuration = 0.0f;
            public float p2StunDuration = 0.0f;
            public float TrainSpeed = 0.0f;
            //public float TrainFallDuration = 0.0f;
            private float TrainMoveTimer = 0.0f;

            private Vector3 Player1OldScale = Vector3.ZERO;
            private Vector3 Player2OldScale = Vector3.ZERO;
            private Vector3 TargetLocation = Vector3.ZERO;
            //private Vector3 newPosition = Vector3.ZERO;

            private Vector3 trainOriginalPostion = Vector3.ZERO;
            private Vector3 trainOriginalRotation = Vector3.ZERO;
            private Vector3 trainRotateAmount = new Vector3(90.0f,0.0f,0.0f);

            public override void OnCreate()
            {
                TrainOldTransform = entity.GetComponent<Transform>();
                //TrainNewTransform = TrainOldTransform; // init
                //TrainNewTransform.Rotation = new Vector3 (45.0f, 0.0f, 0.0f);
                TargetLocation = TrainOldTransform.Position + new Vector3(0, 0, -100.0f);
                trainOriginalPostion = TrainOldTransform.Position;
                audioComp = entity.GetComponent<AudioComponent>();
                trainSmoke = Entity.FindEntityByName("Smoke");

                if (trainSmokeTransform == null)
                {
                    trainSmokeTransform = trainSmoke.GetComponent<Transform>();
                }
                
            }

            public override void OnTriggerEnter(uint otherEntityID)
            {
                Entity player = Entity.GetEntity(otherEntityID);
                if (player == null) return;

                MoveCharacter localMoveScript = player.GetScript<MoveCharacter>();
                if (localMoveScript == null) return;

                playerTransform = player.GetComponent<Transform>();
                localMoveScript.DropPickable();
                PlayHitSound();

                if (localMoveScript.isPlayerOne)
                {
                    player1MoveScript = localMoveScript;
                    shouldFlatten = true;
                    stunDuration = 2.0f;
                    player1MoveScript.DropPickable();
                }

                if (!localMoveScript.isPlayerOne)
                {
                    player2MoveScript = localMoveScript;
                    shouldP2Flatten = true;
                    p2StunDuration = 2.0f;
                    player2MoveScript.DropPickable();
                }
                
            }


            public override void Update()
            {
                if (GameState.IsPaused) return;
                if (trainSmokeTransform != null)
                {
                    trainSmokeTransform.Position = TrainOldTransform.Position + new Vector3(0, 40.5f, 26f);
                }
                
                TrainMoveTimer += Time.DeltaTime;
                Vector3 trainPos = TrainOldTransform.Position;
                Vector3 trainRotation = TrainOldTransform.Rotation;
                
                //NativeLog.Log("${0}", TargetLocation);
                if (TrainMoveTimer >= 4.0f)
                {
                    //tmz = false;
                    if (!tmz)
                    {
                        PlayChoochoo();
                        tmz = true;
                    }
                    if (trainPos.Z < TrainResetLocation)
                    {
                        trainPos.Z += Time.DeltaTime * TrainSpeed; // multiply movement speed by delta time
                    }
                    else if (trainPos.Z >= TrainResetLocation)
                    {
                        //trainFall = true;
                        if (/*trainFall &&*/ trainRotation.X < trainRotateAmount.X)
                        {
                            trainRotation.X += Time.DeltaTime * TrainSpeed;

                            //rotation

                            if (trainRotation.X > trainRotateAmount.X * 0.6f)
                            {
                                trainPos.Y -= Time.DeltaTime * (TrainSpeed *2);
                                trainPos.Z += Time.DeltaTime * (TrainSpeed * 0.75f);
                                hasTrainFallen = true;
                            }
                            


                        }
                        else
                        {
                            trainFall = true;
                        }

                    //trainPos.Z += Time.DeltaTime * TrainSpeed;




                    //trainPos.Y -= Time.DeltaTime * TrainSpeed;
                    //trainRotation = trainRotateAmount;

                    //hasTrainFallen = true;
                    if (hasTrainFallen && trainFall)
                    {
                        trainPos.Y -= Time.DeltaTime * (TrainSpeed * 2.5f);
                    }
                    if (hasTrainFallen && trainPos.Y <= -120.0f)
                    {
                        trainPos = trainOriginalPostion;
                        trainRotation = trainOriginalRotation;
                        hasTrainFallen = false;
                        trainFall = false;

                        PlayChoochoo();
                    }
                    //trainPos.Z = trainOriginalPostion;
                    //TrainOldTransform.Position = trainPos;
                }
                    TrainOldTransform.Position = trainPos; //Vector3.Lerp(TrainOldTransform.Position, TargetLocation, Time.DeltaTime);
                    TrainOldTransform.Rotation = trainRotation;

                }




                if (player1MoveScript != null)
                {
                    if (shouldFlatten)
                    {
                        Player1OldScale = player1MoveScript.PlayerOgScale;
                        player1MoveScript.GetTransform().Scale = new Vector3(Player1OldScale.X * 2, Player1OldScale.Y / 2, Player1OldScale.Z);
                        var colliderShape = player1MoveScript.GetCollider().GetColliderShape();
                        CubeColliderShape cubeCollider = (CubeColliderShape)colliderShape;
                        cubeCollider.HalfSize = new Vector3(5f, 0.5f, 4f);
                        player1MoveScript.GetCollider().Offset = new Vector3(0, -3.05f, 0);
                        //player1MoveScript.AddXZImpulseForce(new Vector3(GameData.random.Next(-20,20),0,GameData.random.Next(-20,20)));
                        stunDuration -= Time.DeltaTime;
                        player1MoveScript.IsScriptActive = false;
                    }

                    if (stunDuration <= 0.0f)
                    {
                        player1MoveScript.GetTransform().Scale = Player1OldScale;
                        var colliderShape = player1MoveScript.GetCollider().GetColliderShape();
                        CubeColliderShape cubeCollider = (CubeColliderShape)colliderShape;
                        cubeCollider.HalfSize = new Vector3(2.5f, 2.7f, 4f);
                        player1MoveScript.GetCollider().Offset = new Vector3(0, -0.5f, 0);
                        player1MoveScript.IsScriptActive = true;
                        shouldFlatten = false;
                        player1MoveScript = null;


                        PlayUnflattenSound();
                    }
                }

                if (player2MoveScript != null)
                {
                    if (shouldP2Flatten)
                    {
                        Player2OldScale = player2MoveScript.PlayerOgScale;
                        player2MoveScript.GetTransform().Scale = new Vector3(Player2OldScale.X * 2, Player2OldScale.Y / 2, Player2OldScale.Z);
                        var colliderShape = player2MoveScript.GetCollider().GetColliderShape();
                        CubeColliderShape cubeCollider = (CubeColliderShape)colliderShape;
                        cubeCollider.HalfSize = new Vector3(5f, 0.5f, 4f);
                        player2MoveScript.GetCollider().Offset = new Vector3(0, -3.05f, 0);
                        //player2MoveScript.AddXZImpulseForce(new Vector3(GameData.random.Next(-20, 20), 0, GameData.random.Next(-20, 20)));
                        p2StunDuration -= Time.DeltaTime;
                        player2MoveScript.IsScriptActive = false;
                    }

                    if (p2StunDuration <= 0.0f)
                    {
                        player2MoveScript.GetTransform().Scale = Player2OldScale;
                        var colliderShape = player2MoveScript.GetCollider().GetColliderShape();
                        CubeColliderShape cubeCollider = (CubeColliderShape)colliderShape;
                        cubeCollider.HalfSize = new Vector3(2.5f, 2.7f, 4f);
                        player2MoveScript.GetCollider().Offset = new Vector3(0, -0.5f, 0);
                        player2MoveScript.IsScriptActive = true;
                        shouldP2Flatten = false;
                        player2MoveScript = null;

                        PlayUnflattenSound();

                    }
                }

            }
            public void PlayHitSound()
            {
                if (audioComp != null)
                {
                    int randNum = GameData.random.Next(1, 4);
                    audioComp.SoundFilepath = $".\\resources\\audios\\TrainSquish0{randNum}.wav";
                    audioComp.Volume = 0.15f;
                    audioComp.ChannelType = Audio.ChannelType.SFX;
                    NativeLog.LogWarn("Did it play Hit sound");
                    audioComp.PlaySound();
                }
            }

            public void PlayUnflattenSound()
            {
                if (audioComp != null)
                {
                    //int randNum = GameData.random.Next(1, 6);
                    audioComp.SoundFilepath = ".\\resources\\audios\\Hammer_Unflatten.wav";
                    audioComp.Volume = 0.6f;
                    audioComp.ChannelType = Audio.ChannelType.SFX;
                    //NativeLog.LogWarn("Did it play Unflatten sound");
                    audioComp.PlaySound();
                }
            }

            private void PlayChoochoo()
            {
                audioComp.SoundFilepath = ".\\resources\\audios\\TrainWhistle.wav";
                audioComp.Volume = 0.5f;
                audioComp.ChannelType = Audio.ChannelType.SFX;
                audioComp.PlaySound();
            }

       }
        
    }
