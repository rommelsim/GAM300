using AeonScriptCore;

namespace AeonScripts
{
    public class PowerUpSpin : AeonBehaviour
    {
        private Transform transformComp = null;

        private Vector3 initialPosition = new Vector3();
        private bool rising = true;

        public override void OnCreate()
        {
            transformComp = entity.GetComponent<Transform>();
            initialPosition = transformComp.Position;
        }

        public override void Update()
        {
            if (GameState.IsPaused) return;
            transformComp.Rotation += new Vector3(0, 20f, 0) * Time.DeltaTime;
            

            if (transformComp.Rotation.Y >= 360f)
            {
                transformComp.Rotation = new Vector3(0, 0, 0);
            }

            if (rising)
            {
                transformComp.Position += new Vector3(0, 1f, 0) * Time.DeltaTime;

                if (transformComp.Position.Y >= (initialPosition.Y + 1.5f))
                {
                    rising = false;
                }
            }
            else
            {
                transformComp.Position -= new Vector3(0, 1f, 0) * Time.DeltaTime;

                if (transformComp.Position.Y <= (initialPosition.Y - 1.5f))
                {
                    rising = true;
                }
            }
        }
    }
}
