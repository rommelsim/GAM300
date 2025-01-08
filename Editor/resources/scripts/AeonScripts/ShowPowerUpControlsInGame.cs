using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class ShowPowerUpControlsInGame : AeonBehaviour
    {
        public bool hasPowerUp = false;
        Transform thisEntTrans = null;

        PlayerInventory playerInventory = null;

        public override void OnCreate()
        {
            if (this.entity.Name == "uiEntityChild")
            {
                playerInventory = Entity.FindEntityByName("Player 1").GetScript<PlayerInventory>();
            }
            else if (this.entity.Name == "uiEntity2Child")
            {
                playerInventory = Entity.FindEntityByName("player2").GetScript<PlayerInventory>();
            }
            thisEntTrans = this.entity.GetComponent<Transform>();

        }


        public override void Update()
        {
            hasPowerUp = playerInventory.HoldingPowerUp;

            if (hasPowerUp)
            {
                //stretch open UI based on time.
                if (this.entity.Name == "uiEntityChild")
                {
                    if (thisEntTrans.Scale.X < 400)
                    {
                        thisEntTrans.Scale += new Vector3(200, 0, 0) * Time.DeltaTime * 6;
                        thisEntTrans.Position += new Vector3(100, 0, 0) * Time.DeltaTime * 6;

                    }
                    else
                    {
                        thisEntTrans.Scale = new Vector3(400, 200, 0);
                        thisEntTrans.Position = new Vector3(-610, 360, 0);
                    }
                }
                else if (this.entity.Name == "uiEntity2Child")
                {
                    if (thisEntTrans.Scale.X < 400)
                    {
                        thisEntTrans.Scale += new Vector3(200, 0, 0) * Time.DeltaTime * 6;
                        thisEntTrans.Position += new Vector3(-100, 0, 0) * Time.DeltaTime * 6;

                    }
                    else
                    {
                        thisEntTrans.Scale = new Vector3(400, 200, 0);
                        thisEntTrans.Position = new Vector3(610, 360, 0);
                    }
                }




            }
            else
            {
                if (this.entity.Name == "uiEntityChild")
                {
                    if (thisEntTrans.Scale.X > 0)
                    {
                        thisEntTrans.Scale += new Vector3(-200, 0, 0) * Time.DeltaTime * 6;
                        thisEntTrans.Position += new Vector3(-100, 0, 0) * Time.DeltaTime * 6;

                    }
                    else
                    {
                        thisEntTrans.Scale = new Vector3(0, 200, 0);
                        thisEntTrans.Position = new Vector3(-810, 360, 0);
                    }
                }
                else if (this.entity.Name == "uiEntity2Child")
                {
                    if (thisEntTrans.Scale.X > 0)
                    {
                        thisEntTrans.Scale += new Vector3(-200, 0, 0) * Time.DeltaTime * 6;
                        thisEntTrans.Position += new Vector3(100, 0, 0) * Time.DeltaTime * 6;

                    }
                    else
                    {
                        thisEntTrans.Scale = new Vector3(0, 200, 0);
                        thisEntTrans.Position = new Vector3(810, 360, 0);
                    }
                }
                
            }

        }

    }
}
