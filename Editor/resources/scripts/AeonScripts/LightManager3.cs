using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{

    //Indicate player respawn 
    /*
        1. Light Should follow player transform and player color 
        2. To identify the respective checkpoint position
        3. Pulsing effect of the light 
        4. Effect to last for about 3 seconds
     */
    public class LightManager3 : AeonBehaviour
    {
        //Need to get boolean from the respawn 
        Entity abyss = null;
        Entity box = null;
        RespawnZoneTrigger level1 = null;
        Lighting respawn = null;
        Vector3 lightPos = Vector3.ZERO;

        //Timer
        private float lightTimer;
        private bool lightStarted = false;

        //Level2
        Level2.RespawnZoneTriggerL2 level2 = null;
        public override void OnCreate()
        {
            abyss = Entity.FindEntityByName("Abyss.Trigger");
            box = Entity.FindEntityByName("Box");
            if (abyss != null)
            {
                level1 = abyss.GetScript<RespawnZoneTrigger>();
                if (Entity.FindEntityByName("Train") != null)
                {
                    level2 = abyss.GetScript<Level2.RespawnZoneTriggerL2>();
                }
            }
        }
        public override void Update()
        {

            if (!lightStarted)
            {
                lightStarted = true;
                lightTimer = 3.0f;
            }

            if (lightTimer > 0)
            {
                if (level1 != null)
                {
                    if (level1.boxrespawn)
                    {

                        lightTimer -= Time.DeltaTime;
                        lightPos = new Vector3(box.GetComponent<Transform>().Position.X, box.GetComponent<Transform>().Position.Y + 20.0f, box.GetComponent<Transform>().Position.Z);
                        entity.GetComponent<Transform>().Position = lightPos;
                        respawn = entity.GetComponent<Lighting>();
                        respawn.Intensity = Mathf.Lerp(1.0f, 50.0f, lightTimer);
                    }
                }
                if (level2 != null)
                {
                    if (level2.boxrespawn2)
                    {
                        lightTimer -= Time.DeltaTime;
                        lightPos = new Vector3(box.GetComponent<Transform>().Position.X, box.GetComponent<Transform>().Position.Y + 20.0f, box.GetComponent<Transform>().Position.Z);
                        entity.GetComponent<Transform>().Position = lightPos;
                        respawn = entity.GetComponent<Lighting>();
                        respawn.Intensity = Mathf.Lerp(1.0f, 50.0f, lightTimer);
                    }
                }
            }
            else
            {
                lightStarted = false;
                if (level1 != null)
                    level1.boxrespawn = false;
                if (level2 != null)
                    level2.boxrespawn2 = false;
                respawn.Intensity = 2.0f;
            }
        }


    }
}
