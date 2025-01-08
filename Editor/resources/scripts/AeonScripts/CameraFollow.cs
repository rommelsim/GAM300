using AeonScriptCore;
using System.Runtime.CompilerServices;

namespace AeonScripts
{
    public class CameraFollow : AeonBehaviour
    {
        public string PlayerName = "";
        public string PlayerTwoName = "";
        public float MinDistToPlayer = 30.0f;
        public float CamYaw = 0.0f;
        public float CamPitch = 45.0f;
        public float CamZoom = 20.0f;

        private Camera camComp = null;
        private Entity entP1 = null;
        private Entity entP2 = null;
        private Transform transformP1 = null;
        private Transform transformP2 = null;

        public override void OnCreate()
        {
            camComp = entity.GetComponent<Camera>();

            entP1 = Entity.FindEntityByName(PlayerName);
            entP2 = Entity.FindEntityByName(PlayerTwoName);

            if (entP1 != null)
                transformP1 = entP1.GetComponent<Transform>();
            if (entP2 != null)
                transformP2 = entP2.GetComponent<Transform>();
        }

        public override void Update()
        {
            if (camComp == null)
            {
                camComp = entity.GetComponent<Camera>();
                return;
            }

            if (entP1 == null)
            {
                entP1 = Entity.FindEntityByName(PlayerName);
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
            float distToTarget = MinDistToPlayer;
            Vector3 camPos = target;

            if (transformP1 != null && transformP2 != null)
            {
                //NativeLog.LogWarn("Targeting both Ps");
                Vector3 p1Pos = transformP1.Position;
                Vector3 p2Pos = transformP2.Position;

                // find midpoint between the 2 coordinates
                target = (p1Pos + p2Pos) * 0.5f;

                //float distOfPlayers = Vector3.Dist(p1Pos, p2Pos);
                // Using pythagoras theorem to get the distance from midpoint to camera: min dist is hyp, dist of players
                //distToTarget = MathHelper.Sqrt((MinDistToPlayer * MinDistToPlayer) - (distOfPlayers/2f));
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

            if (hasTarget) {
                camPos.X = target.X + distToTarget * MathHelper.Cos(MathHelper.DegToRad * (CamPitch)) * MathHelper.Cos(MathHelper.DegToRad * (CamYaw));
                camPos.Y = target.Y + distToTarget * MathHelper.Sin(MathHelper.DegToRad * (CamPitch));
                camPos.Z = target.Z + distToTarget * MathHelper.Cos(MathHelper.DegToRad * (CamPitch)) * MathHelper.Sin(MathHelper.DegToRad * (CamYaw));

                camComp.Zoom = CamZoom;
                camComp.Yaw = CamYaw;
                camComp.Pitch = CamPitch;
                camComp.Position = camPos;
                camComp.Target = target;
            }
        }

        public void SetPlayerOneName(string name)
        {
            PlayerName = name;
            entP1 = name == string.Empty ? null : Entity.FindEntityByName(PlayerName);
        }

        public void SetPlayerTwoName(string name)
        {
            PlayerTwoName = name;
            entP2 = name == string.Empty ? null : Entity.FindEntityByName(PlayerTwoName);
        }
    }
}
