using AeonScriptCore;
using AeonScriptCore.Physics;
using System.Collections.Generic;
using System.Resources;

namespace AeonScripts
{
    public class PickableObject : AeonBehaviour
    {
        private enum OwnerChangeState { NoRequest, NewOwner, RemoveOwner, Fulfilled };

        public string AttachedColliderName;
        public Vector3 CarryPosOffset = new Vector3 (0, 2, 0);
        public float dropDistance = 3.0f;
        public float SlowerMoveSpeed = 10.0f;

        private Transform transComp = null;
        private bool forcedDropByAbyss = false;

        private Entity colliderEntity = null;
        private Transform colliderTransform = null; // the transform of the collider entity
        private RigidBody colliderRb = null; // the rigid body of the collider entity
        private AudioComponent pickupSFX = null;

        private List<MoveCharacter> possibleOwners = new List<MoveCharacter>(); // used to determine whether a player has entered the trigger zone or not
        //private Transform ownerTransform = null; // the transform of the one carrying this object
        public MoveCharacter ownerScript = null;

        // OLD Usage: a dirty flag, to determine whether an owner change was detected, that is was the interact key pressed when a possible or existing owner is in the trigger area. This is to prevent the interact behaviour from occurring if multiple boxes are detecting the owner is in the area at the same time
        // New Usage: an enum value, NoRequest means no change made, NewOwner means there was a new owner made, NoOwner means the box is being dropped, Fulfilled means the change state is done (to tell frame end handler to set change state back to NoRequest
        private static OwnerChangeState _wasAnOwnerChanged = OwnerChangeState.NoRequest;

        // static constructor is invoked only when the first instance of the class is created or a static member of the class is referenced
        static PickableObject()
        {
            EventHandler.AddHandlerToFrameEndEvent(() =>
            {
                if (_wasAnOwnerChanged == OwnerChangeState.Fulfilled)
                    _wasAnOwnerChanged = OwnerChangeState.NoRequest;
            });
        }

        public override void OnCreate()
        {
            transComp = entity.GetComponent<Transform>();
            colliderEntity = Entity.FindEntityByName(AttachedColliderName);
            pickupSFX = entity.GetComponent<AudioComponent>();

            if (colliderEntity != null)
            {
                colliderTransform = colliderEntity.GetComponent<Transform>();
                colliderRb = colliderEntity.GetComponent<RigidBody>();
            }
        }

