using AeonScriptCore;
using AeonScriptCore.Physics;

namespace AeonScripts
{
    public class EqTestScript : AeonBehaviour
    {
        static int counter = 0;
        Collider colliderComp = null;
        CubeColliderShape cubeCollider = null;
        SphereColliderShape sphereCollider = null;

        public override void OnCreate()
        {
            colliderComp = entity.GetComponent<Collider>();

            if (colliderComp != null )
            {
                var colliderShape = colliderComp.GetColliderShape();
                if (colliderShape != null)
                {
                    if (colliderShape is CubeColliderShape)
                    {
                        cubeCollider = (CubeColliderShape)colliderShape;
                    }
                    else if (colliderShape is SphereColliderShape)
                    {
                        sphereCollider = (SphereColliderShape)colliderShape;
                    }
                }
            }
        }

        public override void Update()
        {
            if (Input.GetKeyPressed(Keys.KEY_V))
            {
                // NOTE: If you do not need add a TransformBuilder, the ScriptCore will automatically add a Transform component for you with all values set to 0

                // Construct an EntityBuilder and add on to it using the AddComponent functions that the EntityBuilder has
                EntityBuilder entBuilder = new EntityBuilder($"CSharp Ent {counter}")
                        .AddComponent(new BehaviourBuilder().AddScript<MoveCharacter>().AddScript<PickableObject>().AddScript<CameraLookAtSpot>()) // For each script to add, you need to call AddScript<SomeScriptClass>()
                        .AddComponent(new RigidBodyBuilder().SetUseGravity(true).SetType(AeonScriptCore.Physics.RigidBodyType.DYNAMIC));

                TransformBuilder transform = new TransformBuilder();
                ColliderBuilder collider = new ColliderBuilder().SetOffet(new Vector3(counter));

                // Capsule collider is not working at the moment, it crashes on PhysX side, so disabled for now
                switch (counter%2)
                {
                    case 0:
                        transform.SetScale(Vector3.ONE * 3).SetPosition(new Vector3(2 * counter, 0, 0));
                        collider.SetShape(new ColliderBuilder.CubeShapeBuilder());
                        entBuilder.AddComponent(new RenderBuilder("./resources/objects/balloon.bin", "./resources/textures/container.dds"));
                        break;
                    case 1:
                        collider.SetShape(new ColliderBuilder.SphereShapeBuilder());
                        transform.SetScale(Vector3.ONE * 100).SetPosition(new Vector3(-800 + counter * 120, -400, 0));
                        entBuilder.AddComponent(new UIBuilder().SetFormat(FormatType.Image).SetTexture("./resources/textures/contestant1winner.dds"));
                        break;
                    default:break;
                }
                entBuilder.AddComponent(collider);
                entBuilder.AddComponent(transform);

                // NOTE that you cannot add the same component type TWICE!. This one will be ignored and not added to the components list
                entBuilder.AddComponent(new TransformBuilder().SetScale(new Vector3(counter + 1)));

                /* FINAL step: sending the entity builder information to a create queue to be created at the end of the frame.
                   The second parameter allows you to execute code to set script variables or whatever AFTER the creation of entity and its components.
                     This is particularly important as you may want to set a script's variables BEFORE a script's onCreate() is ran
                */
                Entity.Create(entBuilder, (entity) => {
                    // You should see in the Editor, MoveCharacter's moveSpeed reflect the counter number (+1), which you can observe from the created entity's name
                    MoveCharacter moveChar = entity.GetScript<MoveCharacter>();

                    // For sanity's sake, do a null safety check
                    if (moveChar != null)
                    {
                        NativeLog.LogWarn("Setting newly created entity's MoveCharacter variables");
                        moveChar.moveSpeed = counter;
                    }
                });
                counter++;
            }

            if (Input.GetKeyPressed(Keys.KEY_R))
            {
                if (cubeCollider != null)
                    cubeCollider.HalfSize += Vector3.ONE;
                else if (sphereCollider != null)
                    sphereCollider.Radius += 1.0f;
            }

            if (Input.GetKeyPressed(Keys.KEY_ESCAPE))
            {
                GameState.IsPaused = !GameState.IsPaused;
            }
        }
    }
}
