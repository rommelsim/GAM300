using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;


namespace AeonScripts
{
    public class CameraShake : AeonBehaviour
    {
        public float camStart = 0.0f;
        public float camDuration = 5.0f;
        private float _timer = 0.0f;
        private Camera camComponent = null;
        private Vector3 originalPos = Vector3.ZERO;
        private Vector3 newPos = Vector3.ZERO;
        public float shakeAmount = 0.7f;

        private Entity cam1= null;
        //access the position from CameraLookAtPlayer
        public CameraLookAtPlayers cameraLookAtPlayers= null;
        public override void OnCreate()
        {
                camComponent= entity.GetComponent<Camera>();
                cam1=Entity.FindEntityByName("Cam1");
               

        }
        public override void Update()
        {
            CameraLookAtPlayers orgPos = cam1.GetScript<CameraLookAtPlayers>();
            if (camComponent == null)
            {
                camComponent = entity.GetComponent<Camera>();
                return;
            }
            if (GameState.IsPaused) return;
            //Increament the timer
            _timer += Time.DeltaTime;
            originalPos = new Vector3(orgPos.GetCameraComp().Position.X, orgPos.GetCameraComp().Position.Y, orgPos.GetCameraComp().Position.Z);
            //Locate game camera
            camComponent.Position = originalPos;
            if (_timer >= camStart)
            {
                
                //Locate game camera
                if (camDuration > 0)
                {
                    //NativeLog.Log("Camera Shake");
                    if (GameState.IsPaused) return;
					Graphics.RequestSwitchCamera(entity.Id);
					var random = new Random();
                    float x = originalPos.X + (float)(random.NextDouble() * 2 - 1) * shakeAmount;
                    float y = originalPos.Y + (float)(random.NextDouble() * 2 - 1) * shakeAmount;
                    float z = originalPos.Z + (float)(random.NextDouble() * 2 - 1) * shakeAmount;
                    newPos = new Vector3(x, y, z);
                    camComponent.Position = newPos;
                    camComponent.Pitch=orgPos.GetCameraComp().Pitch; 
                    camComponent.Yaw = orgPos.GetCameraComp().Yaw;
                    camComponent.Target=orgPos.GetCameraComp().Target;
                    camDuration -= Time.DeltaTime;
                }
                //Stop after y duration
                else
                {
                    if (GameState.IsPaused) return;
                    camDuration = 0f;
                    //camComponent.Position = originalPos;
                    //camComponent.Pitch = orgPos.GetCameraComp().Pitch;
                    //camComponent.Yaw = orgPos.GetCameraComp().Yaw;
                    //camComponent.Target = orgPos.GetCameraComp().Target;
                    Graphics.RequestSwitchCamera(cam1.Id);
                }
            }
        }
    }
}
