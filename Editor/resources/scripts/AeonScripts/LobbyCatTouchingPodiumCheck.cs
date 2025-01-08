using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class LobbyCatTouchingPodiumCheck : AeonBehaviour
    {
        public bool touchingPodium = false;

        public override void OnCollisionEnter(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);

            if (otherEnt.Name == "Platform1_Top" || otherEnt.Name == "Platform2_Top")
            {
                touchingPodium = true;
            }
            else
            {
                return;
            }
        }

        public override void OnCollisionExit(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            if (otherEnt.Name == "Platform1_Top" || otherEnt.Name == "Platform2_Top")
            {
                touchingPodium = false;
            }
            else
            {
                return;
            }
        }


    }
}
