using AeonScriptCore;
using AeonScriptCore.Physics;
using System;
using System.Collections.Generic;
using System.IO.Pipes;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class PlayerInventory :AeonBehaviour
    {
        public enum PickUpSFXType
        {
            None = 0,
            Weight = 1,
        }
        [HideFromEditor]
        public bool PlayPickupSfx = false;

        public bool HoldingPowerUp = false;
        public string uiEntity;
        public Transform uiTransform = null;
        
        private AudioComponent audioComp = null;
        private bool hasplayedyet = false;


        MoveCharacter movePlayer = null;
        Transform playerTransform = null;
        public float Duration = 0.0f;

        public float oldMoveSpeed = 0.0f;
        public float newMoveSpeed = 0.0f;

        public bool weightCollected = false;
        //public bool bananaCollected = false;
        public bool hammerCollected = false;
        public bool rangeBallCollected = false;
        public bool starCollected = false;
        public bool dashCollected = false;
        public bool spinnerCollected = false;

        private Entity displayEntity = null;

        public Entity banana = null;

        private UI powerUpUi = null;

        private Transform highlightTransform = null;

        public string highlightPlatformname;
        private bool standingOnGround = false;
        private bool hasSpinnerActivate = false;

        private Vector3 groundPos;
        private Entity spinnerEnt = null;
        private Vector3 originalSpinnerPos;

        private float spinDuration = 0f;

        public override void OnCreate()
        {
            audioComp = entity.GetComponent<AudioComponent>();
            movePlayer = entity.GetScript<MoveCharacter>();
            playerTransform = entity.GetComponent<Transform>();
            oldMoveSpeed = movePlayer.moveSpeed;
            newMoveSpeed = oldMoveSpeed * 10.0f;

            //ui stuff
            displayEntity = Entity.FindEntityByName(uiEntity);

            Entity highlightPlatform = Entity.FindEntityByName(highlightPlatformname);
            if (highlightPlatform != null)
            {
                highlightTransform = highlightPlatform.GetComponent<Transform>();
            }

            if (displayEntity != null)
            {
                uiTransform = displayEntity.GetComponent<Transform>();
                powerUpUi = displayEntity.GetComponent<UI>();
                //uiTransform.Scale = Vector3.ZERO;
                powerUpUi.Texture = ".\\resources\\textures\\InventoryBubble.dds";
            }

            if (movePlayer.isPlayerOne)
            {
                spinnerEnt = Entity.FindEntityByName("RedSpinner");
                originalSpinnerPos = spinnerEnt.GetComponent<Transform>().Position;
            }
            else
            {
                spinnerEnt = Entity.FindEntityByName("BlueSpinner");
                originalSpinnerPos = spinnerEnt.GetComponent<Transform>().Position;
            }

        }

        public override void Update()
        {
            if (weightCollected == true)
            {
                if (movePlayer.isPlayerOne)
                {
                    //red player UI
                    powerUpUi.Texture = ".\\resources\\textures\\RedCollect(Weight).dds";
                }
                else
                {
                    //blue player UI
                    powerUpUi.Texture = ".\\resources\\textures\\BlueCollect(Weight).dds";
                }
                //NativeLog.LogWarn("swapping Ui to weight");

                //powerUpUi.Texture = ".\\resources\\textures\\InventoryBubbleWeight.dds";

            }

            if (HoldingPowerUp == false && weightCollected == false && rangeBallCollected == false && !dashCollected && !spinnerCollected && !starCollected) 
            {
                if (movePlayer.isPlayerOne)
                {
                    //red player UI
                    powerUpUi.Texture = ".\\resources\\textures\\RedGrey.dds";
                    //displayEntity.GetComponent<Transform>().Position = new Vector3(-800, 400,0);
                    //displayEntity.GetComponent<Transform>().Scale = new Vector3(200, 200, 1);
                }
                else
                {
                    //blue player UI
                    powerUpUi.Texture = ".\\resources\\textures\\BlueGrey.dds";
                    //displayEntity.GetComponent<Transform>().Position = new Vector3(800,400,0);
                    //displayEntity.GetComponent<Transform>().Scale = new Vector3(200, 200, 1);
                }
                //powerUpUi.Texture = ".\\resources\\textures\\EmptyBlue.dds";
            }

            if (rangeBallCollected == true)
            {
                if (movePlayer.isPlayerOne)
                {
                    //red player UI
                    powerUpUi.Texture = ".\\resources\\textures\\RedCollect(Ball).dds";
                }
                else
                {
                    //blue player UI
                    powerUpUi.Texture = ".\\resources\\textures\\BlueCollect(Ball).dds";
                }
            }

            if (starCollected == true)
            {
                if (movePlayer.isPlayerOne)
                {
                    //red player UI
                    powerUpUi.Texture = ".\\resources\\textures\\RedCollect(Star).dds";
                }
                else
                {
                    //blue player UI
                    powerUpUi.Texture = ".\\resources\\textures\\BlueCollect(Star).dds";
                }
            }

            if (dashCollected == true)
            {
                if (movePlayer.isPlayerOne)
                {
                    //red player UI
                    powerUpUi.Texture = ".\\resources\\textures\\RedCollect(Dash).dds";
                }
                else
                {
                    //blue player UI
                    powerUpUi.Texture = ".\\resources\\textures\\BlueCollect(Dash).dds";
                }
            }

            if (spinnerCollected)
            {
                if (movePlayer.isPlayerOne)
                {
                    //red player UI
                    powerUpUi.Texture = ".\\resources\\textures\\RedCollect(Spinny).dds";
                }
                else
                {
                    //blue player UI
                    powerUpUi.Texture = ".\\resources\\textures\\BlueCollect(Spinny).dds";
                }

            }


            if (movePlayer.WasInteractKeyPressed() && !hasSpinnerActivate && standingOnGround && HoldingPowerUp && spinnerCollected)
            {
                ActivateSpinner();
                //NativeLog.Log("Activated weight");
            }

            if (hasSpinnerActivate)
            {
                var colliderShape = spinnerEnt.GetComponent<Collider>().GetColliderShape();
                CubeColliderShape cubeColliderShape = (CubeColliderShape)colliderShape;
                if(spinDuration > 0)
                {
                    if (GameState.IsPaused) return;
                    spinDuration -= Time.DeltaTime;
                    if (spinnerEnt.GetComponent<Transform>().Scale.X < 12f)
                    {
                        spinnerEnt.GetComponent<Transform>().Scale += new Vector3(10f, 10f, 10f) *Time.DeltaTime;
                        cubeColliderShape.HalfSize = Vector3.ZERO;
                    }
                    else if(spinnerEnt.GetComponent<Transform>().Scale.X > 10f)
                    {
                        cubeColliderShape.HalfSize = new Vector3(10, 3, 1);
                        return;
                    }

                }
                else
                {
                    spinnerEnt.GetComponent<Transform>().Scale -= new Vector3(30f, 30f, 30f) * Time.DeltaTime;
                    spinnerEnt.GetComponent<Transform>().Position = originalSpinnerPos;
                    hasSpinnerActivate = false;
                }
                
            }


        }

        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            if (otherEnt == null) return;

            CollectPowerUp collectPowerUpScript = otherEnt.GetScript<CollectPowerUp>();
            //HoldingPowerUp = true;
            
            if (collectPowerUpScript == null) return;

            if (PlayPickupSfx == true)
            {
                PlaySfx();
                //if (collectPowerUpScript.powerType.Equals("banana") && !collectPowerUpScript.isPlacedDownbanana)
                //{
                //    PlaySfx();
                //}
                //if (collectPowerUpScript.powerType.Equals("weight"))
                //{
                //    PlaySfx();
                //}
                //if (collectPowerUpScript.powerType.Equals("hammer"))
                //{
                //    PlaySfx();
                //}
            }
        }

        public override void OnCollisionUpdate(uint otherEntityID)
        {
            
            Entity ent2 = Entity.GetEntity(otherEntityID);

            if (HoldingPowerUp == true)
            {
                string name = ent2.Name;
                //NativeLog.LogError("standing on {0}", name);
                if (name.StartsWith("Ground."))
                {
                    if (spinnerCollected == true)
                    {
                        ////getting weight location to destroy platform
                        //weightPowerUpLocation.Position = ent2.GetComponent<Transform>().Position + new Vector3(0, 10, 0);
                        groundPos = ent2.GetComponent<Transform>().Position;

                        Vector3 groundDir = groundPos - movePlayer.GetTransform().Position;
                        //NativeLog.Log("groundDir: {0}", groundDir);

                        // this is for if player is colliding with the ground from the side, like they are going to drop in the abyss, but they are "sticking" to the side of the ground tile
                        if (groundDir.Y > -7.0f) // if the Y component is positive, that means the player is on the side
                        {
                            standingOnGround = false;
                            highlightTransform.Position = new Vector3(0, 1000, 0);
                            return;
                        }

                        //highlight platform
                        highlightTransform.Position = groundPos + new Vector3(0, 5, 0);
                        //NativeLog.Log($"ground postition {ent2.GetComponent<Transform>().Position}, highlistLocation {highlightLocation.Position}");

                        standingOnGround = true;
                        //NativeLog.Log($"ent 1{ent.Name}");
                    }
                }
            }
        }
        //public override void OnCollisionExit(uint otherEntityID)
        //{
        //    Entity ent2 = Entity.GetEntity(otherEntityID);  // ground
        //    if (ent2 != null)
        //    {
        //        string name = ent2.Name;
        //        if (name.StartsWith("Ground."))
        //        {
        //            //standingOnGround = false;
        //            highlightTransform.Position = new Vector3(0, 1000, 0);
        //        }
        //    }
        //}

        private void ActivateSpinner()
        {
            hasSpinnerActivate = true;
            spinDuration = 2f;

            spinnerEnt.GetComponent<Transform>().Position = groundPos + new Vector3(0, 5, 0);
            spinnerEnt.GetComponent<Transform>().Scale = new Vector3(1,1,1);

            
            //if (!hasplayedyet)
            {
                PlaySpin();
                //hasplayedyet = true;
            }
            

            spinnerCollected = false;
            HoldingPowerUp = false;

        }

        private void PlaySfx()
        {
            if (audioComp == null)
                return;

            int randNum = GameData.random.Next(1, 3);
            audioComp.SoundFilepath = $".\\resources\\audios\\sfx_pickup_powerup_{randNum}.wav";
            audioComp.PlaySound();

            PlayPickupSfx = false;
        }

        public void PlaySpin()
        {
            if (audioComp != null)
            {
                int randNum = GameData.random.Next(1, 3);
                audioComp.SoundFilepath = $".\\resources\\audios\\Spinny{randNum}.wav";
                audioComp.ChannelType = Audio.ChannelType.SFX;
                audioComp.Volume = 0.6f;
                audioComp.PlaySound();
            }
        }
    }
    
}
