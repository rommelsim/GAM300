/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	Components.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines the base Component class
            and various component classes
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/

using System.Security.Cryptography.X509Certificates;

namespace AeonScriptCore
{
    public abstract class Component
    {
        internal Component() {}

        public uint EntityId { get; internal set; }
    }

    public class AudioComponent : Component
    {
        public string SoundFilepath
        {
            get
            {
                return InternalCalls.AudioComponent_GetSoundFile(EntityId);
            }
            set
            {
                InternalCalls.AudioComponent_SetSoundFile(EntityId, value);
            }
        }

        public float Volume
        {
            get
            {
                return InternalCalls.AudioComponent_GetVolume(EntityId);
            }
            set
            {
                InternalCalls.AudioComponent_SetVolume(EntityId, value);
            }
        }

        public float Pitch
        {
            get
            {
                return InternalCalls.AudioComponent_GetPitch(EntityId);
            }
            set
            {
                InternalCalls.AudioComponent_SetPitch(EntityId, value);
            }
        }

        public float Pan
        {
            get
            {
                return InternalCalls.AudioComponent_GetPan(EntityId);
            }
            set
            {
                InternalCalls.AudioComponent_SetPan(EntityId, value);
            }
        }
        
        public Audio.ChannelType ChannelType
        {
            get
            {
                return (Audio.ChannelType)InternalCalls.AudioComponent_GetChannelType(EntityId);
            }
            set
            {
                InternalCalls.AudioComponent_SetChannelType(EntityId, (int)value);
            }
        }

        public void PlaySound()
        {
            InternalCalls.AudioComponent_PlaySound(EntityId);
        }

        public void PauseSound()
        {
            InternalCalls.AudioComponent_PauseSound(EntityId);
        }

        public void ResumeSound()
        {
            InternalCalls.AudioComponent_ResumeSound(EntityId);
        }

        public void StopSound()
        {
            InternalCalls.AudioComponent_StopSound(EntityId);
        }

        public bool IsPaused()
        {
            return InternalCalls.AudioComponent_IsPaused(EntityId);
        }

        public void FadeIn(float duration)
        {
            InternalCalls.AudioComponent_FadeIn(EntityId, duration);
        }

        public void FadeOut(float duration)
        {
            InternalCalls.AudioComponent_FadeOut(EntityId, duration);
        }
    }

