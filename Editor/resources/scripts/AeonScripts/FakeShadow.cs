using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class FakeShadow : AeonBehaviour
    {
        public string ownerPlayer;

        public Entity playerEnt = null;

        Transform shadowTransform = null;

        public override void OnCreate()
        {
            
            playerEnt = Entity.FindEntityByName(ownerPlayer);

        }

        public override void Update()
        {
            if (this.entity.Name == "FakeShadowP1")
            {
                this.entity.GetComponent<Transform>().Position = playerEnt.GetScript<MoveCharacter>().shadowPos + new Vector3(0,1.5f,0);
            }
            else if (this.entity.Name == "FakeShadowP2")
            {
                this.entity.GetComponent<Transform>().Position = playerEnt.GetScript<MoveCharacter>().shadowPos + new Vector3(0, 1.5f, 0);
            }
        }


    }
}
