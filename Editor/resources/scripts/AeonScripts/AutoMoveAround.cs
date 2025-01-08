using AeonScriptCore;

namespace AeonScripts
{
    internal class AutoMoveAround : AeonBehaviour
    {
        public float MoveSpeed = 1f;
        public float TravelDistance = 8f;
        public float PosX = 0f;
        public float PosY = 0f;
        public float PosZ = 0f;
        public float TimePassed = 0f;
        //public bool CanFly = true;

        private Transform transformComp = null;
        private Vector3 startPos; // record the initial pos, to base off +- calculations
        private float TimeStartMove = -1f; // get the time when update first ran

        public override void OnCreate()
        {
            this.transformComp = this.entity.GetComponent<Transform>();
            this.startPos = transformComp.Position;
        }

        public override void Update()
        {
            //if (Input.GetKeyHeld(Input.KEY_4))
            //{
            //    Console.WriteLine("C#: key 4 is held");
            //}
            //if (Input.GetMouseButtonHeld(Input.MOUSE_BUTTON_LEFT))
            //{
            //    Console.WriteLine("C#: mouse left is held");
            //}
            //if (Input.GetMouseButtonHeld(Input.MOUSE_BUTTON_MIDDLE))
            //{
            //    Console.WriteLine("C#: mouse middle is held");
            //}
            //if (Input.GetMouseButtonHeld(Input.MOUSE_BUTTON_RIGHT))
            //{
            //    Console.WriteLine("C#: mouse right is held");
            //}

            if (TimeStartMove == -1f)
            {
                TimeStartMove = Time.ElapsedTime;
            }

            Vector3 newPos = startPos;
            newPos.Z += TravelDistance * MathHelper.Sin((/*Time.ElapsedTime -*/ TimeStartMove) * MoveSpeed);
            transformComp.Position = newPos;

            PosX = newPos.X;
            PosY = newPos.Y;
            PosZ = newPos.Z;
            TimePassed = TimeStartMove;

            TimeStartMove += Time.DeltaTime;
        }
    }
}
