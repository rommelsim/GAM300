using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class PushDestroy : AeonBehaviour
    {
        public RigidBody body = null;
        public Transform trans = null;

        private Entity balloon1 = null;
        private Entity balloon2 = null;
        private Entity balloon3 = null;
        private Entity balloon4 = null;

        private Entity ground22 = null;
        private Entity ground23 = null;
        private Entity ground32 = null;
        private Entity ground33 = null;

        private Transform ground22trans = null;
        private Transform ground23trans = null;
        private Transform ground32trans = null;
        private Transform ground33trans = null;

        private Transform balloon1trans = null;
        private Transform balloon2trans = null;
        private Transform balloon3trans = null;
        private Transform balloon4trans = null;



        //public DateTime checkTime;
        //public float timer = 5f;

        private float timer = 0f;
        public float delayTimer = 5f;

        public override void OnCreate()
        {
            body = entity.GetComponent<RigidBody>();
            trans = entity.GetComponent<Transform>();

            balloon1 = Entity.FindEntityByName("Balloon24");
            balloon2 = Entity.FindEntityByName("Balloon25");
            balloon3 = Entity.FindEntityByName("Balloon26");
            balloon4 = Entity.FindEntityByName("Balloon27");

            ground22 = Entity.FindEntityByName("Ground.A22");
            ground23 = Entity.FindEntityByName("Ground.A23");
            ground32 = Entity.FindEntityByName("Ground.A32");
            ground33 = Entity.FindEntityByName("Ground.A33");

            if (balloon1 != null) { balloon1trans = balloon1.GetComponent<Transform>(); }
            if (balloon2 != null) { balloon2trans = balloon2.GetComponent<Transform>(); }
            if (balloon3 != null) { balloon3trans = balloon3.GetComponent<Transform>(); }
            if (balloon4 != null) { balloon4trans = balloon4.GetComponent<Transform>(); }

            if (ground22 != null) { ground22trans = ground22.GetComponent<Transform>(); }
            if (ground23 != null) { ground23trans = ground23.GetComponent<Transform>(); }
            if (ground32 != null) { ground32trans = ground32.GetComponent<Transform>(); }
            if (ground33 != null) { ground33trans = ground33.GetComponent<Transform>(); }
            //checkTime = DateTime.Now.AddSeconds(timer);
        }

        public override void Update()
        {
            if (body == null)
            {
                body = entity.GetComponent<RigidBody>();
            }
            if (trans == null)
            {
                trans = entity.GetComponent<Transform>();
            }
            //if (checkTime == default(DateTime))
            //{
            //    // Initialize checkTime only once
            //    checkTime = DateTime.Now.AddSeconds(timer);
            //}

            //TimeSpan elapsed = DateTime.Now - checkTime;

            //if (elapsed.TotalSeconds >= 0 && elapsed.TotalSeconds < timer)
            //{
            //    NativeLog.Log($"Seconds remaining until activation: {timer - elapsed.TotalSeconds} seconds");
            //}
            //else if (elapsed.TotalSeconds >= timer) 
            //{
            //    NativeLog.Log("Activated");
            //    //body.Mass = 50f;
            //    body.UseGravity = true;
            //}

            //if (Input.GetKeyPressed(Keys.KEY_SPACE))
            //{

            timer += Time.DeltaTime;

            if (timer >= delayTimer)
            {
                NativeLog.Log("Activated");
                DestroyEntity("Collider2");

                Vector3 drop = trans.Position;

                Vector3 balloons1 = balloon1trans.Position;
                Vector3 balloons2 = balloon2trans.Position;
                Vector3 balloons3 = balloon3trans.Position;
                Vector3 balloons4 = balloon4trans.Position;

                Vector3 grd22 = ground22trans.Position;
                Vector3 grd23 = ground23trans.Position;
                Vector3 grd32 = ground32trans.Position;
                Vector3 grd33 = ground33trans.Position;

                Vector3 gravity = new Vector3(0, -9.8f, 0);
                Vector3 vel = Vector3.ZERO;
                float dt = 0.2f;

                if (drop.Y <= 2f)
                {
                    vel += gravity * dt;
                    balloons1 += vel * dt;
                    balloons2 += vel * dt;
                    balloons3 += vel * dt;
                    balloons4 += vel * dt;

                    grd22 += vel * dt;
                    grd23 += vel * dt;
                    grd32 += vel * dt;
                    grd33 += vel * dt;
                }

                balloon1trans.Position = balloons1;
                balloon2trans.Position = balloons2;
                balloon3trans.Position = balloons3;
                balloon4trans.Position = balloons4;

                ground22trans.Position = grd22;
                ground23trans.Position = grd23;
                ground32trans.Position = grd32;
                ground33trans.Position = grd33;

                if (drop.Y <= -150f)
                {
                    Entity.Destroy(entity);
                    DestroyEntities();
                }
            }

            //}
            //NativeLog.Log("Activated");
            //body.UseGravity = true;

        }

        public void DestroyEntity(string entName)
        {
            Entity ent = Entity.FindEntityByName(entName);
            if (ent != null)
            {
                Entity.Destroy(ent);
            }
        }

        private void DestroyEntities()
        {
            DestroyEntity("Ground.A00");
            DestroyEntity("Ground.A01");
            DestroyEntity("Ground.A02");
            DestroyEntity("Ground.A03");
            DestroyEntity("Ground.A04");
            DestroyEntity("Ground.A05");
            DestroyEntity("Ground.A06");
            DestroyEntity("Ground.A07");
            DestroyEntity("Ground.A08");
            DestroyEntity("Ground.A09");
            DestroyEntity("Ground.A10");
            DestroyEntity("Ground.A11");
            DestroyEntity("Ground.A12");
            DestroyEntity("Ground.A13");
            DestroyEntity("Ground.A14");
            DestroyEntity("Ground.A15");
            DestroyEntity("Ground.A16");
            DestroyEntity("Ground.A17");
            DestroyEntity("Ground.A18");
            DestroyEntity("Ground.A19");
            DestroyEntity("Ground.A20");
            DestroyEntity("Ground.A21");
            DestroyEntity("Ground.A22");
            DestroyEntity("Ground.A23");
            DestroyEntity("Ground.A24");
            DestroyEntity("Ground.A25");
            DestroyEntity("Ground.A26");
            DestroyEntity("Ground.A27");
            DestroyEntity("Ground.A28");
            DestroyEntity("Ground.A29");
            DestroyEntity("Ground.A30");
            DestroyEntity("Ground.A31");
            DestroyEntity("Ground.A32");
            DestroyEntity("Ground.A33");
            DestroyEntity("Ground.A34");
            DestroyEntity("Ground.A35");
            DestroyEntity("Ground.A36");
            DestroyEntity("Ground.A37");
            DestroyEntity("Ground.A38");
            DestroyEntity("Ground.A39");
            DestroyEntity("Ground.A40");
            DestroyEntity("Ground.A41");
            DestroyEntity("Ground.A42");
            DestroyEntity("Ground.A43");
            DestroyEntity("Ground.A44");
            DestroyEntity("Ground.A45");
            DestroyEntity("Ground.A46");
            DestroyEntity("Ground.A47");
            DestroyEntity("Ground.A48");
            DestroyEntity("Ground.A49");
            DestroyEntity("Ground.A50");
            DestroyEntity("Ground.A51");
            DestroyEntity("Ground.A52");
            DestroyEntity("Ground.A53");
            DestroyEntity("Ground.A54");
            DestroyEntity("Ground.A55");
            DestroyEntity("Balloon24");
            DestroyEntity("Balloon25");
            DestroyEntity("Balloon26");
            DestroyEntity("Balloon27");
            DestroyEntity("Checkpoint1");
        }
    }
}
