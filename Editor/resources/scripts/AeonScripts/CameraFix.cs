using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class CameraFix : AeonBehaviour
    {
        public Camera camCom = null;
        

        public override void OnCreate()
        {
            if (camCom == null) camCom = entity.GetComponent<Camera>();
           

        }

        public override void Update()
        {
            camCom.Position = new Vector3(20f, 53f, 170f);
        }
    }
}
