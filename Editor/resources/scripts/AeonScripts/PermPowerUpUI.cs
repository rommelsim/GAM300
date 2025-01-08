using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.Diagnostics.Eventing.Reader;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class PermPowerUpUI : AeonBehaviour
    {

        private bool _isRed = false;
        private bool _isBlue = false;

        public bool usedHammer;

        private HammerPowerUp hammerPowerUp=null;
        private BananaPowerUp bananaPowerUp=null;

        private Entity entUI;

        private MoveCharacter move;

        float coolDown;

        bool pressed= false;

        public override void OnCreate()
        {
            
            if (entity.Name == "RedHammerCountdown")
            {
                _isRed = true;
                hammerPowerUp = Entity.FindEntityByName("Player 1").GetScript<HammerPowerUp>();
                bananaPowerUp = Entity.FindEntityByName("Player 1").GetScript<BananaPowerUp>();
                move = Entity.FindEntityByName("Player 1").GetScript<MoveCharacter>();
                entUI = Entity.FindEntityByName("PermRedHammerUI");
            }
            else if (entity.Name == "BlueHammerCountdown")
            {
                _isBlue = true;
                hammerPowerUp = Entity.FindEntityByName("player2").GetScript<HammerPowerUp>();
                bananaPowerUp = Entity.FindEntityByName("player2").GetScript<BananaPowerUp>();
                move = Entity.FindEntityByName("player2").GetScript<MoveCharacter>();
                entUI = Entity.FindEntityByName("PermBlueHammerUI");

            }
        }

        public override void Update()
        {
            if (!move.IsCarryingPickable())
            {
                coolDown = hammerPowerUp.swingCooldown;
                entity.GetComponent<UI>().Text = ((int)coolDown).ToString();

                if (!hammerPowerUp.swinging)
                {
                    entity.GetComponent<Transform>().Scale = Vector3.ZERO;
                    if (entity.Name == "BlueHammerCountdown")
                    {
                        entUI.GetComponent<UI>().Texture = ".\\resources\\textures\\BluePerm(Hammer).dds";
                    }
                    else if (entity.Name == "RedHammerCountdown")
                    {
                        entUI.GetComponent<UI>().Texture = ".\\resources\\textures\\RedPerm(Hammer).dds";
                    }


                }
                else
                {

                    if (move.WasPermKeyPress())
                    {
                        pressed = true;
                    }

                    if (pressed)
                    {
                        if (coolDown > 1.96f)
                        {
                            entUI.GetComponent<Transform>().Scale += new Vector3(1, 1, 0) * Time.DeltaTime * 200f;
                        }
                        else
                        {
                            entUI.GetComponent<Transform>().Scale = new Vector3(100, 100, 0);
                            pressed = false;

                        }
                    }

                    entity.GetComponent<Transform>().Scale = new Vector3(1, 1, 1);
                    if (entity.Name == "BlueHammerCountdown")
                    {
                        entUI.GetComponent<UI>().Texture = ".\\resources\\textures\\BluePerm(NoHammer).dds";
                    }
                    else if (entity.Name == "RedHammerCountdown")
                    {
                        entUI.GetComponent<UI>().Texture = ".\\resources\\textures\\RedPerm(NoHammer).dds";
                    }
                }
            }
            else if (move.IsCarryingPickable())
            {
                coolDown = bananaPowerUp.bananaCooldown;
                entity.GetComponent<UI>().Text = ((int)coolDown).ToString();
                if(coolDown >= 3)
                {
                    if (move.WasPermKeyPress())
                    {
                        pressed = true;
                    }
                    entity.GetComponent<Transform>().Scale = Vector3.ZERO;
                    if (entity.Name == "BlueHammerCountdown")
                    {
                        entUI.GetComponent<UI>().Texture = ".\\resources\\textures\\BluePerm(Banana).dds";
                    }
                    else if (entity.Name == "RedHammerCountdown")
                    {
                        entUI.GetComponent<UI>().Texture = ".\\resources\\textures\\RedPerm(Banana).dds";
                    }
                }
                else
                {
                    if (pressed)
                    {
                        if (coolDown > 2.9f)
                        {
                            entUI.GetComponent<Transform>().Scale += new Vector3(1, 1, 0) * Time.DeltaTime * 200f;
                        }
                        else
                        {
                            entUI.GetComponent<Transform>().Scale = new Vector3(100, 100, 0);
                            pressed = false;

                        }
                    }
                    entity.GetComponent<Transform>().Scale = new Vector3(1, 1, 1);
                    if (entity.Name == "BlueHammerCountdown")
                    {
                        entUI.GetComponent<UI>().Texture = ".\\resources\\textures\\BluePerm(NoBanana).dds";
                    }
                    else if (entity.Name == "RedHammerCountdown")
                    {
                        entUI.GetComponent<UI>().Texture = ".\\resources\\textures\\RedPerm(NoBanana).dds";
                    }
                }
            }


        }





    }
}
