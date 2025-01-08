using AeonScriptCore;
using AeonScripts;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class ConfettiSplash : AeonBehaviour
    {
        private Vector3 gravity = new Vector3(0, -0.3f, 0);
        private Vector3 vel = Vector3.ZERO;
        private float dt = 0;

        private Transform confettiTrans = null;
        private float randomMass = GameData.random.Next(3,5);
        private float randomAdd = (float)GameData.random.NextDouble() * (1.0f - 0.0f) + 0.0f;

        private float delayTimer = 1.7f;
        private float timer = 0;

        private float burstTimer = 0;
        private float burstDuration = 1f;
        private bool isBursting = false;

        public override void OnCreate()
        {
            if (entity != null)
            {
                confettiTrans = entity.GetComponent<Transform>();
                //randomMass = GameData.random.Next(1, 3);
            }
        }

        public override void Update()
        {
            timer += Time.DeltaTime;
            if (timer >= delayTimer)
            {

                if (isBursting)
                {
                    burstTimer += Time.DeltaTime;
                    if (burstTimer >= burstDuration)
                    {
                        isBursting = false;
                        //burstTimer = 0;
                    }
                }

                if (burstTimer == 0)
                {
                    ApplyBurstForce();
                    isBursting = true;
                }

                Vector3 drop = confettiTrans.Position;
                dt = 0.1f;

                if (!isBursting)
                {
                    // Apply gravity when not bursting
                    vel += gravity * (randomMass + randomAdd) * dt;
                }

                drop += vel * dt;
                confettiTrans.Position = drop;

                if (drop.Y >= 150)
                {
                    Vector3 stop = new Vector3(GameData.random.Next(-20, 50), confettiTrans.Position.Y, confettiTrans.Position.Z);
                    confettiTrans.Position = stop;
                }

                if (drop.Y <= -10)
                {
                    //Entity.Destroy(entity);
                    if (GrandWinnerScreen.grandWinner)
                    {
                        ResetPosition();
                    }
                    else
                    {
                        Entity.Destroy(entity);
                    }
                }

                
            }
        }

        private void ApplyBurstForce()
        {
            // Add a burst force to the current velocity
            vel += new Vector3(
                GameData.random.Next(-5, 5),
                GameData.random.Next(-10, 20),
                0
            );
        }

        private void ResetPosition()
        {
            // Adjust the reset position based on your requirements
            confettiTrans.Position = new Vector3(18f, 10f, 40f);
            vel = Vector3.ZERO; // Reset velocity
            //isBursting = true;
            burstTimer = 0;
        }
    }
}

//namespace AeonScripts
//{
//    public class ConfettiSplash : AeonBehaviour
//    {
//        public Vector3 gravity = new Vector3(0, -0.5f, 0);
//        public Vector3 vel = Vector3.ZERO;

//        public float dt = 0;

//        private int randomMass = 0;
//        private float randomAdd = (float)GameData.random.NextDouble() * (1.0f - 0.0f) + 0.0f;

//        private Transform confettiTrans = null;

//        //private List<Entity> entPoofs = new List<Entity>();
//        //private List<Transform> poofTrans = new List<Transform>();

//        //private bool isConfetti = false;

//        public float delayTimer = 3f;
//        private float timer = 0;
//        public override void OnCreate()
//        {
//            if (entity != null)
//            {
//                confettiTrans = entity.GetComponent<Transform>();
//                randomMass = GameData.random.Next(1, 3);
//            }

//            //for (int i = 0; i <= 9; i++)
//            //{
//            //    Entity ent = Entity.FindEntityByName($"Poof{i}");
//            //    if (ent != null)
//            //    {
//            //        entPoofs.Add(ent);

//            //    }             
//            //}

//            //for (int i = 0; i <= entPoofs.Count; i++)
//            //{
//            //    if (entPoofs[i] != null)
//            //    {
//            //        poofTrans.Add(entPoofs[i].GetComponent<Transform>());
//            //    }
//            //}
//        }
//        public override void Update()
//        {
//            timer += Time.DeltaTime;

//            Vector3 shoot = new Vector3(0, 7f * Time.DeltaTime, 0);

//            //if (timer >= (delayTimer - 0.5f))
//            //{
//                //if (!isConfetti)
//                //{
//                //    for (int i = 0; i < poofTrans.Count; i++)
//                //    {
//                //        poofTrans[i].Position += shoot;
//                //    }
//                //}

//                if (timer >= delayTimer)
//                {
//                    //isConfetti = true;
//                    Vector3 drop = confettiTrans.Position;

//                    dt = 0.1f;

//                    vel += gravity * (randomMass + randomAdd) * dt;
//                    drop += vel * dt;
//                    confettiTrans.Position = drop;

//                    if (drop.Y <= 10)
//                    {
//                        ResetPosition();
//                    }
//                }

//            //}
//        }
//        private void ResetPosition()
//        {
//            // Adjust the reset position based on your requirements
//            confettiTrans.Position = new Vector3(GameData.random.Next(-50, 84), 95f, 25f);
//            vel = Vector3.ZERO; // Reset velocity
//        }
//    }
//}
