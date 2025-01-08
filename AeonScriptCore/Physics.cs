/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	Physics.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines Physics-related types such as
            ColliderType and Geometry classes
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/

namespace AeonScriptCore.Physics
{
    public enum RigidBodyType
    {
        STATIC, DYNAMIC
    }

    public enum ColliderShapeType
    {
        CUBE,
        SPHERE,
        CAPSULE,
        NONE
    }

    public class ColliderShape
    {
        protected uint _entityId;

        internal ColliderShape(uint entityId)
        {
            this._entityId = entityId;
        }
    }

    public class CubeColliderShape : ColliderShape
    {
        internal CubeColliderShape(uint entityId) : base(entityId) {}

        /// <summary>
        /// The half-size dimensions of the cube shape
        /// </summary>
        public Vector3 HalfSize
        {
            get
            {
                InternalCalls.ColliderComponent_CubeGeom_GetHalfExtentSize(_entityId, out Vector3 outSize);
                return outSize;
            }
            set
            {
                InternalCalls.ColliderComponent_CubeGeom_SetHalfExtentSize(_entityId, ref value);
            }
        }
    }

    public class SphereColliderShape : ColliderShape
    {
        internal SphereColliderShape(uint entityId) : base(entityId) {}

        /// <summary>
        /// The radius of the sphere shape
        /// </summary>
        public float Radius
        {
            get
            {
                return InternalCalls.ColliderComponent_SphereGeom_GetRadius(_entityId);
            }
            set
            {
                InternalCalls.ColliderComponent_SphereGeom_SetRadius(_entityId, value);
            }
        }
    }

    public class CapsuleColliderShape : ColliderShape
    {
        internal CapsuleColliderShape(uint entityId) : base(entityId) {}

        /// <summary>
        /// The radius of the half-sphere end of the capsule shape
        /// </summary>
        public float Radius
        {
            get
            {
                return InternalCalls.ColliderComponent_CapsuleGeom_GetRadius(_entityId);
            }
        }


        public float HalfHeight
        {
            get
            {
                return InternalCalls.ColliderComponent_CapsuleGeom_GetHalfHeight(_entityId);
            }
        }
    }
}
