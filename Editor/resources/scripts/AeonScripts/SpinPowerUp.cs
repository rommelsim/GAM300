using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{

    /*
        1. Make object spin along with the collider (done)
        2. Ensure object collide with character then character drop box (done)
        3. Ability to collect power up 
        4. Power up random spawn 
        5. QOL (Audio, Animation)
     */

    public class SpinPowerUp : AeonBehaviour
    {
        private float pi = 3.1415926535897931f;

        public Transform transformComp = null;
        private Vector3 startSpin = Vector3.ZERO;
        private Vector3 endSpin = Vector3.ZERO;
        public float spinSpeed = 180.0f;

        public Transform tail1Trans = null;

        private Entity player1 = null;
        private Entity player2 = null;

        private Transform p1Transform = null;
        private Transform p2Transform = null;

        private Vector3 p1KnockOffset = Vector3.ZERO;
        private Vector3 p2KnockOffset = Vector3.ZERO;

        private bool knock1 = false;
        private bool knock2 = false;

        private AudioComponent audioComp = null;

        private Vector3 knockedPositionP1 = Vector3.ZERO;
        private Vector3 knockedPositionP2 = Vector3.ZERO;

        private float knockTimer = 1f;
        // private Vector3 knockEndPosition = Vector3.ZERO;

        public override void OnCreate()
        {
            //Get object transforem 
            transformComp = entity.GetComponent<Transform>();
            //default their rotation
            startSpin = transformComp.Rotation;
            endSpin = startSpin;
            //find 4 objects
            player1 = Entity.FindEntityByName("Player 1");
            player2 = Entity.FindEntityByName("player2");

            p1Transform = player1.GetComponent<Transform>();
            p2Transform = player2.GetComponent<Transform>();

            audioComp = entity.GetComponent<AudioComponent>();
        }

        public override void OnCollisionEnter(uint otherEntityID)
        {
            // Check if otherEntityID is player1 or player2
            Entity colliisonName = Entity.GetEntity(otherEntityID);
            string name = colliisonName.Name;
            MoveCharacter movey = colliisonName.GetScript<MoveCharacter>();

            if (movey != null)
            {
                int randNum = GameData.random.Next(1, 4);

                if (movey.isPlayerOne)
                {

                    if (audioComp != null)
                    {
                        audioComp.SoundFilepath = $".\\resources\\audios\\Red Cat_Slip_{randNum}.wav";
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        audioComp.Volume = 0.6f;
                        audioComp.PlaySound();
                    }
                    Vector3 playerPos = p1Transform.Position;
                    p1KnockOffset = Vector3.Normalize(playerPos - transformComp.Position) * 50.0f;
                    p1KnockOffset.Y = 10.0f;
                    p1KnockOffset += playerPos;
                    knock1 = true;

                }
                else
                {
                    knockedPositionP2 = player2.GetComponent<Transform>().Position;
                    if (audioComp != null)
                    {
                        audioComp.SoundFilepath = $".\\resources\\audios\\Blue Cat_Slip_{randNum}.wav";
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        audioComp.Volume = 0.6f;
                        audioComp.PlaySound();
                    }
                    knock2 = true;
                }
            };
        }
        public override void Update()
        {
            if (GameState.IsPaused) return;
            //check for entity 
            if (transformComp == null)
            {
                transformComp = entity.GetComponent<Transform>();
            }

            if (endSpin.Y >= 360.0f || endSpin.Y <= 0.0f)
            {
                endSpin = startSpin;
            }
            float rotationAmount = spinSpeed * Time.DeltaTime;
            endSpin += new Vector3(0.0f, rotationAmount, 0.0f);
            transformComp.Rotation = endSpin;

            if (knock1)
            {
                p1Transform.Position = Vector3.Lerp(p1Transform.Position, p1KnockOffset, 1.0f * Time.DeltaTime);

                knockTimer -= Time.DeltaTime;

                if(p1Transform.Position.Y<=0.0f)
                {
                    knock1 = false;
                }
                if (knockTimer <= 0)
                {
                    knock1 = false;
                    knockTimer = 1f;
                }
            }

            if (knock2)
            {
                Vector3 knockEndPosition = knockedPositionP2 + new Vector3(100.0f, 10.0f, 50.0f);         
                p2Transform.Position = Vector3.Lerp(p2Transform.Position, knockEndPosition, 1.0f * Time.DeltaTime);

                knockTimer -= Time.DeltaTime;

                if (p2Transform.Position.Y <= 0.0f)
                {
                    knock2 = false;
                }
                if (knockTimer <= 0)
                {
                    knock1 = false;
                    knockTimer = 1f;
                }
            }

        }

        
    }
    }
