using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class BridgeDrop : AeonBehaviour
    {
        //Lokgik:
        //1. Give a time for drop to happen 
        //2. Drop the bridge to Y:0 
        //3. Set camera shake to true 
        private AudioComponent audioComp = null;
        private bool hasAudioPlayed = false;

        private Transform bridgeTransform = null;
        private Vector3 bridgePos = Vector3.ZERO;
        public float startTime = 0.0f;
        public float endTime = 0.0f;
        public Vector3 gravity = Vector3.ZERO;
        private float _timer =0.0f;
        private Entity camera= null;
        public CameraL1 cameraLevel1=null;
        private MoveCharacter checkpoint2p1 = null;
        private MoveCharacter checkpoint2p2 = null;


        public override void OnCreate()
        {
            bridgeTransform=entity.GetComponent<Transform>();
            camera = Entity.FindEntityByName("Camera");
            cameraLevel1 = camera.GetScript<CameraL1>();
            audioComp = entity.GetComponent<AudioComponent>();
            checkpoint2p1 = Entity.FindEntityByName("Player 1").GetScript<MoveCharacter>();
            checkpoint2p2 = Entity.FindEntityByName("player2").GetScript<MoveCharacter>();
        }

        public override void Update()
        {
            if (bridgeTransform == null)
            {
                bridgeTransform = entity.GetComponent<Transform>();
                return;
            }
            if (camera == null)
            {
                camera = Entity.FindEntityByName("Camera");
                return;
            }
            if (GameState.IsPaused) return;
            _timer += Time.DeltaTime;
            //Timer > startTime drop

            if (_timer >= startTime && _timer<=endTime)
            {
                Vector3 dropping = bridgeTransform.Position;

                dropping += gravity * Time.DeltaTime;
                bridgeTransform.Position = dropping;
                if (dropping.Y <=3.0f)
                {
                    bridgeTransform.Position = new Vector3(bridgeTransform.Position.X, 0.0f, bridgeTransform.Position.Z);
                    cameraLevel1.cameraShake = true;

                }
               
            }
            if (_timer >= endTime)
            {
                if (GameState.IsPaused) return;
                if (!hasAudioPlayed)
                {
                    if (audioComp != null)
                    {
                        audioComp.SoundFilepath = ".\\resources\\audios\\sfx_platform_falling.wav";
                        audioComp.PlaySound();
                        hasAudioPlayed = true;
                    }
                }

                Vector3 droppingFurther = bridgeTransform.Position;

                droppingFurther += gravity * 0.1f * 0.1f;
                bridgeTransform.Position = droppingFurther;
                if (droppingFurther.Y <= -60.0f)
                {
                    
                    Entity.Destroy(entity);

                }
            }

        }


    }
}
