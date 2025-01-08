//using AeonScriptCore;
//using System;
//using System.Resources;

//namespace AeonScripts
//{
//    public class PlayerZoneDetectBox : AeonBehaviour
//    {
//        public bool isPlayerOneZone = true;

//        private float _timer = 0;
//        private int _pointsZoneHas = 0;

//        private AudioComponent playBGM;
//        private string[] randomRedAudioComponents;
//        private string[] randomBlueAudioComponents;

//        // for game setup only
//        public static int boxesEntered = 0;
//        private static bool setup_AllBoxesIn = false;

//        public override void OnCreate()
//        {
//            _timer = GameData.GivePointInterval;

//            // got to reset the data values too lmao
//            boxesEntered = 0;
//            setup_AllBoxesIn = false;
//            playBGM = entity.GetComponent<AudioComponent>();

//            // Initialize an array of AudioComponent for random audio playback
//            randomRedAudioComponents = new string[3]; // Adjust the size as needed
//            randomBlueAudioComponents = new string[3]; // Adjust the size as needed
//            //for (int i = 0; i < randomRedAudioComponents.Length; i++)
//            {
//                randomRedAudioComponents[0] = ".\\resources\\audios\\Point Increase.wav"; // Adjust the file paths
//                randomRedAudioComponents[1] = ".\\resources\\audios\\Point Increase 2.wav"; // Adjust the file paths
//                randomRedAudioComponents[2] = ".\\resources\\audios\\Point Increase 3.wav"; // Adjust the file paths
//            }
//            //for (int i = 0; i < randomBlueAudioComponents.Length; i++)
//            {
//                randomBlueAudioComponents[0] = ".\\resources\\audios\\Point Decrease.wav"; // Adjust the file paths
//                randomBlueAudioComponents[1] = ".\\resources\\audios\\Point Decrease 2.wav"; // Adjust the file paths
//                randomBlueAudioComponents[2] = ".\\resources\\audios\\Point Decrease 3.wav"; // Adjust the file paths
//            }
//        }

//        public override void OnTriggerEnter(uint otherEntityID)
//        {
//            Entity otherEnt = Entity.GetEntity(otherEntityID);
//            BoxWithPoint boxWithPoint = otherEnt.GetScript<BoxWithPoint>();
//            if (boxWithPoint != null)
//            {
//                _pointsZoneHas += boxWithPoint.PointScore;
//                //NativeLog.LogWarn("{0}'s score is {1}", isPlayerOneZone ? "Player One" : "Player Two", _pointsZoneHas);
//                NativeLog.LogWarn("Box of point {1} entered {2}, points the zone has is {0}", _pointsZoneHas, boxWithPoint.PointScore, entity.Name);
//                boxesEntered += 1;

//                if (boxesEntered == GameData.TOTAL_BOXES_IN_ZONES) {
//                    NativeLog.LogWarn("All {0} boxes have entered their trigger zones", GameData.TOTAL_BOXES_IN_ZONES);
//                    setup_AllBoxesIn = true;
//                }
//            }
//        }

//        public override void OnTriggerExit(uint triggerEntityID)
//        {
//            Entity otherEnt = Entity.GetEntity(triggerEntityID);
//            BoxWithPoint boxWithPoint = otherEnt.GetScript<BoxWithPoint>();
//            if (boxWithPoint != null)
//            {
//                _pointsZoneHas -= boxWithPoint.PointScore;
//                NativeLog.LogWarn("Box of point {2} exited {0}, points now is {1}", entity.Name, _pointsZoneHas, boxWithPoint.PointScore);
//                //NativeLog.LogWarn("{0}'s score is {1}", isPlayerOneZone ? "Player One" : "Player Two", _pointsZoneHas);
//            }
//        }

//        //public override void Update()
//        //{
//        //    if (GameState.IsPaused) return;

//        //    GivePlayerPoints();
//        //}

//        public void GivePlayerPoints()
//        {
//            _timer -= Time.DeltaTime;

//            if (_timer <= 0)
//            {
//                if (setup_AllBoxesIn)
//                {
//                    if (isPlayerOneZone) {
//                        GameData.P1_Score += _pointsZoneHas;

//                        if (playBGM != null)
//                        {
//                            //playBGM.PlaySound();
//                            // Play a random audio
//                            PlayRandomRedAudio();
//                        }
//                    } else {
//                        GameData.P2_Score += _pointsZoneHas;

//                        if (playBGM != null) { PlayRandomBlueAudio(); }
//                    }
//                }

//                _timer = GameData.GivePointInterval;

//                //NativeLog.LogWarn("Points updated {0} from {1}", PlayerData.P1_Score, entity.Name);
//            }
//        }
//        private void PlayRandomRedAudio()
//        {
//            int randomIndex = GameData.random.Next(0, randomRedAudioComponents.Length);

//            if (randomIndex >= 0 && randomIndex < randomRedAudioComponents.Length)
//            {
//                playBGM.SoundFilepath = randomRedAudioComponents[randomIndex];
//                playBGM.PlaySound();
//            }
//        }
//        private void PlayRandomBlueAudio()
//        {
//            int randomIndex = GameData.random.Next(0, randomBlueAudioComponents.Length);

//            if (randomIndex >= 0 && randomIndex < randomBlueAudioComponents.Length)
//            {
//                playBGM.SoundFilepath = randomBlueAudioComponents[randomIndex];
//                playBGM.PlaySound();
//            }
//        }
//        public void ResetTimer()
//        {
//            _timer = GameData.GivePointInterval;
//        }
//    }
//}
