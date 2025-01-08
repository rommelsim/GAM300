using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class DashPowerUp : AeonBehaviour
    {
        public PlayerInventory inventory = null;
        public MoveCharacter playerMoveScript = null;
        public AudioComponent audioComp = null;

        private MoveCharacter otherPlayerMoveScript = null;

        public float speedMultiplier = 2.0f;
        public float maxDashDuration = 0.2f;
        public float pushOtherPlayerForce = 25.0f;

        public float dashDuration = 0.0f;
        private bool wasImpulseAdded = false;

        public override void OnCreate()
        {
            inventory = entity.GetScript<PlayerInventory>();
            playerMoveScript = entity.GetScript<MoveCharacter>();
            audioComp = entity.GetComponent<AudioComponent>();
        }

        public override void OnCollisionEnter(uint otherEntityID)
        {
            Entity otherPlayer = Entity.GetEntity(otherEntityID);
            MoveCharacter otherMoveScript = otherPlayer.GetScript<MoveCharacter>();
            if (otherMoveScript == null) return;

            if (otherMoveScript != null && dashDuration > 0.0f)
            {
                otherPlayerMoveScript = otherMoveScript;
                Vector3 rotation = playerMoveScript.GetTransform().Rotation - playerMoveScript.GetBaseRotation();

                float ownerRotY = rotation.Y * MathHelper.DegToRad; // convert the transform's rotation from deg to rad

                /* To rotate our point (which can be inferred by the player's forward direction) by the Y-axis, in which we only need to care about x and z coordinates
                  x2 = x1 * cos  + z1 * sin
                  z2 = x1 * -sin + z1 * cos
                */
                Vector3 ownerForwardDir = playerMoveScript.forwardDir;
                float cosResult = MathHelper.Cos(ownerRotY);
                float sinResult = MathHelper.Sin(ownerRotY);

                Vector3 ownerFacingDir = new Vector3(ownerForwardDir.X * cosResult + ownerForwardDir.Z * sinResult, 0.0f, ownerForwardDir.X * (-sinResult) + ownerForwardDir.Z * cosResult); // first, calculate the rotated point


                Vector3 otherPlayerLookDir = ownerFacingDir * (otherPlayerMoveScript.moveSpeed + pushOtherPlayerForce);
                otherPlayerMoveScript.AddXZImpulseForce(otherPlayerLookDir);
                otherPlayerMoveScript.IsScriptActive = false;
                NativeLog.Log("Player pushed other player");
                //player2RB.AddImpulseForce(ref otherPlayerLookDir);

            }
        }

        public override void Update()
        {

            if (inventory != null)
            {
                if (dashDuration <= 0.0)
                {
                    playerMoveScript.moveSpeed = playerMoveScript.OgMoveSpeed;
                    if (wasImpulseAdded)
                    {
                        playerMoveScript.IsScriptActive = true;
                        wasImpulseAdded = false;
                    }

                    if (otherPlayerMoveScript != null)
                    {
                        otherPlayerMoveScript.IsScriptActive = true;
                        
                        otherPlayerMoveScript = null;
                    }
                } 
                else
                {
                    dashDuration -= Time.DeltaTime;
                }

                if (inventory.HoldingPowerUp == true)
                {
                    if (inventory.dashCollected == true)
                    {
                        if (playerMoveScript.WasInteractKeyPressed())
                        {
                            NativeLog.Log("Dash used");
                            dashDuration = maxDashDuration;
                            if (dashDuration >= maxDashDuration)
                            {
                                //playerMoveScript.moveSpeed = playerMoveScript.moveSpeed * speedMultiplier;
                                playerMoveScript.AddXZImpulseForce(playerMoveScript.GetFacingDirection() * 100f);
                                playerMoveScript.IsScriptActive = false;
                                wasImpulseAdded = true;
                            }

                            if (audioComp !=  null)
                            {
                                if (playerMoveScript.isPlayerOne)
                                {
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Red Cat_Hit_2.wav";
                                }
                                else
                                {
                                    audioComp.SoundFilepath = ".\\resources\\audios\\Blue Cat_Win_1.wav";
                                }
                                audioComp.ChannelType = Audio.ChannelType.SFX;
                                audioComp.Volume = 0.6f;
                                audioComp.PlaySound();
                            }

                            inventory.dashCollected = false;
                            inventory.HoldingPowerUp = false;

                        }
                    }
                }
            }

        }
    }

    
}
