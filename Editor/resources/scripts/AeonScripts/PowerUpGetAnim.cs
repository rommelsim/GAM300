using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class PowerUpGetAnim : AeonBehaviour
    {
        public Entity ownerEnt = null;

        private float startLife = 0.5f;

        private float secondLife = 0.2f;

        private float lifeTime = 0.5f;

        public float yValue = 0f;

        private float speed;

        private bool drop = false;

        private float rotationSpeed = 50f;

        public override void OnCreate()
        {
            
        }

        public override void Update()
        {
            if (entity.Name == "SpinnyAnim" || entity.Name == "WeightAnim" || entity.Name == "StarAnim" || entity.Name == "DashAnim")
            {
                speed = 20f;
            }
            else
            {
                speed = 3f;
            }

            startLife -= Time.DeltaTime;

            if(startLife <= 0 && !drop)
            {
                secondLife -= Time.DeltaTime;
                entity.GetComponent<Transform>().Scale += (2*Vector3.ONE) * Time.DeltaTime * speed/2;
                yValue += Time.DeltaTime;
                if(secondLife <= 0)
                {
                    drop = true;
                }
            }
            if (secondLife <= 0f && drop)
            {
                lifeTime -= Time.DeltaTime;
                yValue -= 28f * Time.DeltaTime;
                if (entity.GetComponent<Transform>().Scale.X >= 0)
                {
                    entity.GetComponent<Transform>().Scale -= Vector3.ONE * Time.DeltaTime * (speed * 2f);
                }
                
            }


            rotationSpeed += (float)Math.Exp(6f) * Time.DeltaTime;
            entity.GetComponent<Transform>().Rotation -= new Vector3(0,1,0) * Time.DeltaTime * rotationSpeed;
            entity.GetComponent<Transform>().Position = new Vector3(ownerEnt.GetComponent<Transform>().Position.X, yValue, ownerEnt.GetComponent<Transform>().Position.Z);

            if (lifeTime <= 0)
            {
                Entity.Destroy(entity);
            }
        }

    }
}
