using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class LightPulsingEffect : AeonBehaviour
    {
        Lighting entLight = null;

        public float brightTime = 0.5f;
        public float dimTime = 0.7f;

        private bool startDim = false;
        private bool startBright = false;

        private float brightness = 1f;
        public override void OnCreate()
        {
            entLight = this.entity.GetComponent<Lighting>();
            startBright = true;
        }

        public override void Update()
        {
            if (startBright)
            {
                if(brightTime >= 0)
                {
                    brightTime -= Time.DeltaTime;
                    brightness += ((dimTime * 10) * Time.DeltaTime);
                    entLight.Intensity = (int)brightness;
                }
                else if (brightTime <= 0)
                {
                    startBright = false;
                    startDim = true;
                    brightTime = 1f;
                }

            }
            else
            {
                if (startDim)
                {
                    if (dimTime >= 0)
                    {
                        dimTime -= Time.DeltaTime;
                        brightness -= ((brightTime * 10) * Time.DeltaTime);
                        entLight.Intensity = (int)brightness;
                    }
                    else if (dimTime <= 0)
                    {
                        startBright = true;
                        startDim = false;
                        dimTime = 3f;
                    }
                }
            }


        }

    }
}
