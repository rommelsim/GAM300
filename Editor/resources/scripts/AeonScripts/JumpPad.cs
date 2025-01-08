using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class JumpPad : AeonBehaviour
    {
        public float boostForce;
        private bool boost = false;
        RigidBody colliderRb = null;

        MoveCharacter player1 = null;
        MoveCharacter player2 = null;

        AudioComponent audioComp = null;

        Entity selfEnt = null;

        bool playAnim = false;

        private float animStart = 0.2f;
        public override void OnCreate()
        {
            selfEnt = this.entity;
            audioComp = entity.GetComponent<AudioComponent>();
        }

        public override void Update()
        {
            if (player1 != null)
            {
                colliderRb = player1.GetRigidbody();

                Vector3 boostVec = Vector3.ZERO;
                boostVec.Y = boostForce;

                colliderRb.AddImpulseForce(ref boostVec);
                player1 = null;

            }
            if (player2 != null)
            {
                colliderRb = player2.GetRigidbody();

                Vector3 boostVec = Vector3.ZERO;
                boostVec.Y = boostForce;
                NativeLog.Log("Boosting");
                colliderRb.AddImpulseForce(ref boostVec);
                player2 = null;
            }
            if (GameState.IsPaused) return;
            if (playAnim)
            {
                animStart -= Time.DeltaTime;
                if (animStart > 0f)
                {
                    selfEnt.GetComponent<Transform>().Scale += new Vector3(2,4,2) * (5*Time.DeltaTime);
                }
                else
                {
                    selfEnt.GetComponent<Transform>().Scale = new Vector3(10, 10, 10);
                    playAnim = false;
                    animStart = 0.2f;
                }


            }

        }

        public override void OnCollisionEnter(uint triggerEntityID)
        {
            Entity collidedEntity = null;
            collidedEntity = Entity.GetEntity(triggerEntityID);
            MoveCharacter collidedEntityMoveChar = collidedEntity.GetScript<MoveCharacter>();
            
            if (collidedEntityMoveChar!= null)
            {
                if (collidedEntityMoveChar.isPlayerOne)
                {
                    player1 = collidedEntityMoveChar;
                }
                else
                {
                    player2 = collidedEntityMoveChar;
                }
                playAnim = true;
                PlayJumpPadEnter();
            }
        }

        public void PlayJumpPadEnter()
        {
            if (audioComp != null)
            {
                int randNum = GameData.random.Next(2, 9);
                audioComp.SoundFilepath = $".\\resources\\audios\\Bounce0{randNum}.wav";
                NativeLog.Log("Playing Bounce{0}", randNum);
                audioComp.ChannelType = Audio.ChannelType.SFX;
                audioComp.Volume = 0.6f;
                audioComp.PlaySound();
            }
        }
    }

}
