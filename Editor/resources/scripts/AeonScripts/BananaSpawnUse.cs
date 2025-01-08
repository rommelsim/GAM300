using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class BananaSpawnUse : AeonBehaviour
    {
        public bool spawned = false;
        public MoveCharacter playerChar = null;

        private bool turnBanana = false;

        public override void OnCreate()
        {
            spawned = true;
            turnBanana = true;
        }

        public override void Update()
        {
            if (spawned)
            {
                Vector3 impulsVec = (10 *  -playerChar.GetFacingDirection()) + new Vector3(0,50,0);
                entity.GetComponent<RigidBody>().AddImpulseForce(ref impulsVec);
                spawned = false;


            }

            if (turnBanana)
            {
                entity.GetComponent<Transform>().Rotation += 10 * new Vector3(playerChar.GetFacingDirection().X, 0, -playerChar.GetFacingDirection().Z);
            }
            else
            {
                entity.GetComponent<Transform>().Rotation = new Vector3(0, 0, 0);
            }
        }

        public override void OnCollisionEnter(uint otherEntityID)
        {
            turnBanana = false;
        }

    }
}
