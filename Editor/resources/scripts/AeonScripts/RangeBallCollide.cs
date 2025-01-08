using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class RangeBallCollide : AeonBehaviour
    {
        private Entity particles = null;
        public bool collided = false;
        private Entity explosion = null;
        private Entity hitPlayer = null;
        public AudioComponent audioComp = null;


        private float reset = 0.2f;
        public bool canReset = false;

        public override void OnCreate()
        {
            if(entity.Name == "Player1RangeBall")
            {
                particles = Entity.FindEntityByName("Ball1Part");
                explosion = Entity.FindEntityByName("Ball1ExplosionCollider");
            }
            else if (entity.Name == "Player2RangeBall")
            {
                particles = Entity.FindEntityByName("Ball2Part");
                explosion = Entity.FindEntityByName("Ball2ExplosionCollider");
            }
            audioComp = entity.GetComponent<AudioComponent>();
        }

        public override void Update()
        {
            particles.GetComponent<Transform>().Position = entity.GetComponent<Transform>().Position;
            if (canReset)
            {
                reset -= Time.DeltaTime;
                if(reset <= 0f)
                {
                    canReset = false;
                    if (hitPlayer != null)
                    {
                        hitPlayer.GetScript<MoveCharacter>().IsScriptActive = true;
                    }
                   
                    collided = true;
                    reset = 0.2f;
                }
            }
        }

        public override void OnCollisionEnter(uint otherEntityID)
        {
            Entity collidedEnt = Entity.GetEntity(otherEntityID);
            hitPlayer = null;

            if (collidedEnt.Name.Equals("Player 1") || collidedEnt.Name.Equals("player2") || 
                collidedEnt.Name.Equals("Player 1 Star") || collidedEnt.Name.Equals("Player 2 Star"))
            {
                if (collidedEnt.Name.Equals("Player 1 Star"))
                {
                    hitPlayer = Entity.FindEntityByName("Player 1");
                }
                else if (collidedEnt.Name.Equals("Player 2 Star"))
                {
                    hitPlayer = Entity.FindEntityByName("player2");
                }
                else if (collidedEnt.Name.Equals("Player 1") || collidedEnt.Name.Equals("player2"))
                {
                    hitPlayer = collidedEnt;
                }

                Vector3 rotation = hitPlayer.GetScript<MoveCharacter>().GetTransform().Rotation - hitPlayer.GetScript<MoveCharacter>().GetBaseRotation();

                float ownerRotY = rotation.Y * MathHelper.DegToRad; // convert the transform's rotation from deg to rad

                /* To rotate our point (which can be inferred by the player's forward direction) by the Y-axis, in which we only need to care about x and z coordinates
                  x2 = x1 * cos  + z1 * sin
                  z2 = x1 * -sin + z1 * cos
                */
                Vector3 ownerForwardDir = -hitPlayer.GetScript<MoveCharacter>().forwardDir;
                float cosResult = MathHelper.Cos(ownerRotY);
                float sinResult = MathHelper.Sin(ownerRotY);

                Vector3 ownerFacingDir = new Vector3(ownerForwardDir.X * cosResult + ownerForwardDir.Z * sinResult, 0.0f, ownerForwardDir.X * (-sinResult) + ownerForwardDir.Z * cosResult); // first, calculate the rotated point


                Vector3 otherPlayerLookDir = ownerFacingDir * 50f;
                hitPlayer.GetScript<MoveCharacter>().IsScriptActive = false;
                hitPlayer.GetScript<MoveCharacter>().AddXZImpulseForce(otherPlayerLookDir);
                hitPlayer.GetScript<MoveCharacter>().DropPickable();

                if (audioComp != null)
                {
                    if (hitPlayer.GetScript<MoveCharacter>().isPlayerOne)
                    {
                        int randNum = GameData.random.Next(1, 12);
                        audioComp.SoundFilepath = $".\\resources\\audios\\Red Cat_Get_Hit_{randNum}.wav";
                    }
                    else
                    {
                        int randNum = GameData.random.Next(1, 10);
                        audioComp.SoundFilepath = $".\\resources\\audios\\Blue Cat_Get_Hit_0{randNum}.wav";
                    }
                    audioComp.ChannelType = Audio.ChannelType.SFX;
                    audioComp.Volume = 0.6f;
                    audioComp.PlaySound();
                }

                hitPlayer.GetScript<RangBallPowerUp>().hitThisPlayer = true;
                canReset = true;
            }
            else if (!collidedEnt.Name.Equals("Player 1") && !collidedEnt.Name.Equals("player2"))
            {
                canReset = true;
            }

        }

    }
}
