using AeonScriptCore;
using System.Resources;

namespace AeonScripts
{
    internal class HammerPowerUp : AeonBehaviour
    {
        private PlayerInventory inventory = null;
        private Transform transformComp =  null;
        private Transform hammerOldTransform = null;
        private MoveCharacter PlayerMovement = null;
        private AudioComponent audioComp = null;
        
        public string entityName; // Name for hammer
        private Entity hammerPowerUp = null;
        private Transform hammerPowerUpLocation = null;
        private HammerCollide hammerCollision = null;

        private float animDuration = 0.0f;
        private bool activateHammer = false;
        private bool playedMissSound = false;

        private float scaleMultiplyer = 2.0f;

        public bool swinging = false;

        private bool boop = false;

        public float swingCooldown = 2f;
        public float collideroffset = 0.0f;
        
        public override void OnCreate()
        {
            transformComp = entity.GetComponent<Transform>();
            inventory = entity.GetScript<PlayerInventory>();
            PlayerMovement = entity.GetScript<MoveCharacter>();
            audioComp = entity.GetComponent<AudioComponent>();

            hammerPowerUp = Entity.FindEntityByName(entityName);
            if (hammerPowerUp != null)
            {
                hammerCollision = hammerPowerUp.GetScript<HammerCollide>();
                hammerPowerUpLocation = hammerPowerUp.GetComponent<Transform>();
            }
        }

        void ActivateHammer()
        {
            playedMissSound = false;
            //move up
            activateHammer = true;

            if (activateHammer)
            {
                hammerPowerUpLocation.Scale = hammerPowerUpLocation.Scale * scaleMultiplyer;
            }

            animDuration = 4.0f;
            hammerCollision.owner = entity;
            NativeLog.LogWarn("actvate hammer");
            NativeLog.LogWarn("facing dir:{0}", PlayerMovement.GetFacingDirection());
            swinging = true;

            if (audioComp != null)
            {
                int randNum = GameData.random.Next(1, 5); // range is 1 to 4, exclusive of 5
                audioComp.SoundFilepath = $".\\resources\\audios\\Hammer_Swing_0{randNum}.wav";
                audioComp.Volume = 0.6f;
                audioComp.ChannelType = Audio.ChannelType.SFX;
                audioComp.PlaySound();
            }

            PlayerMovement.PlayPlayerHittingSomething();
        }

        public override void Update()
        {
            if (hammerPowerUp == null)
            {
                hammerPowerUp = Entity.FindEntityByName(entityName);
            }
            else
            {
                if (hammerPowerUpLocation == null)
                    hammerPowerUpLocation = hammerPowerUp.GetComponent<Transform>();
                if (hammerCollision == null)
                    hammerCollision = hammerPowerUp.GetScript<HammerCollide>();
            }

            if (GameState.IsPaused) return;
            if (activateHammer == true && hammerPowerUpLocation != null && hammerCollision != null)
            {
                Vector3 lookAtDir = PlayerMovement.GetFacingDirection();
                Vector3 lookAtRotation = transformComp.Rotation + lookAtDir + new Vector3 (0,-90,0);
                Vector3 animHammerPos = transformComp.Position + (lookAtDir * 5);

                hammerCollision.SetCollisionOffset(lookAtDir * collideroffset);//(PlayerMovement.isPlayerOne ? -11.5f : 11.5f));
                
                animDuration -= Time.DeltaTime;

                if (animDuration <= 4.0f && animDuration > 3.2f)
                {
                    float time = (0.5f - (animDuration - 3.5f)) / 0.5f;
                    if (time >= 0.95f)
                    {
                        if (!hammerCollision.shouldStun)
                        {
                            hammerCollision.shouldStun = true;
                        }
                        //else
                        //{
                        //    hammerCollision.PlayMissSound();
                        //    NativeLog.LogWarn("PlayMissSound");
                        //}
                        if (time >= 1.0f)
                        {
                            time = 1.0f;
                        }
                    }
                    if (animDuration < 3.6f && !boop)
                    {
                        boop = true;
                        Vector3 playerBoop = new Vector3(0, 25f, 0);

                        PlayerMovement.entity.GetComponent<RigidBody>().AddImpulseForce(ref playerBoop);
                    }

                    if (animDuration < 3.4f)
                    {
                        if (!hammerCollision.WasOtherPlayerStunned() && !playedMissSound)
                        {
                            NativeLog.Log("play hammer miss sound");
                            hammerCollision.PlayMissSound();
                            playedMissSound = true;
                        }
                    }

                    hammerPowerUpLocation.Position = animHammerPos;
                    hammerPowerUpLocation.Rotation = lookAtRotation + Vector3.Lerp(Vector3.ZERO, new Vector3(0, 0, -100), time);
                }
                else if (animDuration <= 3.2f && animDuration > 2.7f)
                {
                    NativeLog.LogWarn("hammer going up");
                    hammerCollision.shouldStun = false;

                    float time = (0.5f - (animDuration - 2.8f)) / 0.5f;
                    hammerPowerUpLocation.Position = animHammerPos;
                    hammerPowerUpLocation.Rotation = lookAtRotation + Vector3.Lerp(new Vector3(0, 0, -100), Vector3.ZERO, time);
                }
                //move down
                else
                {
                    animDuration = 0.0f;
                    //inventory.hammerCollected = false;

                    //hide the hammer after swinging down
                    hammerPowerUpLocation.Position = new Vector3(0, 1000, 0);
                    activateHammer = false;
                    boop = false;
                    if (activateHammer == false)
                    {
                        hammerPowerUpLocation.Scale = hammerPowerUpLocation.Scale / scaleMultiplyer;
                    }
                    //swinging = false;
                    //inventory.HoldingPowerUp = false;
                }
            }

            if (PlayerMovement.WasPermKeyPress() && !PlayerMovement.IsCarryingPickable())
            {
                if (!swinging)
                {
                    ActivateHammer();
                }
                
            }
            if (swinging)
            {
                swingCooldown = animDuration/2;
                if(swingCooldown <= 0f)
                {
                    swinging = false;
                }
            }
            //if (inventory != null)
            //{
            //    //Transform playerTransform = entity.GetComponent<Transform>();

            //    if (inventory.HoldingPowerUp == true && inventory.hammerCollected == true)
            //    {





            //    }
            //}
            //else
            //{
            //    inventory = entity.GetScript<PlayerInventory>();
            //}

        }
    }
}
