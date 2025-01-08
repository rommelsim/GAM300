using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class HowToPlay : AeonBehaviour
    {
        public UI nextUI = null;

        public static string confirmSelect = "confirm_Select";
        public bool isComplete = false;

        public Entity startEntity = null;
        public UI htpStartUI = null;

        public Entity knockoff = null;
        public UI knockoffUI = null;

        public Entity packedTracks = null;
        public UI packedTracksUI = null;

        public Entity htpNext = null;
        public UI htpNextUI = null;

        // AudioComponent Variable Declaration
        private AudioComponent allButtonPressedSFX = null;
        // To delay 0.5 seconds to play the start audio
        private DateTime lastTime = DateTime.MinValue;
        private bool loadStart = false;
        private bool startPressed = false;
        private float delay = 0f;
        private int seconds = 0;
        private int inner_sec = 0;
        

        private int counter = 0;
        //private bool isLevel2 = false;
        public override void OnCreate()
        {
            // nextUI = HowToPlay
            if (entity != null) nextUI = entity.GetComponent<UI>();

            // start button
            startEntity = Entity.FindEntityByName("HTPStart");
            if (startEntity != null) htpStartUI = startEntity.GetComponent<UI>();

            // knockoff picture
            knockoff = Entity.FindEntityByName("KnockOff");
            if (knockoff != null) knockoffUI = knockoff.GetComponent<UI>();

            // packedtracks picture
            packedTracks = Entity.FindEntityByName("PackedTracks");
            if (packedTracks != null) packedTracksUI = packedTracks.GetComponent<UI>();

            // next button
            htpNext = Entity.FindEntityByName("HTPNext");
            if (htpNext != null) htpNextUI = htpNext.GetComponent<UI>();
            
            Input.RegisterMapping(confirmSelect, Input.KBMInputType.Keyboard, Keys.KEY_ENTER, Input.JoystickInput.Button_X);
            allButtonPressedSFX = entity.GetComponent<AudioComponent>();
        }
        public override void Update()
        {
            if (GameMode.unlockedLevel2 == true)
            {
                nextUI.Texture = ".\\resources\\textures\\PackedTracksHowToPlayBG2.dds";
                packedTracksUI.Opacity = 1.0f;
                knockoffUI.Opacity = 0.0f;
                //GameData.isLevel2 = true;
            }

            delay += Time.DeltaTime;
            seconds = (int)(delay % 60);


            if ((seconds > 1.5) && (GameInputs.IsPlayerGamepadButtonPressed(confirmSelect, true) || GameInputs.IsPlayerGamepadButtonPressed(confirmSelect, false) || Input.GetMappingKeyPressed(confirmSelect)))
            {
                if (GameMode.unlockedLevel2 == true)
                {
                    GameMode.unlockedLevel2 = false;
                }
                if (isComplete == true && !startPressed)

                {
                    if (allButtonPressedSFX != null)
                    {
                        allButtonPressedSFX.PlaySound();
                    }
               
                    startPressed = true;
                }
                else
                {
                    
                    if (counter == 0)
                    {
                        if (allButtonPressedSFX != null)
                        {
                            allButtonPressedSFX.PlaySound();
                        }
                        NativeLog.LogWarn("Int: {0}", counter);
                          
                        nextUI.Texture = ".\\resources\\textures\\PermanentPowerups.dds";
                        knockoffUI.Opacity = 0.0f;
                        packedTracksUI.Opacity = 0.0f;
                        htpNextUI.Opacity = 0.0f;
                        counter++;
                        delay = 0f;
                    }
                    else if (counter == 1)
                    {
                        if (allButtonPressedSFX != null)
                        {
                            allButtonPressedSFX.PlaySound();
                        }

                        nextUI.Texture = ".\\resources\\textures\\CollectablePowerup.dds";
                        //htpStartUI.Opacity = 1.0f;
                        htpNextUI.Opacity = 0.0f;
                        counter++;
                        delay = 0f;
                        //}
                    }

                    if (counter >= 2)
                    {
                        isComplete = true;
                    }
                   
                }

            }
            if (!loadStart && startPressed)
            {
                lastTime = DateTime.Now;
                loadStart = true;
            }
            if (loadStart && (DateTime.Now - lastTime).TotalSeconds >= 0.5f)
            {
               
                if (GameData.isLevel2 == true)
                {
                    SceneManager.LoadScene(GameData.GameLvl2SceneFile, true, true);
                    NativeLog.Log("LEVEL 2 NOW");
                }
                else
                {
                    SceneManager.LoadScene(GameData.GameLvl1SceneFile, true, true);
                }
            }
        }
    }
}
