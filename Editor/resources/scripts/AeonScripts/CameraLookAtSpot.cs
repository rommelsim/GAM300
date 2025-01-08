using AeonScriptCore;

namespace AeonScripts
{
    public class CameraLookAtSpot : AeonBehaviour
    {
        public Vector3 CameraPosition = Vector3.ZERO;
        public Vector3 LookAtPosition = Vector3.ZERO;
        public float CamYaw = 0.0f;
        public float CamPitch = 0.0f;
        public float CamZoom = 40.0f;
        
        private Camera camComp = null;

        public override void OnCreate()
        {
            camComp = entity.GetComponent<Camera>();
            Graphics.RequestSwitchCamera(entity.Id);
        }

        public override void Update()
        {
            if (camComp == null)
            {
                camComp = entity.GetComponent<Camera>();
                return;
            }

            camComp.Zoom = CamZoom;
            camComp.Yaw = CamYaw;
            camComp.Pitch = CamPitch;
            camComp.Position = CameraPosition;
            camComp.Target = LookAtPosition;
        }
    }
}
