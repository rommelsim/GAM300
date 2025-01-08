using AeonScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AeonScripts
{
    public class MainMenuLetterMoving : AeonBehaviour
    {
        public float upSpeed;
        public float downSpeed;
        public float moveAmount;

        private float moveTime;
        private float editTime;
        private float moveCharTime = 0.5f;
        private float editCharTime;

        private bool moveLetter = false;
        private bool reset = false;

        private float oriY;


        public override void OnCreate()
        {
            moveTime = (float)GameData.random.NextDouble() * GameData.random.Next(1, 3);
            editTime = moveTime;
            editCharTime = moveCharTime;
            oriY = entity.GetComponent<Transform>().Position.Y;
        }

        public override void Update()
        {
            editTime -= Time.DeltaTime;

            if (editTime <= 0 && !moveLetter)
            {
                moveLetter = true;
            }


            if (moveLetter)
            {
                if (editCharTime >= 0)
                {
                    editCharTime -= Time.DeltaTime;
                    entity.GetComponent<Transform>().Position -= new Vector3(0, moveAmount, 0) * Time.DeltaTime * upSpeed;
                }
                else if (editCharTime <= 0)
                {
                    reset = true;

                    
                }


            }

            if (reset)
            {
                moveLetter = false;
                if (entity.GetComponent<Transform>().Position.Y <= oriY)
                {
                    entity.GetComponent<Transform>().Position += new Vector3(0, moveAmount, 0) * Time.DeltaTime * downSpeed;
                }
                else
                {
                    editTime = moveTime;
                    editCharTime = moveCharTime;
                    reset = false;
                    moveTime = (float)GameData.random.NextDouble() * GameData.random.Next(1,3);


                }

            }


            
        }

    }
}
