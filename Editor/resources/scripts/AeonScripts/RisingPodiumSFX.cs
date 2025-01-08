using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    internal class RisingPodiumSFX : AeonBehaviour
    {
        private AudioComponent risingaudio = null;
        private bool playDrum;
        private float _timer = 0.0f;

        public override void OnCreate()
        {
            risingaudio = entity.GetComponent<AudioComponent>();
            if (risingaudio != null)
                risingaudio.PlaySound();
        }

        public override void Update()
        {
            //_timer += Time.DeltaTime;
            //if (!playDrum)
            //{
            //    if (risingaudio != null)
            //    {
            //        risingaudio.PlaySound();
            //    }
            //}
        }
    }
}
