using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class DeathFly :AeonBehaviour
    {
        public Transform balloonTrans = null;
        private float time = 0;
        private Vector3 endPoint = Vector3.ZERO;

        public override void OnCreate()
        {
            if (entity != null)
            {
                balloonTrans= entity.GetComponent<Transform>();
                endPoint = new Vector3(balloonTrans.Position.X, 100.0f, balloonTrans.Position.Z);
            }
        }
        public override void Update()
        {
            if (GameState.IsPaused) return;
            balloonTrans.Position = Vector3.Lerp(balloonTrans.Position, endPoint, 1.0f * Time.DeltaTime);
            if (balloonTrans.Position.Y >= 70.0f)
            {
                Entity.Destroy(entity);
            }
        }
    }
}
