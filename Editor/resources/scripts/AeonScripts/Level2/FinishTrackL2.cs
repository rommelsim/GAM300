using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using static AeonScripts.GameData;

namespace AeonScripts.Level2
{
    //Celebration
    // confetti , jumping , pulsing 

    public class FinishTrackL2 : AeonBehaviour
    {

        public int bonusPoints = 0;
        private Entity finishTrigger = null;
        [HideFromEditor]
        public bool player1win = false;
        public bool player2win = false;
        Entity player1 = null;
        Entity player2 = null;
        Transform player1startpos = null;
        Transform player2startpos = null;
        public int direction = 1;
        Vector3 player1endpos = Vector3.ZERO;
        Vector3 player2endpos = Vector3.ZERO;
        private float celebrationTimer = 5.0f; // 5 seconds timer
        private bool celebrationStarted = false;
        private bool buildOnce = false;

        //Particles
        public float particleLife = 1.0f;
        public Vector3 gravity = new Vector3(0.0f, 9.8f, 0.0f);
        public Vector3 minVelocity = new Vector3(-6.0f, -1.0f, 1.0f);
        public Vector3 maxVelocity = new Vector3(5.0f, 5.0f, 5.0f);
        public override void OnCreate()
        {
            //Find finish trigger
            finishTrigger = Entity.FindEntityByName("Finish.Trigger");
            player1 = Entity.FindEntityByName("Player 1");
            player2 = Entity.FindEntityByName("player2");
        }

        public override void Update()
        {
            player1startpos = player1.GetComponent<Transform>();
            player2startpos = player2.GetComponent<Transform>();
            player1endpos = new Vector3(player1startpos.Position.X, player1startpos.Position.Y + 40.0f, player1startpos.Position.Z);
            player2endpos = new Vector3(player2startpos.Position.X, player2startpos.Position.Y + 40.0f, player2startpos.Position.Z);

            if (GameState.IsPaused) return;
            if (player1win || player2win)
            {
                if (!celebrationStarted)
                {
                    celebrationStarted = true;
                    celebrationTimer = 5.0f; // Set timer to 5 seconds
                }

                if (celebrationTimer > 0)
                {
                    celebrationTimer -= Time.DeltaTime;

                    if (player1win)
                    {
                        Vector3 target = currentMoveTarget();
                        player1.GetComponent<Transform>().Position = Vector3.Lerp(player1startpos.Position, target, 0.5f * Time.DeltaTime);

                        float distance = (target - (Vector3)player1startpos.Position).GetMagnitude();

                        if (distance <= 0.1f)
                        {
                            direction *= -1;
                        }
                        buildOnce= true;
                        if (buildOnce)
                            buildConfetti(player1.GetComponent<Transform>());
                        
                        Entity finishTrigger = Entity.FindEntityByName("Finish.Trigger");
                        Entity.Destroy(finishTrigger);
                    }
                    else if (player2win)
                    {
                        Vector3 target = currentMoveTarget();
                        player2.GetComponent<Transform>().Position = Vector3.Lerp(player2startpos.Position, target, 0.5f * Time.DeltaTime);

                        float distance = (target - (Vector3)player2startpos.Position).GetMagnitude();

                        if (distance <= 0.1f)
                        {
                            direction *= -1;
                        }
                        buildOnce = true;
                        if (buildOnce)
                             buildConfetti(player2.GetComponent<Transform>());
                        
                        Entity finishTrigger = Entity.FindEntityByName("Finish.Trigger");
                        Entity.Destroy(finishTrigger);
                    }
                }
                else
                {
                    // Celebration time is over, reset flags
                    celebrationStarted = false;
                    GameData.ShowWinnerScreen();
                }
            }
        }
        Vector3 currentMoveTarget()
        {
            if (direction == 1f)
            {
                if (player1win)
                {
                    return player1startpos.Position;
                }
                else if (player2win)
                {
                    return player2startpos.Position;
                }
            }
            else
            {
                if (player1win)
                {
                    return player1endpos;
                }
                else if (player2win)
                {
                    return player2endpos;
                }

            }
            return Vector3.ONE;
        }

        void buildConfetti(Transform player)
        {
            //Build confetti 
            EntityBuilder entBuilder = new EntityBuilder("Celebration");
            TransformBuilder transformBuilder = new TransformBuilder();
            ParticlesBuilder particlesBuilder = new ParticlesBuilder(5, 1);

            //Transform following player 
            //player position to build where the confetti is 
            transformBuilder.SetPosition(new Vector3(player.Position.X,player.Position.Y+10.0f,player.Position.Z));
            transformBuilder.SetRotation(player.Rotation);
            //set scale as random
            Vector3 scale = new Vector3((float)GameData.random.NextDouble(), (float)GameData.random.NextDouble(), (float)GameData.random.NextDouble());
            transformBuilder.SetScale(scale);

            //Particle should set model to prob like "pointbox.bin"
            particlesBuilder.SetModelPath(".\\resources\\objects\\PointBox.bin");
            //then set texture 
            if(player1win)
            particlesBuilder.SetDiffuseTexture(".\\resources\\textures\\PlatformRed_initialShadingGroup_BaseColor.1001.dds");
            if(player2win)
            particlesBuilder.SetDiffuseTexture(".\\resources\\textures\\Platform_initialShadingGroup_BaseColor.1001.dds");
            //Max particles should be 30 the life is 10 
            particlesBuilder.SetParticleLife(particleLife);
            particlesBuilder.SetGravity(gravity);
            particlesBuilder.SetMinVelocity(minVelocity);
            particlesBuilder.SetMaxVelocity(maxVelocity);
            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(particlesBuilder);

            Entity.Create(entBuilder);
            NativeLog.Log("Build Successful");
            buildOnce = false;
        }
    }

}
