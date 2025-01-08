using AeonScriptCore;

namespace AeonScripts.Level2
{
    public class CameraL2 : AeonBehaviour
    {
        public float StartZoom = 10.0f;
        public float CamMoveSpeed = 10.0f;
        public float CamYaw = 0.0f;
        public float CamPitch = 0.0f;
        public float CamZoom = 40.0f;
        public Vector3 CamLookAt = Vector3.ZERO;
        public Vector3 CamPosition = Vector3.ZERO;

        private Camera camComp = null;
        private Transform transComp = null;

        private float _zoomTimer = 0.0f;

        public override void OnCreate()
        {
            camComp = entity.GetComponent<Camera>();
            transComp = entity.GetComponent<Transform>();
            camComp.Zoom = StartZoom;

            CamPosition.Y = 150f;
            CamLookAt.Y = -90f;

            CamPosition.Z = -800f;
            CamLookAt.Z = -550f;
        }

        public override void Update()
        {
            entity.GetComponent<Transform>().Position = Entity.FindEntityByName("BehindCam").GetComponent<Transform>().Position + new Vector3(0, 0, 80);

            if (camComp == null)
            {
                camComp = entity.GetComponent<Camera>();
                return;
            }
            if (transComp == null)
            {
                transComp = entity.GetComponent<Transform>();
                return;
            }
            
            _zoomTimer += Time.DeltaTime;
            if (_zoomTimer <= 2.0f)
            {
                camComp.Zoom = StartZoom;
            }
            else if (_zoomTimer > 2.0f && _zoomTimer <= 5.0f)
            {
				camComp.Zoom = lerp(StartZoom, CamZoom, (_zoomTimer - 2.0f) / 3.0f);
            }
            else if (_zoomTimer >= 6.5f)
            {
                if (!GameState.IsPaused)
                {
                    CamLookAt.Z += Time.DeltaTime * CamMoveSpeed;
                    CamPosition.Z += Time.DeltaTime * CamMoveSpeed;
                }
            }

            if (CamPosition.Z >= 50.0f)
            {
                CamPosition.Z = 50.0f;
				CamLookAt.Z = 300.0f;
			}

            // Move camera along axis over time, until we reach the limit
            camComp.Yaw = CamYaw;
            camComp.Pitch = CamPitch;
            //camComp.Zoom = CamZoom;
            camComp.Position = CamPosition;
            camComp.Target = CamLookAt;
        }
        private float lerp(float a, float b, float t)
        {
            return a + (b - a) * t;
        }
    }
}
