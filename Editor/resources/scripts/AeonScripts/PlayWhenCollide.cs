using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class PlayWhenCollide : AeonBehaviour
    {
        private AudioComponent playsfx = null;
        public override void OnCreate()
        {
            playsfx = entity.GetComponent<AudioComponent>();
        }
        public override void OnCollisionEnter(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            MoveCharacter boxWithPoint = otherEnt.GetScript<MoveCharacter>();
            if (boxWithPoint != null && playsfx != null)
            {

                NativeLog.Log("We Played Sound!");
                //how do we play
                playsfx.PlaySound();
            }
            NativeLog.Log("We Played Sound on collision!");
        }
    }
}
