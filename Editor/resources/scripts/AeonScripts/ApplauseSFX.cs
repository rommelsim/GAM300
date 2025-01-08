using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class ApplauseSFX : AeonBehaviour
    {
        private AudioComponent Applause = null;
        private float _timer = 0f;
        public float trigger_at = 0.0f;
        public float trigger_time = 3.0f;

        private bool hasPlayedSound = false;
        private bool hasStopedSound = false;
        private float threshold = 0.1f; // Adjust as needed
        public override void OnCreate()
        {
            Applause = entity.GetComponent<AudioComponent>();
        }
        public override void Update()
        {
            _timer += Time.DeltaTime;

            if (_timer >= trigger_at && _timer < trigger_at + trigger_time && !hasPlayedSound)
            {
                if (Applause != null)
                {
                    NativeLog.Log("We Played Sound Applause SFX!");
                    Applause.FadeIn(3.0f);
                    Applause.PlaySound();
                    hasPlayedSound = true;
                }
            }
            if (_timer >= trigger_at + trigger_time && _timer < trigger_at + trigger_time + threshold && hasPlayedSound && !hasStopedSound)
            {
                if (Applause != null)
                {
                    NativeLog.Log("We Played Sound!");
                    Applause.FadeOut(3.0f);
                    //Applause.StopSound();
                    hasStopedSound = true;
                }
            }
        }
    }
}
