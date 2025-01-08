using AeonScriptCore;
using AeonScriptCore.Physics;
using System;
using System.Collections.Generic;

namespace AeonScripts
{
    public class MoveCharacter : AeonBehaviour
    {
        public Vector3 forwardDir = new Vector3(0, 0, 1);
        //public Vector3 forwardFacingRotation = Vector3.ZERO;
        public float jumpForce = 4.0f;
        public float moveSpeed = 3.0f;
        public bool isPlayerOne = false;
        public Vector3 shadowPos;

        [HideFromEditor]
        public Vector3 PlayerOgScale { get; private set; }
        [HideFromEditor]
        public float OgMoveSpeed = 0.0f; // only modify the original move speed if you absolutely know what you are doing

        public bool IsScriptActive = true;
        public PickableObject pickableObject = null;

        private Transform transform = null;
        private RigidBody rbComp = null;
        private Collider colliderComp = null;
        private AudioComponent audioComp = null;

		[HideFromEditor]
		public Vector3 _baseRotation = Vector3.ZERO;
        private Vector3 _prevRotation = Vector3.ZERO;
        private Vector3 _facingDir = Vector3.ZERO;

        private static Vector3 _upDir = new Vector3(0, 1, 0);
        private Vector3 _rightDir;
        private float _prevPosY;
        private bool _movedLastFrame = false;

        // Input bool detection
        private bool useKeyboard = true;
        private bool moveForward = false;
        private bool moveBackward = false;
        private bool moveLeft = false;
        private bool moveRight = false;

        private bool wasXZImpulseForceAdded = false;
        private Vector3 xzImpulseForce = Vector3.ZERO;

        private static List<Tuple<RigidBody, Vector3>> pushBoxes = new List<Tuple<RigidBody, Vector3>>();
        private static List<RigidBody> stopBoxes = new List<RigidBody>();

        //public Vector3 shadowPos;

        //checkpoint 
        public string currentCheckpoint = " ";

        public override void OnCreate()
        {
            transform = entity.GetComponent<Transform>();
            rbComp = entity.GetComponent<RigidBody>();
            colliderComp = entity.GetComponent<Collider>();
            audioComp = entity.GetComponent<AudioComponent>();

            OgMoveSpeed = moveSpeed;

            if (transform != null)
            {
                PlayerOgScale = transform.Scale;
                _prevRotation = _baseRotation = transform.Rotation;
                NativeLog.Log("CharacterController onCreate");
            }
            //checkpoint
            if (isPlayerOne)
            {
                currentCheckpoint = "Checkpoint1.Red";
            }
            else
            {
                currentCheckpoint = "Checkpoint1.Blue";
            }

            CalculateFacingProps(_baseRotation);
            NativeLog.Log("P1, P2: {0}, {1}", GameInputs.PlayerOneInput.ToString(), GameInputs.PlayerTwoInput.ToString());
        }