        public override void Update()
        {
            #region Null-safety checks
            if (colliderEntity == null)
                colliderEntity = Entity.FindEntityByName(AttachedColliderName);

            if (colliderEntity != null) {
                if (colliderRb == null)
                    colliderRb = entity.GetComponent<RigidBody>();
                if (colliderTransform == null)
                    colliderTransform = entity.GetComponent<Transform>();
            }

            if (colliderRb == null || colliderTransform == null) return;
            #endregion

            if ((_wasAnOwnerChanged == OwnerChangeState.NewOwner || _wasAnOwnerChanged == OwnerChangeState.RemoveOwner) && ownerScript != null)
            {
                if (_wasAnOwnerChanged == OwnerChangeState.NewOwner)
                {
                    SetCarryState(true);
                    if (ownerScript != null)
                    {
                        GameData.PlayerHoldingBox = ownerScript.isPlayerOne ? GameData.Player.Player1 : GameData.Player.Player2;
                        if (ownerScript.isPlayerOne)
                        {
                            if (pickupSFX != null)
                            {
                                pickupSFX.SoundFilepath = ".\\resources\\audios\\PickupBox_Red.wav";
                                pickupSFX.PlaySound();
                            }
                        }
                        else
                        {
                            if (pickupSFX != null)
                            {
                                pickupSFX.SoundFilepath = ".\\resources\\audios\\PickupBox_Blue.wav";
                                pickupSFX.PlaySound();
                            }
                        }
                    }
                }
                else
                {
                    NativeLog.Log("Dropping box");
                    Vector3 impulseForce = Vector3.ZERO;
                    if (!forcedDropByAbyss) {
                        int randoInt = GameData.random.Next(-1, 3);
                        int randoInt2 = GameData.random.Next(-1, 3);
                        if (randoInt <= 0)
                        {
                            impulseForce.X = -(float)GameData.random.NextDouble();
                        }
                        else
                        {
                            impulseForce.X = (float)GameData.random.NextDouble();
                        }

                        if(randoInt2 <= 0)
                        {
                            impulseForce.Z = -(float)GameData.random.NextDouble();
                        }
                        else
                        {
                            impulseForce.Z = (float)GameData.random.NextDouble();
                        }
                        
                        
                        impulseForce *= GameData.random.Next(30, 45); // scale the offset by how far to drop away from the player
                        impulseForce.Y = GameData.random.Next(4, 6) * 15;

                        /*Vector3 ownerFaceDir = ownerScript.GetFacingDirection();
                        Vector3 newPosition = ownerFaceDir;
                        {
                            newPosition *= dropDistance; // scale the offset by how far to drop away from the player
                            newPosition += ownerScript.GetTransform().Position; // then apply the owner pos

                            // offset via the collider shape
                            Vector3 posOffset = Vector3.ZERO;
                            switch (ownerScript.GetCollider().ShapeType)
                            {
                                case ColliderShapeType.CUBE:
                                    posOffset = ((CubeColliderShape)ownerScript.GetCollider().GetColliderShape()).HalfSize;
                                    posOffset.X *= (ownerFaceDir.X > 0 || ownerFaceDir.X < 0) ? 1.0f : 0.0f;
                                    posOffset.Z *= (ownerFaceDir.Z > 0 || ownerFaceDir.Z < 0) ? 1.0f : 0.0f;
                                    break;
                                case ColliderShapeType.SPHERE:
                                    float radius = ((SphereColliderShape)ownerScript.GetCollider().GetColliderShape()).Radius;
                                    posOffset = new Vector3(radius, 0, radius);
                                    posOffset.X *= (ownerFaceDir.X > 0 || ownerFaceDir.X < 0) ? 1.0f : 0.0f;
                                    posOffset.Z *= (ownerFaceDir.Z > 0 || ownerFaceDir.Z < 0) ? 1.0f : 0.0f;
                                    break;
                            }
                            posOffset.Y = 0;
                            newPosition += posOffset;
                        }
                        colliderTransform.Position = newPosition;*/

                        Vector3 newPos = colliderTransform.Position;
                        newPos.Y += 10.0f;
                        colliderTransform.Position = newPos;
                    }
                    SetCarryState(false);
                    ownerScript = null;

                    if (impulseForce != Vector3.ZERO)
                    {
                        NativeLog.Log("Impulse Force added");
                        colliderRb.AddImpulseForce(ref impulseForce);
                    }
                }
                _wasAnOwnerChanged = OwnerChangeState.Fulfilled;
            }

            {
                // if there is a possible owner and the pick up key is pressed, we pick it up
                if (ownerScript == null && possibleOwners.Count > 0)
                {
                    foreach (MoveCharacter move in possibleOwners)
                    {
                        //NativeLog.LogWarn("owner {0}, pressed {1}", move.entity.Name, move.WasInteractKeyPressed());
                        if (!move.IsCarryingPickable() && move.WasInteractKeyPressed())
                        {
                            ownerScript = move;
                            SetCarryState(true);
                            possibleOwners.Clear();
                            break;
                        }
                    }
                }
                // means this object is being carried
                else if (ownerScript != null)
                {
                    Collider ownerCollider = ownerScript.GetCollider();
                    Vector3 ownerPos = ownerScript.GetTransform().Position;
                    Vector3 carryPos = ownerPos + CarryPosOffset;

                    switch (ownerCollider.ShapeType)
                    {
                        case AeonScriptCore.Physics.ColliderShapeType.CUBE:
                            carryPos.Y += (ownerCollider.GetColliderShape() as CubeColliderShape).HalfSize.Y;
                            break;
                        case AeonScriptCore.Physics.ColliderShapeType.SPHERE:
                            carryPos.Y += (ownerCollider.GetColliderShape() as SphereColliderShape).Radius;
                            break;
                        case AeonScriptCore.Physics.ColliderShapeType.CAPSULE:
                        {
                            CapsuleColliderShape capsuleShape = ownerCollider.GetColliderShape() as CapsuleColliderShape;
                            carryPos.Y += capsuleShape.Radius + capsuleShape.HalfHeight;
                            break;
                        }
                    }

                    colliderTransform.Position = carryPos;
                    // Drop the object
                    //if (ownerScript.WasInteractKeyPressed())
                    //{
                    //    GameData.PlayerHoldingBox = GameData.Player.None;
                    //    Vector3 rotation = ownerScript.GetTransform().Rotation - ownerScript.GetBaseRotation();

                    //    float ownerRotY = rotation.Y * MathHelper.DegToRad; // convert the transform's rotation from deg to rad

                    //    /* To rotate our point (which can be inferred by the player's forward direction) by the Y-axis, in which we only need to care about x and z coordinates
                    //      x2 = x1 * cos  + z1 * sin
                    //      z2 = x1 * -sin + z1 * cos
                    //    */
                    //    Vector3 ownerForwardDir = ownerScript.forwardDir;
                    //    float cosResult = MathHelper.Cos(ownerRotY);
                    //    float sinResult = MathHelper.Sin(ownerRotY);

                    //    Vector3 newPosition = new Vector3(ownerForwardDir.X * cosResult + ownerForwardDir.Z * sinResult, 0.0f, ownerForwardDir.X * (-sinResult) + ownerForwardDir.Z * cosResult); // first, calculate the rotated point
                    //    newPosition *= dropDistance; // scale the offset by how far to drop away from the player
                    //    newPosition += ownerPos; // then apply the owner pos

                    //    colliderTransform.Position = newPosition;
                    //    SetCarryState(false);
                    //    ownerScript = null;
                    //}
                }
            }

            transComp.Position = colliderTransform.Position;
        }

