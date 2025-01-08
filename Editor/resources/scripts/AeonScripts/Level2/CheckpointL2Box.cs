using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts.Level2
{
    /*
    1. Each player have its own checkpoint ()
    2. Enable checkpoint once they trigger next 
    3. Respawn to the correct checkpoint
 */
    public class CheckpointL2Box : AeonBehaviour
    {
        private Entity checkpoint = null;
        private Transform checkPointTransform = null;

        private Transform transform = null;

        public override void OnCreate()
        {
            transform = entity.GetComponent<Transform>();

            checkpoint = Entity.FindEntityByName("Checkpoint2");
            if (checkpoint != null)
            {
                checkPointTransform = checkpoint.GetComponent<Transform>();
            }
        }

        public override void Update()
        {
            transform.Position = new Vector3(transform.Position.X, transform.Position.Y, checkPointTransform.Position.Z + 32f);
        }
    }
}