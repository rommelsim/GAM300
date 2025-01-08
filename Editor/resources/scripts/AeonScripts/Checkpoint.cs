using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class Checkpoint :AeonBehaviour
    {
        public  string checkpoint = ""; //Can define which is first checkpoint
        private Collider checkPointZone = null;
        private Entity initialCheckPointCheck = null;
        private Collider initialCheckPoint = null;

        public bool checkPointEnabled = false;

        /*Two Types of Condition:
            - If previous checkpoint not found then, new checkpoint will be enabled 
            - Else once a cat leave the second checkpoint, that checkpoint becomes enabled
        */
        public override void OnCreate()
        {

            checkPointZone=entity.GetComponent<Collider>();
            initialCheckPointCheck = Entity.FindEntityByName(checkpoint);
            if (initialCheckPointCheck != null)
            {
                initialCheckPoint=initialCheckPointCheck.GetComponent<Collider>();
            }
        }

        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);

            checkPointEnabled = true;
        }

        public override void Update()
        {
            initialCheckPointCheck = Entity.FindEntityByName(checkpoint);
            //Check
            if (checkPointZone == null )
            {
                checkPointZone= entity.GetComponent<Collider>();
                return;
            }

            if (initialCheckPointCheck == null || initialCheckPointCheck.GetComponent<Transform>().Position.Y< 0)
            {
                checkPointEnabled=true;
            }
            if (checkPointEnabled)
            {
                checkpoint = entity.Name;
            }
            else
            {
                //NativeLog.Log("CheckpoinT:{0}",checkpoint);

            }
        }

        public string GetCheckpointName()
        {
            return checkpoint;
        }
    }
}
