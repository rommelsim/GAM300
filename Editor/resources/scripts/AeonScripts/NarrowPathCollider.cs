using AeonScriptCore;

namespace AeonScripts
{
    public class NarrowPathCollider : AeonBehaviour
    {
        [HideFromEditor]
        public Entity entP1 = null;
        [HideFromEditor]
        public Entity entP2 = null;

        public override void OnCollisionEnter(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            if (otherEnt != null && otherEnt.GetScript<MoveCharacter>() != null)
            {
                if (otherEnt.Name.EndsWith("1"))
                {
                    entP1 = otherEnt;
                }
                else
                {
                    entP2 = otherEnt;
                }
                NativeLog.LogWarn("NarrowPathCollider: Set player instances");
            }
        }

        public override void OnCollisionExit(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            if (otherEnt != null && otherEnt.GetScript<MoveCharacter>() != null)
            {
                if (otherEnt.Name.EndsWith("1"))
                {
                    entP1 = otherEnt;
                }
                else
                {
                    entP2 = otherEnt;
                }
                NativeLog.LogWarn("NarrowPathCollider: Reset player instances");
            }
        }
    }
}
