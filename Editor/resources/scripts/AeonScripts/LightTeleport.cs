//using AeonScriptCore;
//using System;
//using System.Collections.Generic;

//namespace AeonScripts
//{
//    public class LightTeleport : AeonBehaviour
//    {
//        public float TeleportInterval = 10.0f;
//        //public float LightHeight = 50.0f;
//        public string P1_Zone1_Name;
//        public string P1_Zone2_Name;
//        public string P2_Zone1_Name;
//        public string P2_Zone2_Name;

//        private bool IsOverP1Zone = true;
//        private Vector3 heightVec = Vector3.ZERO;

//        private Transform _transform = null;
//        private float _timer;
//        private int chosenZone = 0;

//        private PlayerZoneDetectBox _scriptP1Zone1 = null;
//        private PlayerZoneDetectBox _scriptP1Zone2 = null;
//        private PlayerZoneDetectBox _scriptP2Zone1 = null;
//        private PlayerZoneDetectBox _scriptP2Zone2 = null;
//        private Vector3 _posP1Zone1 = Vector3.ZERO;
//        private Vector3 _posP1Zone2 = Vector3.ZERO;
//        private Vector3 _posP2Zone1 = Vector3.ZERO;
//        private Vector3 _posP2Zone2 = Vector3.ZERO;

//        public override void OnCreate()
//        {
//            _transform = entity.GetComponent<Transform>();
//            _timer = TeleportInterval;
//            chosenZone = GameData.random.Next(0, 2); // gen between 0 and 1

//            heightVec.Y = _transform.Position.Y;

//            Entity boxEnt = Entity.FindEntityByName(P1_Zone1_Name);
//            Transform transComp;
//            if (boxEnt != null)
//            {
//                _scriptP1Zone1 = boxEnt.GetScript<PlayerZoneDetectBox>();
//                transComp = boxEnt.GetComponent<Transform>();

//                if (transComp != null)
//                    _posP1Zone1 = transComp.Position;
//            }
//            else
//                NativeLog.LogError("LightTeleport, unable to find zone {0}", P1_Zone1_Name);

//            boxEnt = Entity.FindEntityByName(P1_Zone2_Name);
//            if (boxEnt != null)
//            {
//                _scriptP1Zone2 = boxEnt.GetScript<PlayerZoneDetectBox>();
//                transComp = boxEnt.GetComponent<Transform>();

//                if (transComp != null)
//                    _posP1Zone2 = transComp.Position;
//            }
//            else
//                NativeLog.LogError("LightTeleport, unable to find zone {0}", P1_Zone2_Name);
            
//            boxEnt = Entity.FindEntityByName(P2_Zone1_Name);
//            if (boxEnt != null)
//            {
//                _scriptP2Zone1 = boxEnt.GetScript<PlayerZoneDetectBox>();
//                transComp = boxEnt.GetComponent<Transform>();

//                if (transComp != null)
//                    _posP2Zone1 = transComp.Position;
//            }
//            else
//                NativeLog.LogError("LightTeleport, unable to find zone {0}", P2_Zone1_Name);

//            boxEnt = Entity.FindEntityByName(P2_Zone2_Name);
//            if (boxEnt != null)
//            {
//                _scriptP2Zone2 = boxEnt.GetScript<PlayerZoneDetectBox>();
//                transComp = boxEnt.GetComponent<Transform>();

//                if (transComp != null)
//                    _posP2Zone2 = transComp.Position;
//            }
//            else
//                NativeLog.LogError("LightTeleport, unable to find zone {0}", P2_Zone2_Name);

//            _transform.Position = (chosenZone == 0 ? _posP1Zone2 : _posP1Zone2) + heightVec;
//        }

//        public override void Update()
//        {
//            // ---- Setup
//            //if (entityP1ZoneDetector == null)
//            //{
//            //    entityP1ZoneDetector = Entity.FindEntityByName(PlayerOneZoneDetectorName);
//            //}
//            //if (entityP2ZoneDetector == null)
//            //{
//            //    entityP2ZoneDetector = Entity.FindEntityByName(PlayerTwoZoneDetectorName);
//            //}
//            //if (entityP1ZoneDetector != null && scriptP1Detector == null)
//            //{
//            //    scriptP1Detector = entityP1ZoneDetector.GetScript<PlayerZoneDetectBox>();
//            //}
//            //if (entityP2ZoneDetector != null && scriptP2Detector == null)
//            //{
//            //    scriptP2Detector = entityP2ZoneDetector.GetScript<PlayerZoneDetectBox>();
//            //}
//            // End of Setup ----

//            // if paused or if detect zones are not ready yet
//            if (GameState.IsPaused || PlayerZoneDetectBox.boxesEntered < GameData.TOTAL_BOXES_IN_ZONES) return;

//            _timer -= Time.DeltaTime;

//            // teleport!
//            if (_timer <= 0)
//            {
//                //NativeLog.Log("Light teleported!");

//                // Reset timer
//                if (IsOverP1Zone)
//                {
//                    (chosenZone == 0 ? _scriptP1Zone1 : _scriptP1Zone2).ResetTimer();
//                }
//                else
//                {
//                    (chosenZone == 0 ? _scriptP2Zone1 : _scriptP2Zone2).ResetTimer();
//                }

//                // Switch to next zone
//                IsOverP1Zone = !IsOverP1Zone;
//                chosenZone = GameData.random.Next(0, 2);
//                _transform.Position = (IsOverP1Zone ? (chosenZone == 0 ? _posP1Zone1 : _posP1Zone2) : (chosenZone == 0 ? _posP2Zone1 : _posP2Zone2)) + heightVec;

//                _timer = TeleportInterval;
//                return;
//            }

//            if (IsOverP1Zone)
//            {
//                (chosenZone == 0 ? _scriptP1Zone1 : _scriptP1Zone2).GivePlayerPoints();
//            }
//            else
//            {
//                (chosenZone == 0 ? _scriptP2Zone1 : _scriptP2Zone2).GivePlayerPoints();
//            }
//        }
//    }
//}
