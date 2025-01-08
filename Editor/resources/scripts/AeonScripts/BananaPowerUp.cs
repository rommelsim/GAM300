using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Authentication.ExtendedProtection.Configuration;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{


    public class BananaPowerUp : AeonBehaviour
    {


        public PlayerInventory inventory = null;
        public Transform playerLocation = null;
        public MoveCharacter playerMoveScript = null;
        //MoveCharacter player = null;
        [HideFromEditor]
        public bool canPlaceBanana;
        [HideFromEditor]
        public bool canSlide;
        [HideFromEditor]
        public bool canGetBanana;
        [HideFromEditor]
        public bool collected;

        private Vector3 lookAtDir;
        public float Duration = 0.0f;

        public float slideForce = 4.0f;
        public CollectPowerUp banana = null;

        public Transform bananaLocation = null;
        private float ogSpeed = 0.0f;

        private AudioComponent audioComp = null;

        public float bananaCooldown = 3f;

        private bool canUseBanana = true;
        
        public override void OnCreate()
        {
            playerMoveScript = this.entity.GetScript<MoveCharacter>();
            collected = false;
            canPlaceBanana = false;
            canSlide = true;
            canGetBanana = true;
            inventory = this.entity.GetScript<PlayerInventory>();
            playerLocation = this.entity.GetComponent<Transform>();
            ogSpeed = playerMoveScript.moveSpeed;

            audioComp = entity.GetComponent<AudioComponent>();
        }

        public override void OnCollisionEnter(uint otherEntityID)
        {
            Entity collidedEnt = Entity.GetEntity(otherEntityID);
            if (collidedEnt != null)
            {
                CollectPowerUp tmp = collidedEnt.GetScript<CollectPowerUp>();
                if (tmp != null && tmp.powerType == "banana" && tmp.isPlacedDownbanana)
                {
                    playerMoveScript.DropPickable();
                    NativeLog.Log("{0} sliding player", collidedEnt.Name);
                    
                    Duration = 1.5f;
                    ogSpeed = playerMoveScript.moveSpeed;
                    canSlide = true;
                    playerMoveScript.AddXZImpulseForce((playerMoveScript.GetFacingDirection() + new Vector3(((float)GameData.random.NextDouble()), 0, ((float)GameData.random.NextDouble()))) * 70f);
                    playerMoveScript.IsScriptActive = false;
                    
                    int randplay = GameData.random.Next(1, 101);
                    if (randplay % 5 == 1)
                    {
                        if (playerMoveScript.isPlayerOne)
                        {
                            if (audioComp != null)
                            {
                                int randNum = GameData.random.Next(1, 4);
                                audioComp.SoundFilepath = $".\\resources\\audios\\Red Cat_Slip_{randNum}.wav";
                                audioComp.ChannelType = Audio.ChannelType.Voice;
                                audioComp.Volume = 0.6f;
                                audioComp.PlaySound();
                            }
                        }
                        else
                        {
                            if (audioComp != null)
                            {
                                int randNum = GameData.random.Next(1, 4);
                                audioComp.SoundFilepath = $".\\resources\\audios\\Blue Cat_Slip_{randNum}.wav";
                                audioComp.ChannelType = Audio.ChannelType.Voice;
                                audioComp.Volume = 0.6f;
                                audioComp.PlaySound();
                            }
                        }
                    }
                    else
                    {
                        if (audioComp != null)
                        {
                            int randNum = GameData.random.Next(1, 4);
                            audioComp.SoundFilepath = $".\\resources\\audios\\sfx_banana_slip_{randNum}.wav";
                            audioComp.PlaySound();
                        }
                    }
                    Entity.Destroy(otherEntityID);
                }
            }
        }



        public override void Update()
        {
            if (GameState.IsPaused) return;
            if (canSlide)
            {
                if (Duration > 0.0f)
                {
                    NativeLog.LogWarn("We are sliding!");
                
                    Duration -= Time.DeltaTime;
                    entity.GetComponent<Transform>().Rotation += new Vector3(0, 90, 0) *Time.DeltaTime * 8f;
                    //playerMoveScript.moveSpeed += 5.0f;
                }
                else
                {
                    playerMoveScript.moveSpeed = playerMoveScript.OgMoveSpeed;
                    playerMoveScript.IsScriptActive = true;
                    canSlide = false;
                }
            }


            if (!canUseBanana)
            {
                bananaCooldown -= 1 * Time.DeltaTime;
                if(bananaCooldown <= 0f)
                {
                    bananaCooldown = 3f;
                    canUseBanana = true;
                }
            }

            if (playerMoveScript.WasPermKeyPress() && playerMoveScript.IsCarryingPickable() && canUseBanana)
            {
                canUseBanana = false;
                NativeLog.Log("{0} 1 placing down", "banana");
                Vector3 rotation = playerMoveScript.GetTransform().Rotation - playerMoveScript.GetBaseRotation();

                float ownerRotY = rotation.Y * MathHelper.DegToRad; // convert the transform's rotation from deg to rad

                /* To rotate our point (which can be inferred by the player's forward direction) by the Y-axis, in which we only need to care about x and z coordinates
                  x2 = x1 * cos  + z1 * sin
                  z2 = x1 * -sin + z1 * cos
                */
                Vector3 ownerForwardDir = -playerMoveScript.forwardDir;
                float cosResult = MathHelper.Cos(ownerRotY);
                float sinResult = MathHelper.Sin(ownerRotY);

                lookAtDir = new Vector3(ownerForwardDir.X * cosResult + ownerForwardDir.Z * sinResult, 0.0f, ownerForwardDir.X * (-sinResult) + ownerForwardDir.Z * cosResult); // first, calculate the rotated point

                CreateBanana(playerLocation.Position + (lookAtDir * 6f) + new Vector3(0, 6f, 0));
                //bananaLocation.Position = playerLocation.Position + (lookAtDir * 8.0f);

                //bananaLocation = null;
                //banana = null;

                //inventory.HoldingPowerUp = false;
                //inventory.bananaCollected = true;
                //canGetBanana = true;
                //canPlaceBanana = false;
                //collected = false;

            }

        }

        private void CreateBanana(Vector3 position)
        {
            EntityBuilder entBuilder = new EntityBuilder("Banana");
            TransformBuilder transformBuilder = new TransformBuilder();
            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();
            ColliderBuilder colliderBuilder = new ColliderBuilder();
            RigidBodyBuilder rbBuilder = new RigidBodyBuilder();

            rbBuilder.SetType(AeonScriptCore.Physics.RigidBodyType.DYNAMIC);
            rbBuilder.SetUseGravity(true);

            behaviourBuilder.AddScript<CollectPowerUp>();
            behaviourBuilder.AddScript<BananaSpawnUse>();

            transformBuilder.SetPosition(position); //+ new Vector3(0, 10, 0));
            transformBuilder.SetScale(new Vector3(0.7f, 0.7f, 0.7f));
            colliderBuilder.SetShape(new ColliderBuilder.CubeShapeBuilder().SetHalfSize(new Vector3(1.7f, 1.7f, 1.7f)));

            RenderBuilder renderBuilder = new RenderBuilder("./resources/objects/BananaPeel.bin", "./resources/textures/banana.dds");

            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(colliderBuilder);
            entBuilder.AddComponent(renderBuilder);
            entBuilder.AddComponent(behaviourBuilder);
            entBuilder.AddComponent(rbBuilder);

            Entity.Create(entBuilder, (entity) =>
            {
                CollectPowerUp collectPowerUp = entity.GetScript<CollectPowerUp>();
                BananaSpawnUse bananaSpawnUse = entity.GetScript<BananaSpawnUse>();
                Entity banana = entity;
                collectPowerUp.isPlacedDownbanana = true;
                collectPowerUp.powerType = "banana";
                bananaSpawnUse.playerChar = playerMoveScript;
                //entity.GetComponent<RigidBody>().UseGravity = true;
                //Vector3 impulseVec = position * 20f + new Vector3(0, 20, 0);
                //entity.GetComponent<RigidBody>().AddImpulseForce(ref impulseVec);
            });

        }

    }



}
