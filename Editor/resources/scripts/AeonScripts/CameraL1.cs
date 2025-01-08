using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class CameraL1 :AeonBehaviour
    {
        public string PlayerOneName = "";
        public string PlayerTwoName = "";
        public Vector3 CameraPosition = Vector3.ZERO;
        public float CamYaw = 0.0f;
        public float CamPitch = 0.0f;
        private float CamZoom = 35.0f;

        private Camera camComp = null;
        private Entity entP1 = null;
        private Entity entP2 = null;
        private Transform transformP1 = null;
        private Transform transformP2 = null;
        private Transform CameraTransform = null;
        public static int cam = 0;

        private AudioComponent camAudio = null;
        private bool startRumble = false;

        private float _timer = 0.0f;

        //shake
        public float shakeAmount = 0.7f;
        public float camStart = 10.0f;
        public float camDuration = 15.0f;
        private bool camshakeState = true;

        //panning
        private bool panningState = false;
        private bool panningState2 = false;
        private bool panningState3 = false;
        private Vector3 oldposition = Vector3.ZERO;
        private Vector3 newposition = Vector3.ZERO;
        private float pannningTimer = 0.0f;
        private float panningDuration = 3.0f;
        private Vector3 targetc2 = new Vector3(200, 0, 40);

		public Vector3 target_camera = Vector3.ZERO;
		public bool cameraShake = false;

        private bool area1 = false;
        private bool area2 = false;
        private MoveCharacter MoveCharPlayer1 = null;
        private MoveCharacter MoveCharPlayer2 = null;

		//panning
		public override void OnCreate()
        {
            camComp = entity.GetComponent<Camera>();

            entP1 = Entity.FindEntityByName(PlayerOneName);
            entP2 = Entity.FindEntityByName(PlayerTwoName);

            CameraTransform = entity.GetComponent<Transform>();

            if (entP1 != null)
                transformP1 = entP1.GetComponent<Transform>();
            if (entP2 != null)
                transformP2 = entP2.GetComponent<Transform>();

            camAudio = entity.GetComponent<AudioComponent>();

            Graphics.RequestSwitchCamera(entity.Id);

            panningState = false;
            panningState2 = false;
            camshakeState = true;
            oldposition = Vector3.ZERO;

            MoveCharPlayer1 = entP1.GetScript<MoveCharacter>();
            MoveCharPlayer2 = entP2.GetScript<MoveCharacter>();

			area1 = true;
            area2 = false;
		}

        public override void Update()
        {
            //if (GameState.IsPaused) return;
			Graphics.RequestSwitchCamera(entity.Id);
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
            if (hasTarget)
            {
                if (camshakeState)
                {
                    if (_timer <= 2)
                    {
                        camComp.Zoom = 10.0f;
                    }
                    else if (_timer > 2 && _timer <= 5)
                    {
						//NativeLog.LogWarn("camera zoom : {0}", camComp.Zoom);
						camComp.Zoom = lerp(10.0f, CamZoom, (_timer - 2.0f) / 3.0f);
                    }
                    else
                    {
                        camComp.Zoom = CamZoom;
                        // target=camShake(camStart, camDuration, shakeAmount,target);
                    }

                    if (cameraShake == true)
                    {
                        target = camShake(camStart, camDuration, shakeAmount, target);
                    }
                    camComp.Yaw = CamYaw;
                    camComp.Pitch = CamPitch;

                    CameraTransform.Position = CameraPosition;
                    camComp.Target = target;

                    oldposition = CameraPosition;
                    newposition = new Vector3(oldposition.X, oldposition.Y, oldposition.Z - 80.0f);
                }                
                //  start panning after destruction
                else if (panningState)
                {
                    //  change player direction to new area
					area2 = true;

                    if (!GameState.IsPaused)
                    {
                        pannningTimer += Time.DeltaTime;
                    }

                    //  lerp to move camera to new position
                    float z = lerp(oldposition.Z, newposition.Z, pannningTimer / panningDuration);
					float y = lerp(oldposition.Y, newposition.Y, pannningTimer / panningDuration);
					float x = lerp(oldposition.X, newposition.X, pannningTimer / panningDuration);
					Vector3 newpos = camComp.Position;
                    newpos.Z = z;
                    newpos.Y = y;
                    newpos.X = x;

                    camComp.Target = target;
                    CameraTransform.Position = newpos;
                    //camComp.Position = newpos;

                    if (pannningTimer >= panningDuration)
                    {
                        panningState = false;
                        panningState2 = true;
                        pannningTimer = 0;

						oldposition = newpos;
						newposition = new Vector3(oldposition.X + 120.0f, oldposition.Y, oldposition.Z);
					}
                }
                else if(panningState2)
                {


                    if (!GameState.IsPaused)
                    {
                        pannningTimer += Time.DeltaTime;
                    }

                    //  lerp to move camera to new position
                    float z = lerp(oldposition.Z, newposition.Z, pannningTimer / panningDuration);
					float y = lerp(oldposition.Y, newposition.Y, pannningTimer / panningDuration);
					float x = lerp(oldposition.X, newposition.X, pannningTimer / panningDuration);
					camComp.Zoom = lerp(CamZoom, CamZoom + 10.0f, pannningTimer / panningDuration);

					Vector3 newpos = camComp.Position;
					newpos.Z = z;
					newpos.Y = y;
					newpos.X = x;

					camComp.Target = targetc2;
					CameraTransform.Position = newpos;

					if (pannningTimer >= panningDuration)
					{
						panningState2 = false;
                        panningState3 = true;
						pannningTimer = 0;
					}
				}
				else if (panningState3)
				{
                    NativeLog.Log("Panning {0}", pannningTimer);

                    if (!GameState.IsPaused)
                    {
                        pannningTimer += Time.DeltaTime;
                    }
                    float z = lerp(targetc2.Z, target.Z, pannningTimer / panningDuration);
					float y = lerp(targetc2.Y, target.Y, pannningTimer / panningDuration);
					float x = lerp(targetc2.X, target.X, pannningTimer / panningDuration);

                    Vector3 newpos = camComp.Target;
					newpos.Z = z;
					newpos.Y = y;
					newpos.X = x;
					camComp.Target = newpos;

					if (pannningTimer >= panningDuration)
                    {
						panningState3 = false;
						pannningTimer = 0;
					}
				}
                else
                {
					camComp.Target = target;
				}

			}

			//target_camera = camComp.Target;

			if (area1)
			{
				MoveCharPlayer1.forwardDir = new Vector3(0, 0, -1);
				MoveCharPlayer1._baseRotation = new Vector3(0, 180, 0);

				MoveCharPlayer2.forwardDir = new Vector3(0, 0, -1);
				MoveCharPlayer2._baseRotation = new Vector3(0, 180, 0);
				area1 = false;
			}
            
            else if(area2)
            {
				MoveCharPlayer1.forwardDir = new Vector3(1, 0, 0);
				MoveCharPlayer1._baseRotation = new Vector3(0, 90, 0);

				MoveCharPlayer2.forwardDir = new Vector3(1, 0, 0);
				MoveCharPlayer2._baseRotation = new Vector3(0, 90, 0);
				area2 = false;
			}

		}

        private float lerp(float a, float b, float t)
        {
            return a + (b - a) * t;
        }

        //Camera Shake Logic
        private Vector3 camShake (float start, float duration, float shake, Vector3 position)
        {
            Vector3 newPos = Vector3.ZERO;
            if (_timer >= camStart && !GameState.IsPaused)
            {
               
                if (camDuration > 0)
                {
                    camshakeState = true;
                    var random = new Random();
                    float x =position.X + (float)(random.NextDouble() * 2 - 1) * shakeAmount;
                    float y = position.Y + (float)(random.NextDouble() * 2 - 1) * shakeAmount;
                    float z = position.Z + (float)(random.NextDouble() * 2 - 1) * shakeAmount;
                    position = new Vector3(x, y, z);
                    camDuration -= Time.DeltaTime;
                    NativeLog.Log("Camera Shake Position: {0},{1},{2}",x,y,z);

                    if (!startRumble)
                    {
                        if (camAudio != null)
                        {
                            camAudio.FadeIn(1.0f);
                            camAudio.PlaySound();
                            NativeLog.Log("we rumble");
                            startRumble = true;
                        }
                    }
                    return position;

                }
                //Stop after y duration
                else
                {
                    camshakeState = false;
                    panningState = true;
                    camDuration = 0f;
                    if (startRumble)
                    {
                        if (camAudio != null) 
                            camAudio.FadeOut(1.0f);
                        startRumble = false;

                    }
                    //camAudio.FadeOut(1.0f);
                    return position;
                }
            }
            return position;
        }

        //Camera Panning 
    }
}
