using AeonScriptCore;
using AeonScripts.Powerups;

namespace AeonScripts
{
    public class WeightPowerUp :AeonBehaviour
    {
        // Weight
        public string entityName; // for weight's name
        private Entity ent = null; //  weight
        private WeightCollide weightCollideScript = null;
        //private Transform weightPowerUpLocation = null;

        // Player stuffs
        private PlayerInventory inventory = null;
        public MoveCharacter moveCharacter = null;
        private AudioComponent audioComp = null;

        private Vector3 groundPos = Vector3.ZERO;

        public string highlightPlatformname;
        private Transform highlightTransform = null;

        private float animDuration = 0.0f;
        private bool shouldDestroy = false;
        private bool hasWeightActivated = false;
        private bool standingOnGround = false; // prevent player from activating the powerup if they are not standing on the ground

        public override void OnCreate()
        {
            moveCharacter = entity.GetScript<MoveCharacter>();

            Entity weightPowerUp = Entity.FindEntityByName(entityName);
            if (weightPowerUp != null)
            {
                //weightPowerUpLocation = weightPowerUp.GetComponent<Transform>();
                weightCollideScript = weightPowerUp.GetScript<WeightCollide>();

                if (weightCollideScript != null) {
                    weightCollideScript.owner = this;
                }
            }

            Entity highlightPlatform = Entity.FindEntityByName(highlightPlatformname);
            if (highlightPlatform != null)
            {
                highlightTransform = highlightPlatform.GetComponent<Transform>();
            }

            audioComp = entity.GetComponent<AudioComponent>();
        }

        public override void OnCollisionUpdate(uint otherEntityID)
        {
            Entity ent2 = Entity.GetEntity(otherEntityID);  // ground
            //NativeLog.Log($"weightlocation exists {weightPowerUpLocation != null}");

            if (inventory != null && inventory.HoldingPowerUp == true)
            {
                string name = ent2.Name;
                //NativeLog.LogError("standing on {0}", name);
                if (name.StartsWith("Ground."))
                {
                    if (shouldDestroy == false && inventory.weightCollected == true)
                    {
                        ////getting weight location to destroy platform
                        //weightPowerUpLocation.Position = ent2.GetComponent<Transform>().Position + new Vector3(0, 10, 0);
                        groundPos = ent2.GetComponent<Transform>().Position;

                        Vector3 groundDir = groundPos - moveCharacter.GetTransform().Position;
                        //NativeLog.Log("groundDir: {0}", groundDir);

                        // this is for if player is colliding with the ground from the side, like they are going to drop in the abyss, but they are "sticking" to the side of the ground tile
                        if (groundDir.Y > -7.0f) // if the Y component is positive, that means the player is on the side
                        {
                            standingOnGround = false;
                            highlightTransform.Position = new Vector3(0, 1000, 0);
                            return;
                        }
                        
                        //highlight platform
                        highlightTransform.Position = groundPos + new Vector3(0,5,0);
                        //NativeLog.Log($"ground postition {ent2.GetComponent<Transform>().Position}, highlistLocation {highlightLocation.Position}");
                        
                        ent = ent2;
                        standingOnGround = true;
                        //NativeLog.Log($"ent 1{ent.Name}");
                    }
                }
                if (name.ToLower().Contains("checkpoint"))
                {
                    standingOnGround = false;
                    highlightTransform.Position = new Vector3(0, 1000, 0);
                }
            }
        }

        public override void OnCollisionExit(uint otherEntityID)
        {
            Entity ent2 = Entity.GetEntity(otherEntityID);  // ground
            if (ent2 != null)
            {
                string name = ent2.Name;
                if (name.StartsWith("Ground."))
                {
                    //standingOnGround = false;
                    highlightTransform.Position = new Vector3(0, 1000, 0);
                }
            }
        }

        public override void Update()
        {
            if (GameState.IsPaused) return;
            if (inventory != null)
            {
                if (inventory.HoldingPowerUp == true && inventory.weightCollected == true)
                {
                    if (shouldDestroy == true)
                    {
                        animDuration -= Time.DeltaTime;
                        if (animDuration >= 2.0f && animDuration <= 2.35f)
                        {
                            if (weightCollideScript != null)
                                weightCollideScript.GetTransform().Position += new Vector3(0, Time.DeltaTime * 18.0f, 0);
                            //NativeLog.Log("moving up");
                        }

                        //move down
                        else 
                        {
                            if (weightCollideScript != null)
                            {
                                // passed the ground pos's Y so time to destroy!!
                                if (weightCollideScript.GetTransform().Position.Y <= groundPos.Y)
                                {
                                    EndOfWeightAnim();
                                    return;
                                }

                                weightCollideScript.GetTransform().Position -= new Vector3(0, Time.DeltaTime * 82.0f, 0);
                                //letGravTakeOver = true;
                            }
                                //weightCollideScript.GetTransform().Position -= new Vector3(0, Time.DeltaTime * 82.0f, 0);
                            //NativeLog.Log("moving down");
                            //if (weightPowerUpLocation.Position.Y <= 0)
                            //{
                            //    shouldDestroy = false;
                            //    inventory.HoldingPowerUp = false;
                            //    Entity.Destroy(ent);// destroy ground
                            //    NativeLog.Log("platform should destroy");
                            //    inventory.weightCollected = false;
                            //    inventory.HoldingPowerUp = false;
                            //    hasWeightActivated = false;
                            //    //inventory.uiTransform.Scale = Vector3.ZERO;
                            //    //hide the powerup after destroying the platform
                            //    weightPowerUpLocation.Position = new Vector3(0, 1000, 0);
                            //    highlightTransform.Position = new Vector3(0, 1000, 0);
                                
                            //}

                        }
                    }
                    //NativeLog.Log($"Highlight Y position: {highlightLocation.Position.Y}");

                    if (moveCharacter.WasInteractKeyPressed() && !hasWeightActivated && standingOnGround)
                    {
                        ActivateWeight();
                        NativeLog.Log("Activated weight");
                    }
                }
            }
            else 
            {
                inventory = entity.GetScript<PlayerInventory>();
            }
        }

        void ActivateWeight()
        {
            //getting weight location to destroy platform
            //weightPowerUpLocation.Position = groundPos + new Vector3(0, 25, 0);
            if (weightCollideScript != null)
            {
                weightCollideScript.SetLocation(groundPos + new Vector3(0, 25, 0));
                weightCollideScript.PlayCrashingSound();
            }
            hasWeightActivated = true;
            animDuration = 2.3f;
            shouldDestroy = true;
            //if (audioComp != null)
            //{
            //    audioComp.SoundFilepath = ".\\resources\\audios\\SFX Weight.wav";
            //    audioComp.PlaySound();
            //}
            moveCharacter.PlayPlayerHittingSomething();
        }

        public void EndOfWeightAnim()
        {
            shouldDestroy = false;
            inventory.HoldingPowerUp = false;
            Entity.Destroy(ent);// destroy ground
            NativeLog.Log("platform should destroy");
            inventory.weightCollected = false;
            inventory.HoldingPowerUp = false;
            hasWeightActivated = false;
            //inventory.uiTransform.Scale = Vector3.ZERO;

            //hide the powerup after destroying the platform
            if (weightCollideScript != null)
            {
                weightCollideScript.SetLocation(new Vector3(0, 1000, 0));
            }

            highlightTransform.Position = new Vector3(0, 1000, 0);
        }
    }
}
