using AeonScriptCore;

namespace AeonScripts
{
    public class PhysicsReaction : AeonBehaviour
    {
        public override void OnCollisionEnter(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            NativeLog.LogError("OnCollisionEnter: Entity {0} and other entity {1}", entity.Name, otherEnt.Name);
            NativeLog.LogError("{} had a collision with {}", entity.Name, otherEnt.Name);
        }

        public override void OnCollisionUpdate(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            NativeLog.LogWarn("OnCollisionUpdate: Entity {0} and other entity {1}", entity.Name, otherEnt.Name);
            NativeLog.LogError("{} is colliding with {}", entity.Name, otherEnt.Name);
        }

        public override void OnCollisionExit(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            NativeLog.LogError("OnCollisionExit: Entity {0} and other entity {1}", entity.Name, otherEnt.Name);
        }

        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            NativeLog.LogError("OnTriggerEnter: Entity {0} and other entity {1}", entity.Name, otherEnt.Name);
        }

        //public override void OnTriggerUpdate(uint triggerEntityID)
        //{
        //    Entity otherEnt = Entity.GetEntity(triggerEntityID);
        //    NativeLog.LogWarn("OnTriggerUpdate: Entity {0} and other entity {1}", entity.Name, otherEnt.Name);
        //}

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            NativeLog.LogError("OnTriggerExit: Entity {0} and other entity {1}", entity.Name, otherEnt.Name);
        }
    }
}
