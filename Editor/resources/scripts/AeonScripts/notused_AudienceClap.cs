using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class notused_AudienceClap : AeonBehaviour
    {
        private AudioComponent Applause = null;
        private float _timer = 0.0f;
        //public float volumeDecrement = 0.01f; // Adjust the decrement value as needed
        private bool hasPlayedSound = false;
        private bool hasStopedSound = false;
        private bool hasSpaceTriggeredX1 = false;
        public override void OnCreate()
        {
            Applause = entity.GetComponent<AudioComponent>();
        }
        public override void Update()
        {
            _timer += Time.DeltaTime;
            if (!hasPlayedSound && _timer >= 0.2 && _timer <= 0.4)
            {
                if (Applause != null)
                {
                    NativeLog.Log("We Played Applause!");
                    Applause.PlaySound();
                    hasPlayedSound = true;
                    NativeLog.Log("We Just Played Applause!");
                }
            }
            //if (hasPlayedSound && _timer > _timer) fkkkkkkkkkkkkkkkkkk need fade function
            //{
            //    // Decrement volume per frame
            //    if (Applause != null && Applause.Volume > 0.0f)
            //    {
            //        Applause.Volume -= volumeDecrement;

            //        NativeLog.Log("Current Applause Volume: " + Applause.Volume);
            //    }
            //}

            if (!hasStopedSound)
            {
                if (Input.GetKeyPressed(Keys.KEY_SPACE) && !hasSpaceTriggeredX1)
                {
                    // Stop the sound and mark it as stopped
                    if (Applause != null)
                    {
                        Applause.StopSound();
                        hasStopedSound = true;
                        hasSpaceTriggeredX1 = true;
                        NativeLog.Log("We Stopped Applause!");
                    }
                }
                //else if (_timer >= 2.9 && _timer <= 3.0) did not bother to stop at _timeing as audio won't fade
                //{
                //    // Stop the sound and mark it as stopped
                //    if (Applause != null)
                //    {
                //        Applause.StopSound();
                //        hasStopedSound = true;
                //        NativeLog.Log("We Stopped Applause!");
                //    }
                //}

            }
        }
    }
}