        public override void Update()
        {
            if (GameState.IsPaused) return;

            if (wasXZImpulseForceAdded)
            {
                rbComp.AddImpulseForce(ref xzImpulseForce);
                wasXZImpulseForceAdded = false;
            }

            shadowPos = shadowCheck();
            //fakeShadow.Position = test;

            if (!IsScriptActive) return;

            //_baseRotation = forwardFacingRotation;
            _rightDir = Vector3.Cross(forwardDir, _upDir);
            
            useKeyboard = !GameInputs.IsPlayerController(isPlayerOne);
            float forwardBackAxis = GameInputs.GetPlayerGamepadAxisValue(GameInputs.Map_Forward, isPlayerOne);
            float leftRightAxis = GameInputs.GetPlayerGamepadAxisValue(GameInputs.Map_Left, isPlayerOne);

            moveForward = false;
            moveBackward = false;
            moveLeft = false;
            moveRight = false;
            bool shouldJump = false;

            if (useKeyboard)
            {
                moveForward = isPlayerOne || GameInputs.PlayerOneInput == GameInputs.InputType.Controller ? Input.GetMappingKeyHeld(GameInputs.Map_Forward) : Input.GetMappingKeyHeld(GameInputs.Map_Forward + GameInputs.Alt_Map_Suffix);
                moveBackward = isPlayerOne || GameInputs.PlayerOneInput == GameInputs.InputType.Controller ? Input.GetMappingKeyHeld(GameInputs.Map_Backward) : Input.GetMappingKeyHeld(GameInputs.Map_Backward + GameInputs.Alt_Map_Suffix);
                moveLeft = isPlayerOne || GameInputs.PlayerOneInput == GameInputs.InputType.Controller ? Input.GetMappingKeyHeld(GameInputs.Map_Left) : Input.GetMappingKeyHeld(GameInputs.Map_Left + GameInputs.Alt_Map_Suffix);
                moveRight = isPlayerOne || GameInputs.PlayerOneInput == GameInputs.InputType.Controller ? Input.GetMappingKeyHeld(GameInputs.Map_Right) : Input.GetMappingKeyHeld(GameInputs.Map_Right + GameInputs.Alt_Map_Suffix);
                shouldJump = isPlayerOne || GameInputs.PlayerOneInput == GameInputs.InputType.Controller ? Input.GetMappingKeyPressed(GameInputs.Map_Jump) : Input.GetMappingKeyPressed(GameInputs.Map_Jump + GameInputs.Alt_Map_Suffix);
            }
            else
            {
                moveForward = forwardBackAxis < 0;
                moveBackward = forwardBackAxis > 0;
                moveLeft = leftRightAxis < 0;
                moveRight = leftRightAxis > 0;
                shouldJump = GameInputs.IsPlayerGamepadButtonPressed(GameInputs.Map_Jump, isPlayerOne);
            }

            //useKeyboard = !GameInputs.IsPlayerController(isPlayerOne) || (!isPlayerOne && GameInputs.PlayerOneInput == GameInputs.InputType.Controller);

            // Do translation
            Vector3 dir;
            Vector3 newRot;
            bool keyPressed = SetMoveDirectionRotation(out dir, out newRot, forwardBackAxis, leftRightAxis);

            if (shouldJump && rbComp != null && CheckIfGrounded())
            {
                NativeLog.Log("We jumped! {0}, {1}", entity.Id, colliderComp.EntityId);
                Vector3 jumpVec = Vector3.ZERO;
                jumpVec.Y = jumpForce;
                rbComp.AddImpulseForce(ref jumpVec);
                PlayJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJump();
            }

            // Only when keypress was detected, then we set pos and rot
            if (keyPressed)
            {
                // Set the position
                if (rbComp != null)
                {
                    dir *= moveSpeed;
                    dir.Y += rbComp.Velocity.Y;
                    //rbComp.AddForce(ref dir);
                    rbComp.Velocity = dir;
                }
                else
                {
                    Vector3 newPos = transform.Position;
                    newPos += dir * (moveSpeed * Time.DeltaTime);
                    transform.Position = newPos;
                }

                Vector3 newRotation = _baseRotation + newRot;
                transform.Rotation = newRotation;

                if (_prevRotation != newRotation)
                {
                    CalculateFacingProps(newRotation);
                }

                _prevRotation = newRotation;
                _movedLastFrame = true;
            }
            else if (_movedLastFrame)
            {
                if (rbComp != null)
                {
                    //rbComp.Velocity = Vector3.ZERO;
                }
                _movedLastFrame = false;
            }

            _prevPosY = transform.Position.Y;

            // Fulfill push boxes
            for (int i = pushBoxes.Count - 1; i >= 0; i--) {
                var item = pushBoxes[i];
                Vector3 pushForce = item.Item2;
                item.Item1.AddForce(ref pushForce);

                pushBoxes.RemoveAt(i);
            }

            for (int i = stopBoxes.Count - 1; i >= 0; i--)
            {
                stopBoxes[i].Velocity = Vector3.ZERO;
                stopBoxes.RemoveAt(i);
            }
        }

        public override void OnCollisionEnter(uint otherEntityID)
        {
            //if (rbComp == null) return;

            //Entity otherEnt = Entity.GetEntity(otherEntityID);
            //if (otherEnt == null) return;

            //BoxWithPoint boxPoint = otherEnt.GetScript<BoxWithPoint>();
            //if (boxPoint == null) return;

            //RigidBody otherRbComp = otherEnt.GetComponent<RigidBody>();
            //if (otherRbComp == null) return;

            //Vector3 pushDirection = rbComp.Velocity;
            //pushDirection.Y = 0;
            //float pushForceMagnitude = pushDirection.GetMagnitude() * rbComp.Mass;

            //Vector3 normPushDir = Vector3.Normalize(pushDirection);
            //Vector3 newForce = new Vector3(normPushDir.X * pushForceMagnitude, normPushDir.Y * pushForceMagnitude,
            //    normPushDir.Z * pushForceMagnitude);
            //pushBoxes.Add(new Tuple<RigidBody, Vector3>(otherRbComp, newForce));

            //NativeLog.LogWarn("Player collided with Box of point {0}", boxPoint);
        }

