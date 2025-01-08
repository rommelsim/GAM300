using AeonScriptCore;
using System.Threading;

namespace AeonScripts
{
    public class CameraLookAtPlayers : AeonBehaviour
    {
        public string PlayerOneName = "";
        public string PlayerTwoName = "";
        public Vector3 CameraPosition = Vector3.ZERO;
        public float CamYaw = 0.0f;
        public float CamPitch = 0.0f;
        public float CamZoom = 40.0f;
        
        private Camera camComp = null;
        private Entity entP1 = null;
        private Entity entP2 = null;
        private Transform transformP1 = null;
        private Transform transformP2 = null;
        public static int cam = 0;

        private float _timer = 0.0f;

        public override void OnCreate()
        {
            camComp = entity.GetComponent<Camera>();

            entP1 = Entity.FindEntityByName(PlayerOneName);
            entP2 = Entity.FindEntityByName(PlayerTwoName);

            if (entP1 != null)
                transformP1 = entP1.GetComponent<Transform>();
            if (entP2 != null)
                transformP2 = entP2.GetComponent<Transform>();

            Graphics.RequestSwitchCamera(entity.Id);
        }

        public override void Update()
        {
           //Graphics.RequestSwitchCamera(entity.Id);
            if (camComp == null)
            {
                camComp = entity.GetComponent<Camera>();
                return;
            }

            if (entP1 == null)
            {
                entP1 = Entity.FindEntityByName(PlayerOneName);
            }
            if (entP2 == null)
            {
                entP2 = Entity.FindEntityByName(PlayerTwoName);
            }
            if (transformP1 == null)
            {
                if (entP1 != null)
                    transformP1 = entP1.GetComponent<Transform>();
            }
            if (transformP2 == null)
            {
                if (entP2 != null)
                    transformP2 = entP2.GetComponent<Transform>();
            }
            
            bool hasTarget = true;
            Vector3 target = Vector3.ZERO;

            if (transformP1 != null && transformP2 != null)
            {
                Vector3 p1Pos = transformP1.Position;
                Vector3 p2Pos = transformP2.Position;

                // find midpoint between the 2 coordinates
                target = (p1Pos + p2Pos) * 0.5f;
            }
            else if (transformP1 != null)
            {
                //NativeLog.LogWarn("Targeting P1");
                target = transformP1.Position;
            }
            else if (transformP2 != null)
            {
                //NativeLog.LogWarn("Targeting P2");
                target = transformP2.Position;
            }
            else
            {
                hasTarget = false;
                NativeLog.LogWarn("CameraFollow script cannot find any player");
            }

            _timer += Time.DeltaTime;
            if (hasTarget) {
                if (_timer <= 2)
                {
                    camComp.Zoom = 10.0f;
                }
                else if (_timer > 2 && _timer <= 5)
                    camComp.Zoom = lerp(10.0f, CamZoom, (_timer - 2.0f) / 3.0f);
                else
                {
                    camComp.Zoom = CamZoom;
                    NativeLog.Log("Zooomingggg");

                }
                camComp.Yaw = CamYaw;
                camComp.Pitch = CamPitch;
                camComp.Position = CameraPosition;
                camComp.Target = target;
            }
        }

        public Camera GetCameraComp()
        {
            return camComp;
        }
        private float lerp(float a, float b, float t)
        {
            return a + (b - a) * t;
        }
    }
}
