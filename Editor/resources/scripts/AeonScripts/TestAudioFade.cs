using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class TestAudioFade : AeonBehaviour
    {
        private AudioComponent fadeTest = null;
        private float fadeDuration = 5.0f;

        //bool
        private bool fadeIn = false;
        private bool fadeOut = false;
        private float currentFadeTime = 0.0f;

        public override void OnCreate()
        {
            fadeTest = entity.GetComponent<AudioComponent>();
        }

        // Basic linear interpolation function
        private float Lerp(float a, float b, float t)
        {
            return a + (b - a) * t;
        }

        public override void Update()
        {
            // Check if fading in
            if (fadeIn)
            {
                currentFadeTime += Time.DeltaTime;

                if (currentFadeTime < fadeDuration)
                {
                    // Gradually increase volume during the fade
                    float normalizedTime = currentFadeTime / fadeDuration;
                    fadeTest.Volume = Lerp(0.0f, 1.0f, normalizedTime);
                }
                else
                {
                    fadeIn = false;
                }
            }

            // Check if fading out
            if (fadeOut)
            {
                currentFadeTime += Time.DeltaTime;

                if (currentFadeTime < fadeDuration)
                {
                    // Gradually decrease volume during the fade
                    float normalizedTime = currentFadeTime / fadeDuration;
                    fadeTest.Volume = Lerp(1.0f, 0.0f, normalizedTime);
                }
                else
                {
                    fadeOut = false;
                    fadeTest.StopSound();
                }
            }

            // Check for key press to trigger fade
            if (Input.GetKeyPressed(Keys.KEY_F))
            {
                // Start fading in
                fadeIn = true;
                fadeOut = false;
                currentFadeTime = 0.0f;

                // Optionally, play the sound when starting the fade
                fadeTest.PlaySound();
            }

            // Check for key press to trigger fade out
            if (Input.GetKeyPressed(Keys.KEY_G))
            {
                // Start fading out
                fadeIn = false;
                fadeOut = true;
                currentFadeTime = 0.0f;
            }
        }
    }
}