    public class Transform : Component
    {
        public Vector3 Position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(EntityId, out Vector3 outPos);
                return outPos;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(EntityId, ref value);
            }
        }
        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(EntityId, out Vector3 outScale);
                return outScale;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(EntityId, ref value);
            }
        }

        /// <summary>
        /// Angle unit is in degrees
        /// </summary>
        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(EntityId, out Vector3 outRotation);
                return outRotation * MathHelper.RadToDeg;
            }
            set
            {
                Vector3 radRot = value * MathHelper.DegToRad;
                InternalCalls.TransformComponent_SetRotation(EntityId, ref radRot);
            }
        }
    }

    public class Camera : Component
    {
        public Vector3 Position
        {
            get
            {
                InternalCalls.CameraComponent_GetPosition(EntityId, out Vector3 outPos);
                return outPos;
            }
            set
            {
                InternalCalls.CameraComponent_SetPosition(EntityId, ref value);
            }
        }

        public Vector3 Target
        {
            get
            {
                InternalCalls.CameraComponent_GetTarget(EntityId, out Vector3 outPos);
                return outPos;
            }
            set
            {
                InternalCalls.CameraComponent_SetTarget(EntityId, ref value);
            }
        }

        // the Yaw rotation is in degrees
        public float Yaw
        {
            get
            {
                return InternalCalls.CameraComponent_GetYaw(EntityId);
            }
            set
            {
                InternalCalls.CameraComponent_SetYaw(EntityId, value);
            }
        }
        
        // the Pitch rotation is in degrees
        public float Pitch
        {
            get
            {
                return InternalCalls.CameraComponent_GetPitch(EntityId);
            }
            set
            {
                InternalCalls.CameraComponent_SetPitch(EntityId, value);
            }
        }

        public float Zoom
        {
            get
            {
                return InternalCalls.CameraComponent_GetZoom(EntityId);
            }
            set
            {
                InternalCalls.CameraComponent_SetZoom(EntityId, value);
            }
        }
    }

    public class RigidBody : Component
    {
        public float Mass
        {
            get
            {
                return InternalCalls.RigidBodyComponent_GetMass(EntityId);
            }
            set
            {
                InternalCalls.RigidBodyComponent_SetMass(EntityId, value);
            }
        }

        public float LinearDrag
        {
            get
            {
                return InternalCalls.RigidBodyComponent_GetLinearDrag(EntityId);
            }
            set
            {
                InternalCalls.RigidBodyComponent_SetLinearDrag(EntityId, value);
            }
        }

        public float AngularDrag
        {
            get
            {
                return InternalCalls.RigidBodyComponent_GetAngularDrag(EntityId);
            }
            set
            {
                InternalCalls.RigidBodyComponent_SetAngularDrag(EntityId, value);
            }
        }

        public bool UseGravity
        {
            get
            {
                return InternalCalls.RigidBodyComponent_GetUseGravity(EntityId);
            }
            set
            {
                InternalCalls.RigidBodyComponent_SetUseGravity(EntityId, value);
            }
        }

        public bool IsAutoCenterOfMass
        {
            get
            {
                return InternalCalls.RigidBodyComponent_GetIsAutoCenterOfMass(EntityId);
            }
            set
            {
                InternalCalls.RigidBodyComponent_SetIsAutoCenterOfMass(EntityId, value);
            }
        }

        public Vector3 CenterOfMass
        {
            get
            {
                InternalCalls.RigidBodyComponent_GetCenterOfMass(EntityId, out Vector3 _centerMass);
                return _centerMass;
            }
            set
            {
                InternalCalls.RigidBodyComponent_SetCenterOfMass(EntityId, ref value);
            }
        }

        public bool IsKinematic
        {
            get
            {
                return InternalCalls.RigidBodyComponent_GetIsKinematic(EntityId);
            }
            set
            {
                InternalCalls.RigidBodyComponent_SetIsKinematic(EntityId, value);
            }
        }

        public Vector3 Velocity
        {
            get
            {
                InternalCalls.RigidBodyComponent_GetVelocity(EntityId, out Vector3 _centerMass);
                return _centerMass;
            }
            set
            {
                InternalCalls.RigidBodyComponent_SetVelocity(EntityId, ref value);
            }
        }

        public void AddForce(ref Vector3 force)
        {
            InternalCalls.RigidBodyComponent_AddForce(EntityId, ref force);
        }

        public void AddImpulseForce(ref Vector3 force)
        {
            InternalCalls.RigidBodyComponent_AddImpulse(EntityId, ref force);
        }

        public bool Raycast(ref Vector3 origin, ref Vector3 direction, float maxDistance, out RaycastResult result)
        {
           return InternalCalls.RigidBodyComponent_Raycast(EntityId, ref origin, ref direction, maxDistance, out result);
        }
    }

    public class Collider : Component
    {
        /// <summary>
        /// The shape type of the collider: cube, sphere, capsule or none
        /// </summary>
        public Physics.ColliderShapeType ShapeType
        {
            get
            {
                return (Physics.ColliderShapeType)InternalCalls.ColliderComponent_GetColliderType(EntityId);
            }
        }

        /// <summary>
        /// Is the collider a trigger box or a collidable box?
        /// </summary>
        public bool IsTrigger
        {
            get
            {
                return InternalCalls.ColliderComponent_IsTrigger(EntityId);
            }
            set
            {
                InternalCalls.ColliderComponent_SetIsTrigger(EntityId, value);
            }
        }

        /// <summary>
        /// The collider's offset from the position of the entity
        /// </summary>
        public Vector3 Offset
        {
            get
            {
                InternalCalls.ColliderComponent_GetOffset(EntityId, out Vector3 offset);
                return offset;
            }
            set
            {
                InternalCalls.ColliderComponent_SetOffset(EntityId, ref value);
            }
        }

        /// <summary>
        /// Get the collider shape. If the component does not have a shape, null is returned
        /// </summary>
        /// <returns></returns>
        public Physics.ColliderShape GetColliderShape()
        {
            switch (ShapeType)
            {
                case Physics.ColliderShapeType.CUBE:
                    return new Physics.CubeColliderShape(EntityId);
                case Physics.ColliderShapeType.SPHERE:
                    return new Physics.SphereColliderShape(EntityId);
                case Physics.ColliderShapeType.CAPSULE:
                    return new Physics.CapsuleColliderShape(EntityId);
            }
            return null;
        }
    }

    public class Render : Component
    {
        /// <summary>
        /// You can pass the path relative to the game, that is your string will begin with "./resources/..."
        /// </summary>
        public string ModelPath
        {
            get
            {
                return InternalCalls.RenderComponent_GetModelPath(EntityId);
            }
            set
            {
                InternalCalls.RenderComponent_SetModelPath(EntityId, value);
            }
        }
        
        /// <summary>
        /// You can pass the path relative to the game, that is your string will begin with "./resources/..."
        /// </summary>
        public string DiffuseTexturePath
        {
            get
            {
                return InternalCalls.RenderComponent_GetDiffuseTexturePath(EntityId);
            }
            set
            {
                InternalCalls.RenderComponent_SetDiffuseTexturePath(EntityId, value);
            }
        }
        
        /// <summary>
        /// You can pass the path relative to the game, that is your string will begin with "./resources/..."
        /// </summary>
        public string SpecularTexturePath
        {
            get
            {
                return InternalCalls.RenderComponent_GetSpecularTexturePath(EntityId);
            }
            set
            {
                InternalCalls.RenderComponent_SetSpecularTexturePath(EntityId, value);
            }
        }

        public float Shininess
        {
            get
            {
                return InternalCalls.RenderComponent_GetShininess(EntityId);
            }
            set
            {
                InternalCalls.RenderComponent_SetShininess(EntityId, value);
            }
        }
    }

    public class Lighting : Component
    {
        public LightType Type
        {
            get
            {
                return (LightType)InternalCalls.LightComponent_GetLightType(EntityId);
            }
            set
            {
                InternalCalls.LightComponent_SetLightType(EntityId, (int)value);
            }
        }

        public Vector3 Color
        {
            get
            {
                InternalCalls.LightComponent_GetColor(EntityId, out Vector3 outColor);
                return outColor;
            }
            set
            {
                InternalCalls.LightComponent_SetColor(EntityId, ref value);
            }
        }

        public Vector3 Direction
        {
            get
            {
                InternalCalls.LightComponent_GetDirection(EntityId, out Vector3 outDirection);
                return outDirection;
            }
            set
            {
                InternalCalls.LightComponent_SetDirection(EntityId, ref value);
            }
        }

        public float Intensity
        {
            get
            {
                return InternalCalls.LightComponent_GetIntensity(EntityId);
            }
            set
            {
                InternalCalls.LightComponent_SetIntensity(EntityId, value);
            }
        }

        public float InnerCutoff
        {
            get
            {
                return InternalCalls.LightComponent_GetInnerCutoff(EntityId);
            }
            set
            {
                InternalCalls.LightComponent_SetInnerCutoff(EntityId, value);
            }
        }

        public float OuterCutoff
        {
            get
            {
                return InternalCalls.LightComponent_GetOuterCutoff(EntityId);
            }
            set
            {
                InternalCalls.LightComponent_SetOuterCutoff(EntityId, value);
            }
        }
    }

    public class Particles : Component
    {
        string DiffuseTexture
        {
            get
            {
                return InternalCalls.ParticlesComponent_GetDiffuseTexture(EntityId);
            }
            set
            {
                InternalCalls.ParticlesComponent_SetDiffuseTexture(EntityId, value);
            }
        }

        string SpecularTexture
        {
            get
            {
                return InternalCalls.ParticlesComponent_GetSpecularTexture(EntityId);
            }
            set
            {
                InternalCalls.ParticlesComponent_SetSpecularTexture(EntityId, value);
            }
        }
        
        uint MaximumParticles
        {
            get
            {
                return InternalCalls.ParticlesComponent_GetMaximumParticles(EntityId);
            }
            set
            {
                InternalCalls.ParticlesComponent_SetMaximumParticles(EntityId, value);
            }
        }
        
        /// <summary>
        /// The number of particles to spawn per frame
        /// </summary>
        uint SpawnParticleRate
        {
            get
            {
                return InternalCalls.ParticlesComponent_GetSpawnParticleRate(EntityId);
            }
            set
            {
                InternalCalls.ParticlesComponent_SetSpawnParticleRate(EntityId, value);
            }
        }
        
        /// <summary>
        /// The initial lifetime of the particles, time unit is in seconds
        /// </summary>
        float ParticleLife
        {
            get
            {
                return InternalCalls.ParticlesComponent_GetParticleLife(EntityId);
            }
            set
            {
                InternalCalls.ParticlesComponent_SetParticleLife(EntityId, value);
            }
        }
        
        Vector3 ParticleGravity
        {
            get
            {
                InternalCalls.ParticlesComponent_GetParticleGravity(EntityId, out Vector3 gravityVector);
                return gravityVector;
            }
            set
            {
                InternalCalls.ParticlesComponent_SetParticleGravity(EntityId, ref value);
            }
        }
        
        Vector3 ParticleOffset
        {
            get
            {
                InternalCalls.ParticlesComponent_GetParticleOffset(EntityId, out Vector3 offsetVector);
                return offsetVector;
            }
            set
            {
                InternalCalls.ParticlesComponent_SetParticleOffset(EntityId, ref value);
            }
        }
        
        Vector3 ParticleMinVelocity
        {
            get
            {
                InternalCalls.ParticlesComponent_GetParticleMinVelocity(EntityId, out Vector3 minVelocityVector);
                return minVelocityVector;
            }
            set
            {
                InternalCalls.ParticlesComponent_SetParticleMinVelocity(EntityId, ref value);
            }
        }
        
        Vector3 ParticleMaxVelocity
        {
            get
            {
                InternalCalls.ParticlesComponent_GetParticleMaxVelocity(EntityId, out Vector3 maxVelocityVector);
                return maxVelocityVector;
            }
            set
            {
                InternalCalls.ParticlesComponent_SetParticleMaxVelocity(EntityId, ref value);
            }
        }

        string ModelPath
        {
            get
            {
                return InternalCalls.ParticlesComponent_GetModelPath(EntityId);
            }
            set
            {
                InternalCalls.ParticlesComponent_SetModelPath(EntityId, value);
            }
        }
    }

    public class UI : Component
    {
        public FormatType Format
        {
            get
            {
                return (FormatType)InternalCalls.UIComponent_GetFormat(EntityId);
            }
            set
            {
                InternalCalls.UIComponent_SetFormat(EntityId, (int)value);
            }
        }

        public LayerType Layer
        {
            get
            {
                return (LayerType)InternalCalls.UIComponent_GetLayer(EntityId);
            }
            set
            {
                InternalCalls.UIComponent_SetLayer(EntityId, (int)value);
            }
        }

        /// <summary>
        /// Order starts from smallest to biggest, that is the UI with smallest order is closest to the screen. On the other hand, the bigger the order the further it is from the screen, it would be rendered behind other smaller order UI
        /// </summary>
        public uint Order
        {
            get
            {
                return InternalCalls.UIComponent_GetOrder(EntityId);
            }
            set
            {
                InternalCalls.UIComponent_SetOrder(EntityId, value);
            }
        }

        /// <summary>
        /// You can pass the path relative to the game, that is your string will begin with "./resources/..."
        /// </summary>
        public string Texture
        {
            get
            {
                return InternalCalls.UIComponent_GetTexture(EntityId);
            }
            set
            {
                InternalCalls.UIComponent_SetTexture(EntityId, value);
            }
        }

        public float Opacity
        {
            get
            {
                return InternalCalls.UIComponent_GetOpacity(EntityId);
            }
            set
            {
                InternalCalls.UIComponent_SetOpacity(EntityId, value);
            }
        }

        public string Text
        {
            get
            {
                return InternalCalls.UIComponent_GetText(EntityId);
            }
            set
            {
                InternalCalls.UIComponent_SetText(EntityId, value);
            }
        }

        /// <summary>
        /// Red, Green, Blue in range of 0.0f to 1.0f. You can divide integer rgb code by 255 to obtain the decimal value.
        /// </summary>
        public Vector3 TextColor
        {
            get
            {
                InternalCalls.UIComponent_GetTextColor(EntityId, out Vector3 outColor);
                return outColor;
            }
            set
            {
                InternalCalls.UIComponent_SetTextColor(EntityId, ref value);
            }
        }
    }
}
