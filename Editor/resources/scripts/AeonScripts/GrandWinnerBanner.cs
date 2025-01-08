using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class GrandWinnerBanner : AeonBehaviour
    {
        public Transform trans = null;

        private float delayTimer = 1.5f;
        private float timer = 0;

        public float min = -800f;
        public float max = -350f;

        public override void OnCreate()
        {
            trans = entity.GetComponent<Transform>();
        }

        public override void Update()
        {

            if (!GrandWinnerScreen.isDraw)
            {
                timer += Time.DeltaTime;

                if (timer >= delayTimer)
                {
                    Vector3 addTrans = trans.Position;
                    addTrans.Y += 700f * Time.DeltaTime;

                    if (trans.Position.Y >= max)
                    {
                        addTrans.Y = max;
                        timer = 0f;
                    }

                  trans.Position = addTrans;
                    
                }
            }
        }
    }
}
