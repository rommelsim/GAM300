using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Schema;

namespace AeonScripts
{
    internal class DrawBanner : AeonBehaviour
    {
        public Transform bannerTrans = null;

        private float delayTimer = 1.5f;
        private float timer = 0;

        public float max = -300f;
        public float min = -800f;
        public override void OnCreate()
        {
           bannerTrans = entity.GetComponent<Transform>();
        }

        public override void Update()
        {
            if (GrandWinnerScreen.isDraw)
            {
                timer += Time.DeltaTime;

                if (timer >= delayTimer)
                {
                    Vector3 addTrans = bannerTrans.Position;
                    addTrans.Y += 700f * Time.DeltaTime;

                    if (bannerTrans.Position.Y >= max)
                    {
                        addTrans.Y = max;
                        timer = 0f;
                    }
                    bannerTrans.Position = addTrans;
                }
            }
        }
    }
}
