using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class TimesUp : AeonBehaviour
    {
        private AudioComponent whistleSFX = null;
        private float _timer = 0f;
        public float maxgametime = 95.0f;

        private bool hasPlayedSound = false;
        private bool hasStopedSound = false;
        private float threshold = 0.1f; // Adjust as needed

        public override void OnCreate()
        {
            whistleSFX = entity.GetComponent<AudioComponent>();
        }
        public override void Update()
        {
            _timer += Time.DeltaTime;

            if (_timer >= maxgametime && _timer < maxgametime + 1.0f && !hasPlayedSound)
            {
                if (whistleSFX != null)
                {
                    NativeLog.Log("We Played Sound!");
                    whistleSFX.PlaySound();
                    hasPlayedSound = true;
                }
            }
            NativeLog.Log("We Played Sound on at 95 sec!");
            NativeLog.Log("Current Time: " + _timer);
            if (_timer >= maxgametime + 1.0f && _timer < maxgametime + 1.0f + threshold && hasPlayedSound && !hasStopedSound)
            {
                if (whistleSFX != null)
                {
                    NativeLog.Log("We Played Sound!");
                    whistleSFX.StopSound();
                    hasStopedSound = true;
                }
            }
            NativeLog.Log("We Stoped Sound on at 96 sec!");
            NativeLog.Log("Current Time: " + _timer);
        }
    }
}
