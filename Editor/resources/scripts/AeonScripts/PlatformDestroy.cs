using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class PlatformDestroy : AeonBehaviour
    {
        public static bool isPlatformDestroying = false;

        private Transform[] transforms = new Transform[33];

        //private AudioComponent fallingSfx = null;
        //private bool playfall = false;
        //private bool delayingaudio = false;

        private string[] entityNames = new string[33]
        {
        "Ground.A02", "Ground.A03", "Ground.A01", "Ground.A00", "Ground.A04",
        "Red.Checkpoint1", "Ground.A15", "Ground.A14", "Ground.A11", "Ground.A10",
        "Ground.A20", "Ground.A30", "Ground.A40", "Blue.Checkpoint1", "Ground.A51",
        "Ground.A41", "Ground.A52", "Ground.A53", "Ground.A54", "Ground.A55",
        "Ground.A45", "Ground.A44", "Ground.A35", "Ground.A25", "spawnTile.A22",
        "spawnTile.A23", "spawnTile.A32", "spawnTile.A33", "Checkpoint1", "Balloon24",
        "Balloon25", "Balloon26", "Balloon27"
        };

        private AudioComponent audioComp = null;

        public float dt = 0f;
        public Vector3 gravity = new Vector3(0, -0.1f, 0);
        private Vector3 vel = Vector3.ZERO;

        private float timer = 0f;
        public float delayTimer = 5f;
        //public float delayaudio = 0f;

        //camera
        private Entity camera = null;
        public CameraL1 cameraLevel1 = null;

        public override void OnCreate()
        {
            AssignTransform();
            isPlatformDestroying = false;
            //fallingSfx = entity.GetComponent<AudioComponent>();
            camera = Entity.FindEntityByName("Camera");
            cameraLevel1 = camera.GetScript<CameraL1>();
            audioComp = entity.GetComponent<AudioComponent>();
        }

        public override void Update()
        {
            if (camera == null)
            {
                camera = Entity.FindEntityByName("Camera");
                return;
            }

            if (GameState.IsPaused) return;

            timer += Time.DeltaTime;

            if (timer >= delayTimer)
            {
                if (!isPlatformDestroying)
                {
                    if (audioComp != null)
                    {
                        audioComp.SoundFilepath = ".\\resources\\audios\\sfx_platform_falling.wav";
                        audioComp.PlaySound();
                    }
                }
                isPlatformDestroying = true;
                foreach (var trans in transforms)
                {
                    if (trans != null)
                    {
                        //if (GameState.IsPaused) return;
                        UpdateTransform(trans);
                    }
                }
                
            }

            //if (timer >= delayaudio && !playfall)
            //{
            //    if (fallingSfx != null)
            //    {
            //        fallingSfx.PlaySound();
            //        playfall = true;
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //        NativeLog.Log("play platform falling sound");
            //    }
            //}


        }

        public void AssignTransform()
        {
            for (int i = 0; i < entityNames.Length; i++)
            {
                Entity currentEntity = Entity.FindEntityByName(entityNames[i]);

                if (currentEntity != null)
                {
                    transforms[i] = currentEntity.GetComponent<Transform>();
                }
            }
        }

        private void UpdateTransform(Transform trans)
        {
            Vector3 drop = trans.Position;

            dt = Time.DeltaTime;

            vel += gravity * dt;
            drop += vel;
            trans.Position = drop;

            if (drop.Y <= -100f)
            {
                DestroyEntities();
            }
        }

        private void DestroyEntities()
        {
            for (int i = 0; i < entityNames.Length; i++)
            {
                Entity currentEntity = Entity.FindEntityByName(entityNames[i]);

                if (currentEntity != null)
                {
                    Entity.Destroy(currentEntity);
                }
            }
        }
    }
}
