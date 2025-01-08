using AeonScriptCore;
using System;

namespace AeonScripts
{
    public class LookAtCamera : AeonBehaviour
    {
        private Transform transformComp = null;
        private Entity camera = null;
        private Transform camTransformComp = null;

        public override void OnCreate()
        {
            transformComp = entity.GetComponent<Transform>();

            camera = Entity.FindEntityByName("Camera");
            camTransformComp = camera.GetComponent<Transform>();
        }

        public override void Update()
        {
            Vector3 direction = camTransformComp.Position - transformComp.Position;

            transformComp.Rotation = CalculateRotationAngles(direction);
        }

        public Vector3 CalculateRotationAngles(Vector3 direction)
        {
            float angleY = (float)Math.Atan2(direction.X, direction.Z) * (180f / (float)Math.PI);

            return new Vector3(0f, angleY, 0f);
        }
    }
}
