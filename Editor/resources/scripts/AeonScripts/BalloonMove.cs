using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class BalloonMove:AeonBehaviour
    {
        public Transform balloon=null;
        private Vector3 startPoint=Vector3.ZERO;
        private Vector3 endPoint=Vector3.ZERO;
        public int direction = 1;
        public float speed = 0.5f;
       


        public override void OnCreate()
        {
           balloon=entity.GetComponent<Transform>();
            startPoint = new Vector3(balloon.Position.X, balloon.Position.Y, balloon.Position.Z);
            endPoint = new Vector3(balloon.Position.X, balloon.Position.Y - 10.0f, balloon.Position.Z);
          
        }
        public override void Update()
        {
            if (GameState.IsPaused) return;
            if (balloon==null) { 
                balloon=entity.GetComponent<Transform>();
            }
            Vector3 target = currentMoveTarget();
            balloon.Position=Vector3.Lerp(balloon.Position, target, 2.5f*Time.DeltaTime);

            float distance=(target-(Vector3)balloon.Position).GetMagnitude();

            if (distance <= 0.1f)
            {
                direction *= -1;
            }

        }
        Vector3 currentMoveTarget()
        {
            if (direction == 1f)
            {
                return startPoint;
            }
            else
            {
                return endPoint;
            }
        }
    }

  
}
