using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class HereAreTheRulesSFX : AeonBehaviour
    {
        private AudioComponent hereAreTheRules = null;

        private float _timer = 0.0f;

        private bool hasSpaceTriggeredX1 = false;
        private bool hasPlayedSound = false;

        public override void OnCreate()
        {
            hereAreTheRules = entity.GetComponent<AudioComponent>();
            hereAreTheRules.PlaySound();
        }
        /*public override void Update()
        {
            if (!hasPlayedSound)
            {
                if (Input.GetKeyPressed(Keys.KEY_SPACE) && !hasSpaceTriggeredX1)
                {
                    NativeLog.Log("We Go Rommel Voice!");
                    hasSpaceTriggeredX1 = true;
                    hereAreTheRules.PlaySound();
                    hasPlayedSound = true;
                }
                //if (hasSpaceTriggeredX1)
                //{
                //    if (hereAreTheRules != null)
                //    {
                //        NativeLog.Log("Current Time: " + _timer);
                //        _timer = Time.DeltaTime;
                //        if (_timer >= 0.04f && _timer <= 0.1)
                //        {
                //            NativeLog.Log("We Played Rommel Voice!");
                //            hereAreTheRules.PlaySound();
                //            hasPlayedSound = true;
                //        }
                //    }
                //}
            }
            
        }*/
    }
}
