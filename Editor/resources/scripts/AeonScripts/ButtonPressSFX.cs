using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class ButtonPressSFX : AeonBehaviour
    {
        private AudioComponent sfxAllButtons = null;
        private float lastPressTime = 0f;
        private float cooldownDuration = 1.0f;

        public override void OnCreate()
        {
            sfxAllButtons = entity.GetComponent<AudioComponent>();
        }
        public override void Update()
        {
            if (sfxAllButtons != null)
            {
                // Check for key presses and play the sound
                if ((Input.GetKeyPressed(Keys.KEY_W) ||
                    Input.GetKeyPressed(Keys.KEY_A) ||
                    Input.GetKeyPressed(Keys.KEY_S) ||
                    Input.GetKeyPressed(Keys.KEY_D) ||
                    Input.GetKeyPressed(Keys.KEY_SPACE)) &&
                    (Time.ElapsedTime - lastPressTime >= cooldownDuration))
                {
                    // Generate a random pitch between 0.8f and 1.2f
                    float randomPitch = (float)new Random().NextDouble() * 0.2f + 1.0f;

                    // Set the pitch and play the sound
                    sfxAllButtons.Pitch = randomPitch;
                    sfxAllButtons.PlaySound();

                    // Update the last press time
                    lastPressTime = Time.ElapsedTime;
                }
            }
        }
    }
}
