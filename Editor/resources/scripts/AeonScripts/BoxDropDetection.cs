using AeonScriptCore;
using AeonScriptCore.Physics;

namespace AeonScripts
{
    public class BoxDropDetection : AeonBehaviour
    {
        private bool hasIntersection = false;

        private Transform transformComp = null;
        private Collider colliderComp = null;
        private CubeColliderShape shapeCollider = null;

        public override void OnCreate()
        {
            transformComp = entity.GetComponent<Transform>();
            colliderComp = entity.GetComponent<Collider>();

            if (colliderComp != null && colliderComp.ShapeType == ColliderShapeType.CUBE) {
                shapeCollider = (CubeColliderShape)colliderComp.GetColliderShape();
            }
        }

        public override void OnTriggerEnter(uint otherEntityID)
        {
            Entity otherEnt = Entity.GetEntity(otherEntityID);
            NativeLog.LogError("OnTriggerEnter: Entity {0} and other entity {1}", entity.Name, otherEnt.Name);
            //CheckCollision(otherEntityID);

            hasIntersection = true;
        }

        public override void OnTriggerExit(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            NativeLog.LogError("OnTriggerExit: Entity {0} and other entity {1}", entity.Name, otherEnt.Name);
        }

        public override void Update()
        {
            if (hasIntersection)
            {
                DoSomething();
            }
        }

        private void CheckIntersection(uint otherEntityID)
        {
            Entity otherEntity = Entity.GetEntity(otherEntityID);
            
            if (otherEntity.Name == "Box" && transformComp != null && shapeCollider != null)
            {
                Transform otherTransform = otherEntity.GetComponent<Transform>();
                if (otherTransform == null) return;

                Collider otherCollider = otherEntity.GetComponent<Collider>();
                if (otherCollider.ShapeType != ColliderShapeType.CUBE) return;
                CubeColliderShape otherShapeCollider = (CubeColliderShape)otherCollider.GetColliderShape();

                float distSq = Vector3.DistSq(otherTransform.Position, transformComp.Position);

                Vector3 collideShapeSize = shapeCollider.HalfSize;
                Vector3 otherShapeSize = otherShapeCollider.HalfSize;
                float diagonalLengthSq = collideShapeSize.X * collideShapeSize.X + collideShapeSize.Z * collideShapeSize.Z;
                float otherDiagonalLengthSq = otherShapeSize.X * otherShapeSize.X + otherShapeSize.Z * otherShapeSize.Z;

                // + 2.0f to give some room
                if (distSq > (diagonalLengthSq + otherDiagonalLengthSq + 2.0f)) return;

                DoSomething();

                hasIntersection = true;
            }
        }

        private void DoSomething()
        {
        }
    }
}
