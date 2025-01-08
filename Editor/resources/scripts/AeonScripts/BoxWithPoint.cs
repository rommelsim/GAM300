using AeonScriptCore;
using AeonScriptCore.Physics;

namespace AeonScripts
{
    public class BoxWithPoint : AeonBehaviour
    {
        [HideFromEditor]
        public string EntityOwnerName = "";
        public int PointScore = 1;

        private float collisionLeeway = 1.0f;
        private Transform transformComp = null;
        private AudioComponent audioComp = null;
        private RigidBody rbComp = null;
        private Collider colliderComp = null;
        private CubeColliderShape shapeCollider = null;

        private Entity particles = null;
        private Transform particlesTransform = null;

        private bool hasHitGroundOnce = false;
        private float gravTimer = 0.0f;

        Vector3 targetScale = new Vector3(5f,5f,5f);

        PickableObject pickableObject = null;

        private Entity basicArrow = null;
        private float timerToArrow = 3f;

        private Entity boxSpotLight = null;

        public override void OnCreate()
        {
            boxSpotLight = Entity.FindEntityByName("Box SpotLight");
            basicArrow = Entity.FindEntityByName("BasicArrow");
            pickableObject = Entity.FindEntityByName("Box.Trigger").GetScript<PickableObject>();
            particles = Entity.FindEntityByName("Box Sparkles");
            if (particles != null)
            {
                particlesTransform = particles.GetComponent<Transform>();
            }
            transformComp = entity.GetComponent<Transform>();
            audioComp = entity.GetComponent<AudioComponent>();
            rbComp = entity.GetComponent<RigidBody>();
            colliderComp = entity.GetComponent<Collider>();

            if (colliderComp != null && colliderComp.ShapeType == ColliderShapeType.CUBE) {
                shapeCollider = (CubeColliderShape)colliderComp.GetColliderShape();
            }
        }
        public override void Update()
        {
            transformComp.Scale = Vector3.Lerp(transformComp.Scale, targetScale, 5 * Time.DeltaTime);
            if (particlesTransform != null)
            {
                particles.GetComponent<Transform>().Position = transformComp.Position;
            }

            if(pickableObject.ownerScript == null)
            {
                timerToArrow -= Time.DeltaTime;
            }
            else
            {
                timerToArrow = 3f;
            }

            if(timerToArrow <= 0)
            {
                basicArrow.GetComponent<Transform>().Position = entity.GetComponent<Transform>().Position + new Vector3(0, 6, 0);
                basicArrow.GetComponent<Transform>().Rotation += new Vector3(0, 45, 0) * Time.DeltaTime;
            }
            else
            {
                basicArrow.GetComponent<Transform>().Position = new Vector3(0, 100, -100);
            }

            boxSpotLight.GetComponent<Transform>().Position = entity.GetComponent<Transform>().Position + new Vector3(0, 52f, 0);
        }

        private bool CheckIfGrounded()
        {
            Vector3 origin = transformComp.Position; //+ collider half extents, so bottom of the collider which is the feet
            float offsetFromCol = 0.3f; // i added some offset because its raycasting detecting the inside of the collider

            if (colliderComp != null && colliderComp.GetColliderShape() != null)
            {
                //cube? spehere? capsule?
                //origin += _colliderComponent.GetColliderShape()
                ColliderShape colliderShape = colliderComp.GetColliderShape();

                CubeColliderShape cubeShape = colliderShape as CubeColliderShape;
                if (cubeShape != null)
                {
                    Vector3 halfSize = cubeShape.HalfSize;
                    origin.Y -= (halfSize.Y + offsetFromCol);
                }

                SphereColliderShape sphereShape = colliderShape as SphereColliderShape;
                if (sphereShape != null)
                {
                    float radius = sphereShape.Radius;
                    origin.Y -= (radius + offsetFromCol);
                }

                CapsuleColliderShape capsuleShape = colliderShape as CapsuleColliderShape;
                if (capsuleShape != null)
                {
                    float radius = capsuleShape.Radius;
                    float halfHeight = capsuleShape.HalfHeight;
                    origin.Y -= (halfHeight + radius + offsetFromCol);
                }
            }
            Vector3 direction = new Vector3(0, -1, 0);     
            float maxDistance = 0.1f;

            RaycastResult result; //one day u might need this so i leave it here, basically in this case, it just checks if there is anything below the player thats all, it doesnt differentiate from ground or not
            bool hit = rbComp.Raycast(ref origin, ref direction, maxDistance, out result);

            return hit;
        }

        private void CheckCollision(uint otherEntityID)
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
                if (distSq > (diagonalLengthSq + otherDiagonalLengthSq + collisionLeeway)) return;
            }
        }

        public void ScoreIncreaseVisuals(Vector3 increaseSize)
        {
            transformComp.Scale = increaseSize;
            CreatePlusOne();
        }

        public void CreatePlusOne()
        {         
            EntityBuilder entBuilder = new EntityBuilder("Plus One");


            TransformBuilder transformBuilder = new TransformBuilder();
            transformBuilder.SetPosition(transformComp.Position + new Vector3(0, 10, 0));
            transformBuilder.SetScale(new Vector3(1.5f, 1.5f, 1.5f));


            BehaviourBuilder behaviourBuilder = new BehaviourBuilder();
            behaviourBuilder.AddScript<LookAtCamera>();
            behaviourBuilder.AddScript<RiseAndDestroy>();


            RenderBuilder renderBuilder = new RenderBuilder("./resources/objects/PlusOne.bin", "./resources/textures/plusOne.dds");


            entBuilder.AddComponent(transformBuilder);
            entBuilder.AddComponent(behaviourBuilder);
            entBuilder.AddComponent(renderBuilder);


            Entity.Create(entBuilder);
        }

        public void PlayScoreSFX(string soundFile)
        {
            if (audioComp == null) return;

            audioComp.SoundFilepath = soundFile;
            audioComp.ChannelType = Audio.ChannelType.SFX;
            audioComp.Volume = 0.6f;
            audioComp.PlaySound();
        }
    }
}
