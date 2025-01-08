using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class RangBallPowerUp : AeonBehaviour
    {
        Entity onwerBall = null;
        public PlayerInventory inventory = null;
        public Transform playerLocation = null;
        public MoveCharacter playerMoveScript = null;
        public AudioComponent audioComp = null;

        private RigidBody rigidBody = null;
        private Collider collider = null;

        private RangeBallCollide ownBallCollideScript = null;

        private Vector3 holdPos;
        public bool tossBall = false;

        public bool hitThisPlayer = false;

        private float timerHit = 0.2f;
        public override void OnCreate()
        {
            
            if (this.entity.Name == "Player 1")
            {
                onwerBall = Entity.FindEntityByName("Player1RangeBall");
            }
            else if (this.entity.Name == "player2")
            {
                onwerBall = Entity.FindEntityByName("Player2RangeBall");
            }
            playerMoveScript = this.entity.GetScript<MoveCharacter>();
            inventory = this.entity.GetScript<PlayerInventory>();
            playerLocation = this.entity.GetComponent<Transform>();
            rigidBody = onwerBall.GetComponent<RigidBody>();
            collider = onwerBall.GetComponent<Collider>();
            holdPos = new Vector3(0, 1000,0);
            ownBallCollideScript = onwerBall.GetScript<RangeBallCollide>();
            audioComp = entity.GetComponent<AudioComponent>();

        }


        public override void Update()
        {
            if (playerMoveScript.WasInteractKeyPressed() && inventory.rangeBallCollected)
            {
                ShootBall();
                tossBall = true;
                inventory.HoldingPowerUp = false;
                inventory.rangeBallCollected = false;
                if(audioComp !=  null)
                {
                    audioComp.SoundFilepath = ".\\resources\\audios\\Cartoon_Boing_Bounce_Spring_Trampoline.wav";
                    audioComp.Volume = 1.0f;
                    audioComp.ChannelType = Audio.ChannelType.SFX;
                    audioComp.PlaySound();
                }

                playerMoveScript.PlayPlayerHittingSomething();
            }

            if (hitThisPlayer)
            {
                timerHit -= Time.DeltaTime;
                if(timerHit<= 0)
                {
                    playerMoveScript.IsScriptActive = true;
                    hitThisPlayer = false;
                    timerHit = 0.2f;
                }
            }

            if (ownBallCollideScript.collided)
            {
                ResetBall();
            }
            else
            {
                return;
            }

        }

        public void ShootBall()
        {
            Vector3 rotation = playerMoveScript.GetTransform().Rotation - playerMoveScript.GetBaseRotation();
            rigidBody.UseGravity = true;

            float ownerRotY = rotation.Y * MathHelper.DegToRad; // convert the transform's rotation from deg to rad

            /* To rotate our point (which can be inferred by the player's forward direction) by the Y-axis, in which we only need to care about x and z coordinates
              x2 = x1 * cos  + z1 * sin
              z2 = x1 * -sin + z1 * cos
            */
            Vector3 ownerForwardDir = playerMoveScript.forwardDir;
            float cosResult = MathHelper.Cos(ownerRotY);
            float sinResult = MathHelper.Sin(ownerRotY);

            Vector3 forwardDir = new Vector3(ownerForwardDir.X * cosResult + ownerForwardDir.Z * sinResult, 0.0f, ownerForwardDir.X * (-sinResult) + ownerForwardDir.Z * cosResult); // first, calculate the rotated point
            Vector3 impulseVec = forwardDir * 40.0f + new Vector3(0, 5,0);

            onwerBall.GetComponent<Transform>().Position = playerLocation.Position + (forwardDir * 8.0f) + new Vector3(0, 10f, 0);
            rigidBody.AddImpulseForce(ref impulseVec);
            
        }

        public void ResetBall()
        {
            rigidBody.UseGravity = false;
            onwerBall.GetComponent<Transform>().Position = holdPos;
            ownBallCollideScript.collided = false;
        }


    }
}
