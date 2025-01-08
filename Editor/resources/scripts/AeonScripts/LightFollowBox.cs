using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    //this script should be for light -> point box so attach it to light that wants to follow box
    public class LightFollowBox : AeonBehaviour
    {
        /*
        Logik:
        get points box positon 
        get light direction position
        then update the direction to follow the box 
        */

        public string targetBox = "";
        private Entity entBox = null;
        private Transform transformLight = null;
        private Transform transformBox = null;
        private Lighting lightingBox = null;
        private Vector3 offset = Vector3.ZERO;

        private float turn;
        public override void OnCreate()
        {
            transformLight = entity.GetComponent<Transform>();
            //lightingBox = entity.GetComponent<Lighting>();

            entBox = Entity.FindEntityByName(targetBox);
            //Box position
            //if (entBox != null)
            //{
            //    transformBox = entBox.GetComponent<Transform>();
            //}
            //offset = transformLight.Position - transformBox.Position;
            //offset = lightingBox.Direction - transformBox.Position;
        }
        public override void Update()
        {
            //Checks
            if (transformLight == null)
            {
                transformLight = entity.GetComponent<Transform>();
                return;
            }
            if (entBox == null)
            {
                entBox = Entity.FindEntityByName(targetBox);
            }
            if (transformBox == null)
            {
                if (entBox != null)
                {
                    transformBox = entBox.GetComponent<Transform>();

                }
            }
            // Logic

            if (transformBox != null)
            {
                if (GameState.IsPaused) return;
                turn += 10;
                transformLight.Rotation = new Vector3(0, turn, 0);
                //offset = transformBox.Position - transformLight.Position;
                //offset = Vector3.Normalize(offset);

                // Update light position to follow the box
                //lightingBox.Direction = offset;
                //transformLight.Position= transformBox.Position+offset;
            }
        }
    }
}
