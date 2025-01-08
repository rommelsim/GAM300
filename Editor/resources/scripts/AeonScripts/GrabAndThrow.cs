using AeonScriptCore;

namespace AeonScripts
{
    public class GrabAndThrow : AeonBehaviour
    {
        public Vector3 ThrowForce;

        private Entity grabbedEntity = null;
        private Vector3 Velocity = Vector3.ZERO;
        private Transform transform = null;

        public override void OnCreate()
        {
            transform = entity.GetComponent<Transform>();
        }

        public override void Update()
        {
            if (Input.GetKeyPressed(Input.KEY_R))
            {
                Velocity = ThrowForce;
            }

        }
    }
}