        public override void OnTriggerEnter(uint triggerEntityID)
        {
            Entity otherEnt = Entity.GetEntity(triggerEntityID);
            MoveCharacter moveScript = otherEnt.GetScript<MoveCharacter>();

            if (otherEnt != null && moveScript != null)
            {
                //foreach (MoveCharacter move in possibleOwners)
                //    if (move == moveScript)
                //        return;

                //possibleOwners.Add(moveScript);
                if (!moveScript.IsCarryingPickable() && ownerScript == null && _wasAnOwnerChanged == OwnerChangeState.NoRequest) // if the character is not carrying a pickable object and this object has no owner
                {
                    ownerScript = moveScript;
                    _wasAnOwnerChanged = OwnerChangeState.NewOwner;
                    //SetCarryState(true);
                    NativeLog.LogWarn("Player entered pickable trigger {0}", entity.Name);
                }
            }
        }

        public override void OnTriggerExit(uint triggerEntityID)
        {
            //if (possibleOwners.Count > 0)
            //{
            //    for (int i = 0; i < possibleOwners.Count; i++)
            //    {
            //        if (possibleOwners[i].entity.Id == triggerEntityID)
            //        {
            //            NativeLog.LogWarn("Player exited pickable trigger {0}", entity.Name);
            //            possibleOwners.RemoveAt(i);
            //            break;
            //        }
            //    }
            //}
        }

        public Entity GetColliderBoxEntity()
        {
            return colliderEntity;
        }

        public void DropObject(bool forcedByAbyss)
        {
            if (ownerScript == null) return;

            _wasAnOwnerChanged = OwnerChangeState.RemoveOwner;
            forcedDropByAbyss = forcedByAbyss;
        }

        private void SetCarryState(bool isCarried)
        {
            //_wasAnOwnerChanged = true;

            if (colliderRb != null)
            {
                colliderRb.UseGravity = !isCarried;
                //colliderRb.IsKinematic = isCarried;
            }

            if (ownerScript != null)
            {
                ownerScript.pickableObject = isCarried ? this : null;

                if (isCarried)
                {
                    ownerScript.moveSpeed = SlowerMoveSpeed;
                    NativeLog.Log("Setting speed to {1} for {0}", ownerScript.entity.Name, ownerScript.moveSpeed);
                    GameData.PlayerHoldingBox = ownerScript.isPlayerOne ? GameData.Player.Player1 : GameData.Player.Player2;
                    colliderTransform.Position = ownerScript.GetTransform().Position + CarryPosOffset;
                }
                else
                {
                    ownerScript.moveSpeed = ownerScript.OgMoveSpeed;
                    GameData.PlayerHoldingBox = GameData.Player.None;
                }
            }
        }
    }
}