        public override void OnCollisionExit(uint otherEntityID)
        {
            //Entity otherEnt = Entity.GetEntity(otherEntityID);
            //if (otherEnt == null) return;

            //BoxWithPoint boxPoint = otherEnt.GetScript<BoxWithPoint>();
            //if (boxPoint == null) return;

            //RigidBody otherRbComp = otherEnt.GetComponent<RigidBody>();
            //if (otherRbComp == null) return;

            ////otherRbComp.Velocity = Vector3.ZERO;
            //stopBoxes.Add(otherRbComp);
        }

        private bool CheckIfGrounded()
        {
            Vector3 origin = transform.Position; //+ collider half extents, so bottom of the collider which is the feet
			float offsetFromCol = 0.1f; // i added some offset because its raycasting detecting the inside of the collider

			if (colliderComp != null && colliderComp.GetColliderShape() != null)
            {
                //cube? spehere? capsule?
                origin += colliderComp.Offset;
                //if (isPlayerOne)
                //    NativeLog.LogError("Player offset:{0}", colliderComp.Offset);
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

            if (hit && Entity.IsAlive(result.id) && result.id != entity.Id)
            {
                Entity hitEnt = Entity.GetEntity(result.id);
                if (hitEnt != null && hitEnt.HasComponent<RigidBody>() && hitEnt.HasComponent<Collider>())
                {
                    Collider collider = hitEnt.GetComponent<Collider>();
                    if (!collider.IsTrigger)
                    {
                        //NativeLog.Log("Move Jump Raycast Hit:{0}", hitEnt.Name);
                        return true;
                    }
                }
            }

            return false;
        }

        public Vector3 shadowCheck()
        {

            Vector3 origin = transform.Position; //+ collider half extents, so bottom of the collider which is the feet
                                                 //float offsetFromCol = 0.5f; // i added some offset because its raycasting detecting the inside of the collider
            float offsetFromCol = 1.5f; // i added some offset because its raycasting detecting the inside of the collider

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
            float maxDistance = 10f;

            RaycastResult result; //one day u might need this so i leave it here, basically in this case, it just checks if there is anything below the player thats all, it doesnt differentiate from ground or not
            bool hit = rbComp.Raycast(ref origin, ref direction, maxDistance, out result);

            return result.HitPosition;

        }

        private void CalculateFacingProps(Vector3 currRotation)
        {
            Vector3 rotation = currRotation - GetBaseRotation(); // get the "true" rotation, that is without "bias" from the base rotation

            float charRotY = rotation.Y * MathHelper.DegToRad; // convert the transform's rotation from deg to rad

            Vector3 ownerForwardDir = forwardDir;
            float cosResult = MathHelper.Cos(charRotY);
            float sinResult = MathHelper.Sin(charRotY);

            _facingDir = new Vector3(ownerForwardDir.X * cosResult + ownerForwardDir.Z * sinResult, 0.0f, ownerForwardDir.X * (-sinResult) + ownerForwardDir.Z * cosResult); // first, calculate the rotated point
        }

        private bool SetMoveDirectionRotation(out Vector3 moveDir, out Vector3 charRotation, float forwardBackwardAxis, float leftRightAxis)
        {
            bool didMove = false;
            moveDir = Vector3.ZERO;
            charRotation = Vector3.ZERO;

            if (useKeyboard)
            {
                if (moveForward) // Move forward
                {
                    //NativeLog.Log("MoveForward");
                    moveDir += forwardDir;
                    didMove = true;
                }
                if (moveBackward) // Move backwards
                {
                    moveDir -= forwardDir;
                    charRotation.Y = 180.0f; // turn about
                    didMove = true;
                }
                if (moveLeft) // Move left
                {
                    moveDir -= _rightDir;
                    charRotation.Y = 90.0f; // rotate left is rotate CCW, so +ve rotation
                    didMove = true;
                }
                if (moveRight) // move right
                {
                    moveDir += _rightDir;
                    charRotation.Y = -90.0f; // rotate right is rotate CW, so -ve rotation
                    didMove = true;
                }

                if (moveForward && moveLeft) // Move up-left
                {
                    charRotation.Y = 45.0f;
                }
                else if (moveForward && moveRight) // Move up-right
                {
                    charRotation.Y = -45.0f;
                }
                else if (moveBackward && moveLeft) // Move down-left
                {
                    charRotation.Y = 135.0f;
                }
                else if (moveBackward && moveRight) // Move down-right
                {
                    charRotation.Y = -135.0f;
                }
            }
            else if (moveForward || moveBackward || moveLeft || moveRight) // as long as controller had movement...
            {
                didMove = true;
                NativeLog.Log("Forward-Back, Left-Right axes: {0}, {1}", forwardBackwardAxis, leftRightAxis);
                NativeLog.Log("Moved - Forward, Back, Left, Right: {0}, {1}, {2}, {3}", moveForward, moveBackward, moveLeft, moveRight);

                // One direction movement only
                if (moveForward)
                {
                    moveDir += forwardDir * MathHelper.Abs(forwardBackwardAxis);
                }
                else if (moveBackward)
                {
                    moveDir -= forwardDir * MathHelper.Abs(forwardBackwardAxis);
                    charRotation.Y = 180.0f;
                }

                if (moveLeft)
                {
                    moveDir -= _rightDir * MathHelper.Abs(leftRightAxis);
                    charRotation.Y = 90.0f;
                }
                else if (moveRight)
                {
                    moveDir += _rightDir * MathHelper.Abs(leftRightAxis);
                    charRotation.Y = -90.0f;
                }

                // Two-direction movement
                if (moveForward && (moveLeft || moveRight))
                {
                    charRotation.Y = MathHelper.ATan(leftRightAxis / forwardBackwardAxis);
                    charRotation.Y *= MathHelper.RadToDeg;
                }
                else if (moveBackward && (moveLeft || moveRight))
                {
                    if (moveLeft)
                        charRotation.Y = MathHelper.PI_Half + -MathHelper.ATan(forwardBackwardAxis / leftRightAxis);
                    else
                        charRotation.Y = MathHelper.PI + MathHelper.ATan(leftRightAxis / forwardBackwardAxis);

                    charRotation.Y *= MathHelper.RadToDeg;
                }
            }

            return didMove;
        }

        public Vector3 GetForwardDir()
        {
            return forwardDir;
        }

        /// <summary>
        /// Get the direction that the player is facing at. It is NOT the SAME as Forward Direction, Forward Direction means what direction the player will face when pressing the MoveForward input mapping
        /// </summary>
        /// <returns></returns>
        public Vector3 GetFacingDirection()
        {
            return _facingDir;
        }

        public Transform GetTransform()
        {
            return transform;
        }

        public Collider GetCollider()
        {
            return colliderComp;
        }

        public RigidBody GetRigidbody()
        {
            return rbComp;
        }
        public Vector3 GetBaseRotation()
        {
            return _baseRotation;
        }

        /// <summary>
        /// Check if the player is carrying a pickable object or not
        /// </summary>
        /// <returns>A boolean, true or false</returns>
        public bool IsCarryingPickable()
        {
            return pickableObject != null;
        }

        public bool WasInteractKeyPressed()
        {
            if (GameState.IsPaused) return false;
            if (GameInputs.IsPlayerController(isPlayerOne))
            {
                //return Input.GetMappingKeyPressed(Map_Interact);
                return GameInputs.IsPlayerGamepadButtonPressed(GameInputs.Map_Interact, isPlayerOne);
            }
            else
            {
                if (isPlayerOne || GameInputs.PlayerTwoInput == GameInputs.InputType.Controller)
                {
                    return Input.GetMappingKeyPressed(GameInputs.Map_Interact);
                }
                return Input.GetMappingKeyPressed(GameInputs.Map_Interact + GameInputs.Alt_Map_Suffix);
            }
        }

        public bool WasPermKeyPress()
        {
            if (GameState.IsPaused) return false;
            if (GameInputs.IsPlayerController(isPlayerOne))
            {
                //return Input.GetMappingKeyPressed(Map_Interact);
                return GameInputs.IsPlayerGamepadButtonPressed(GameInputs.Map_PermInteract, isPlayerOne);
            }
            else
            {
                if (isPlayerOne || GameInputs.PlayerTwoInput == GameInputs.InputType.Controller)
                {
                    return Input.GetMappingKeyPressed(GameInputs.Map_PermInteract);
                }
                return Input.GetMappingKeyPressed(GameInputs.Map_PermInteract + GameInputs.Alt_Map_Suffix);
            }
        }

        public void DropPickable(bool forcedByAbyss=false)
        {
            if (pickableObject == null) return;

            pickableObject.DropObject(forcedByAbyss);
        }

        public void AddXZImpulseForce(Vector3 impulseForce)
        {
            if (rbComp == null)
                return;

            impulseForce.Y = 0;
            xzImpulseForce = impulseForce;
            wasXZImpulseForceAdded = true;
            //NativeLog.Log("Adding XZ Impulse Force");
        }

        public void PlayHitByWeight()
        {
            if (audioComp != null)
            {
                int randPlay = GameData.random.Next(1, 101);
                if (randPlay % 4 == 0)
                {
                    if (isPlayerOne)
                    {
                        //audioComp.SoundFilepath = ".\\resources\\audios\\Red Cat_Get_Hit_5.wav";
                        int randNum = GameData.random.Next(1, 12);
                        audioComp.SoundFilepath = $".\\resources\\audios\\Red Cat_Get_Hit_{randNum}.wav";
                        audioComp.Volume = 0.6f;
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        //NativeLog.LogWarn("Did it play Unflatten sound");
                        audioComp.PlaySound();
                    }
                    else
                    {
                        //audioComp.SoundFilepath = ".\\resources\\audios\\Blue Cat_Get_Hit_06.wav";
                        int randNum = GameData.random.Next(1, 10);
                        audioComp.SoundFilepath = $".\\resources\\audios\\Blue Cat_Get_Hit_0{randNum}.wav";
                        audioComp.Volume = 0.6f;
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        //NativeLog.LogWarn("Did it play Unflatten sound");
                        audioComp.PlaySound();
                    }
                }
                
                audioComp.ChannelType = Audio.ChannelType.Voice;
                audioComp.Volume = 0.7f;
                audioComp.PlaySound();
            }
        }

        public void PlayHitByHammer()
        {
            if (audioComp != null)
            {
                int randPlay = GameData.random.Next(1, 101);
                if (randPlay % 5 == 1)
                {
                    if (isPlayerOne)
                    {
                        int randNum = GameData.random.Next(1, 12);
                        audioComp.SoundFilepath = $".\\resources\\audios\\Red Cat_Get_Hit_{randNum}.wav";
                        audioComp.Volume = 0.6f;
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        //NativeLog.LogWarn("Did it play Unflatten sound");
                        audioComp.PlaySound();
                    }
                    else
                    {
                        int randNum = GameData.random.Next(1, 10);
                        //audioComp.SoundFilepath = ".\\resources\\audios\\Red Cat_Get_Hit_2.wav";
                        audioComp.SoundFilepath = $".\\resources\\audios\\Blue Cat_Get_Hit_0{randNum}.wav";
                        audioComp.Volume = 0.6f;
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        //NativeLog.LogWarn("Did it play Unflatten sound");
                        audioComp.PlaySound();
                    }
                }
            }
        }

        public void PlayPlayerHittingSomething()
        {
            if (audioComp != null)
            {
                int randPlay = GameData.random.Next(1, 101);
                if (randPlay % 5 == 1)
                {
                    if (isPlayerOne)
                    {
                        int randNum = GameData.random.Next(1, 6);
                        audioComp.SoundFilepath = $".\\resources\\audios\\Red Cat_Hit_{randNum}.wav";
                        audioComp.Volume = 0.6f;
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        //NativeLog.LogWarn("Did it play Unflatten sound");
                        audioComp.PlaySound();
                    }
                    else
                    {
                        int randNum = GameData.random.Next(1, 6);
                        //audioComp.SoundFilepath = ".\\resources\\audios\\Red Cat_Get_Hit_2.wav";
                        audioComp.SoundFilepath = $".\\resources\\audios\\Blue Cat_Hit_{randNum}.wav";
                        audioComp.Volume = 0.6f;
                        audioComp.ChannelType = Audio.ChannelType.SFX;
                        //NativeLog.LogWarn("Did it play Unflatten sound");
                        audioComp.PlaySound();
                    }
                }
            }
        }

        public void PlayFallingSound()
        {
            if (audioComp != null)
            {
                int randNum = GameData.random.Next(1,5);
                if (isPlayerOne)
                {
                    audioComp.SoundFilepath = $".\\resources\\audios\\Red Cat_Falling_0{randNum}.wav";
                }
                else
                {
                    audioComp.SoundFilepath = $".\\resources\\audios\\Blue Cat_Falling_0{randNum}.wav";
                }
                audioComp.ChannelType = Audio.ChannelType.Voice;
                audioComp.Volume = 0.7f;
                audioComp.PlaySound();
            }
        }

        public void PlayJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJumpJump()
        {
            if (audioComp != null)
            {
                int randNum = GameData.random.Next(1, 5);
                audioComp.SoundFilepath = $".\\resources\\audios\\Jump{randNum}.wav";
                audioComp.ChannelType = Audio.ChannelType.SFX;
                audioComp.Volume = 0.4f;
                audioComp.PlaySound();
            }
        }

        
    }
}