using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class Level2PlatformSpawn : AeonBehaviour
    {
        Vector3 originalPos;
        Transform entTransform = null;
        Transform behindCam = null;

        bool dropPlatform = false;
        bool dropDropPlatform = false;

        public override void OnCreate()
        {
            originalPos = entity.GetComponent<Transform>().Position;
            entTransform = entity.GetComponent<Transform>();
            behindCam = Entity.FindEntityByName("BehindCam").GetComponent<Transform>();

            entTransform.Position += new Vector3(0, 300, 0);
        }


        public override void Update()
        {
            if (GameState.IsPaused)
            {
                return;
            }
            else
            {
                if (entTransform.Position.Z - behindCam.Position.Z <= 170)
                {
                    dropPlatform = true;
                }

                if (entTransform.Position.Z - behindCam.Position.Z <= 50f && entTransform.Position.Z - behindCam.Position.Z > 36.5f)
                {
                    int rando = GameData.random.Next(-1, 3);
                    float randoInt = 0;
                    if (rando == -1 || rando == 0)
                    {
                        randoInt = -1;
                    }
                    else if (rando == 1 || rando == 2)
                    {
                        randoInt = 1;
                    }

                    entTransform.Position += new Vector3(randoInt * (float)(Math.Sin(200f * Time.DeltaTime) * 0.3f), 0, randoInt * (float)(Math.Sin(180f * Time.DeltaTime) * 0.5f));

                    //entTransform.Position = new Vector3(
                    //    entTransform.Position.X - (float)(Math.Sin(Time.DeltaTime) * 10f), entTransform.Position.Y, 
                    //    entTransform.Position.Z - (float)(Math.Sin(Time.DeltaTime) * 10f));
                }

                if (entTransform.Position.Z - behindCam.Position.Z <= 35f)
                {
                    dropDropPlatform = true;
                }

                if (dropPlatform && !dropDropPlatform)
                {
                    entTransform.Position = Vector3.Lerp(entTransform.Position, originalPos, 0.1f);
                }

                if (dropDropPlatform && dropPlatform)
                {
                    entTransform.Position -= new Vector3(0, 90, 0) * (1.5f * Time.DeltaTime);
                }


                if (entity.GetComponent<Transform>().Position.Y <= -200f && dropPlatform && dropDropPlatform)
                {
                    Entity.Destroy(this.entity);
                }

            }

        }

    }
}
