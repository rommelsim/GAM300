/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	Entity.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines the Entity class
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Reflection;

namespace AeonScriptCore
{
    using SceneID = System.Byte; // 8 bits unsigned integer
    using EntityID = System.UInt32;

    public sealed class Entity /*: IEquatable<Entity>*/
    {
        // list of living entities, created from FindEntityByName or GetEntity
        internal static Dictionary<SceneID, Dictionary<EntityID, Entity>> _entities = new Dictionary<SceneID, Dictionary<EntityID, Entity>>();

        public readonly EntityID Id;

        public string Name
        {
            get
            {
                return InternalCalls.Entity_GetName(this.Id);
            }
            set
            {
                InternalCalls.Entity_SetName(this.Id, value);
            }
        }

        public Entity Parent {
            get
            {
                EntityID parentId = InternalCalls.Entity_GetParent(Id, out bool DoesParentExist);

                return DoesParentExist ? GetEntity(parentId) : null;
            }
            set
            {
                InternalCalls.Entity_SetParent(value.Id, Id);
            }
        }

        private List <Component> components = new List<Component>();

        protected Entity() {
            Id = 0;
        }

        internal Entity(EntityID id)
        {
            Id = id;
        }

        /// <summary>
        /// Creates an entity
        /// </summary>
        /// <param name="entityBuilder">Instance of an entity builder</param>
        /// <param name="onCreateEntityAction"></param>
        public static void Create(EntityBuilder entityBuilder, Action<Entity> onCreateEntityAction = null)
        {
            entityBuilder.createAction = onCreateEntityAction;
            EntityBuilder.entityCreateRequests.Add(entityBuilder);
        }

        /// <summary>
        /// Destroy the desired entity if you have the Entity instance
        /// </summary>
        /// <param name="entity">The instance of an entity</param>
        public static void Destroy(Entity entity)
        {
            if (entity != null)
                Destroy(entity.Id);
        }

        /// <summary>
        /// Destroy the desired entity using its id
        /// </summary>
        /// <param name="entityId">The id of an entity</param>
        public static void Destroy(EntityID entityId)
        {
            InternalCalls.Entity_DestroyEntity(entityId);
        }

        /// <summary>
        /// Check if an entity is alive, more reliable than the null check. Entity variant
        /// </summary>
        /// <param name="entity">The Entity to check for</param>
        /// <returns>A boolean is returned. True if the entity is alive, false otherwise.</returns>
        public static bool IsAlive(Entity entity)
        {
            if (entity == null) return false;
            return IsAlive(entity.Id);
        }

        /// <summary>
        /// Check if an entity is alive, more reliable than the null check. EntityID variant
        /// </summary>
        /// <param name="entityId">Id of an Entity to check for</param>
        /// <returns>A boolean is returned. True if the entity is alive, false otherwise.</returns>
        public static bool IsAlive(EntityID entityId)
        {
            return InternalCalls.Entity_EntityExists(entityId);
        }

        /// <summary>
        /// Get an Entity using their id
        /// </summary>
        /// <param name="entityId">An entity ID</param>
        /// <returns>An instance to the Entity</returns>
        public static Entity GetEntity(EntityID entityId)
        {
            if (InternalCalls.Entity_EntityExists(entityId))
            {
                SceneID sceneId = InternalCalls.Entity_GetSceneFromEntityID(entityId);

                // try to get Entity Dictionary using a SceneID
                if (_entities.TryGetValue(sceneId, out var entities))
                {
                    if (entities.TryGetValue(entityId, out Entity entity))
                    {
                        return entity;
                    }
                    else
                    {
                        Entity ent = new Entity(entityId);
                        entities.Add(entityId, ent);
                        return ent;
                    }
                }
                else // failed to get, so need to add
                {
                    Entity ent = new Entity(entityId);
                    var newEnts = new Dictionary<EntityID, Entity>
                    {
                        { entityId, ent }
                    };
                    _entities.Add(sceneId, newEnts);
                    return ent;
                }
            }
            return null;
        }

        /// <summary>
        /// Find an Entity using their name
        /// </summary>
        /// <param name="name">An entity's name</param>
        /// <returns>An instance to the Entity</returns>
        public static Entity FindEntityByName(string name)
        {
            EntityID entityID = InternalCalls.Entity_GetEntityByName(name, out bool _doesEntExist);

            if (_doesEntExist)
            {
                //if (_entities.TryGetValue(entityID, out Entity entity))
                //{
                //    return entity;
                //}
                //else
                //{
                //    Entity ent = new Entity(entityID);
                //    _entities.Add(entityID, ent);
                //    return ent;
                //}
                SceneID sceneId = InternalCalls.Entity_GetSceneFromEntityID(entityID);
                if (_entities.TryGetValue(sceneId, out var entities))
                {
                    if (entities.TryGetValue(entityID, out Entity entity))
                    {
                        return entity;
                    }
                    else
                    {
                        Entity ent = new Entity(entityID);
                        entities.Add(entityID, ent);
                        return ent;
                    }
                }
                else // failed to get, so need to add
                {
                    Entity ent = new Entity(entityID);
                    var newEnts = new Dictionary<EntityID, Entity>
                    {
                        { entityID, ent }
                    };
                    _entities.Add(sceneId, newEnts);
                    return ent;
                }
            }
            return null;
        }

        /// <summary>
        /// Returns a new instance of T component
        /// </summary>
        /// <typeparam name="T">A class type that inherits from Component</typeparam>
        /// <returns>Instance of T, otherwise null if entity doesn't have the component</returns>
        public T GetComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
            {
                foreach (var comp in components)
                {
                    if (comp is T)
                    {
                        return (T)comp;
                    }
                }

                T component = new T() { EntityId = this.Id };
                components.Add(component);

                return component;
            }
            return null;
        }

        /// <summary>
        /// Does this Entity have a particular component
        /// </summary>
        /// <typeparam name="T">A type that must extend Component</typeparam>
        /// <returns>A boolean</returns>
        public bool HasComponent<T>() where T : Component
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(Id, componentType);
        }

        /// <summary>
        /// Get the desired script that this entity owns. If the entity does not have the desired script, null is returned.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns>Desired script instance. If the entity does not have the desired script, <b>null</b> is returned</returns>
        public T GetScript<T>() where T : AeonBehaviour, new()
        {
            object scriptInst = InternalCalls.Behaviour_GetScriptInstance(Id, typeof(T).FullName);
            if (scriptInst != null)
                return scriptInst as T;
            return null;
        }

        /// <summary>
        /// Get the immediate children of this entity
        /// </summary>
        /// <returns>A List of Entity, can be null if there is no child</returns>
        public List<Entity> GetChildren()
        {
            EntityID[] childArr = InternalCalls.Entity_GetChildren(this.Id);

            if (childArr.Length == 0) return null;

            List<Entity> childrenList = new List<Entity>();

            foreach (EntityID childId in childArr)
                childrenList.Add(GetEntity(childId));

            return childrenList;
        }

        //public bool Equals(Entity entity)
        //{
        //    if (entity == null) return false;
        //    return Id.Equals(entity.Id);
        //}

        //public override bool Equals(object obj)
        //{
        //    if (obj == null) return false;
        //    return base.Equals(obj);
        //}
    }

    /// <summary>
    /// Create an Entity using the builder
    /// </summary>
    public class EntityBuilder
    {
        internal static List<EntityBuilder> entityCreateRequests = new List<EntityBuilder>();

        private bool hasParent = false;
        private EntityID parentId;
        private string name;

        private int entitySignature = 0;
        private List<IComponentBuilder> components = new List<IComponentBuilder>();
        private bool hasTransform = false;

        internal Action<Entity> createAction = null;

        public EntityBuilder(string name)
        {
            this.name = name;
        }

        public EntityBuilder SetParent(EntityID entityID)
        {
            parentId = entityID;
            hasParent = true;
            return this;
        }

        /// <summary>
        /// Add a component to the EntityBuilder.<br/><b>Note:</b> If the entity does not have a TransformBuilder, one will be created using the default constructor.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="component"></param>
        /// <returns></returns>
        public EntityBuilder AddComponent<T>(T component) where T : IComponentBuilder
        {
            TypeInfo compTypeInfo = typeof(T).GetTypeInfo();
            int compSignature = 1 << compTypeInfo.GetCustomAttribute<ComponentBitAttribute>().bit;

            if ((entitySignature & compSignature) != compSignature)
            {
                components.Add(component);

                if (component is TransformBuilder)
                    hasTransform = true;
            }
            return this;
        }

        internal void Create()
        {
            EntityID id = InternalCalls.Entity_CreateEntity(name);

            if (hasParent)
                InternalCalls.Entity_SetParent(id, parentId);

            // Add a transform component by default, if no transform was added
            if (!hasTransform)
            {
                BuilderAddComponent(id, new TransformBuilder());
            }

            foreach (IComponentBuilder comp in components)
            {
                BuilderAddComponent(id, comp);
            }

            createAction?.Invoke(Entity.GetEntity(id));
        }

        internal static void BuilderAddComponent(EntityID entityId, IComponentBuilder builder)
        {
            if (builder is TransformBuilder)
            {
                TransformBuilder transform = (TransformBuilder)builder;
                InternalCalls.Builder_AddTransformComponent(entityId, ref transform._position, ref transform._scale, ref transform._rotation);
            }
            if (builder is RigidBodyBuilder)
            {
                RigidBodyBuilder rigidBody = (RigidBodyBuilder)builder;
                InternalCalls.Builder_AddRigidBodyComponent(entityId, (int)rigidBody.Type, rigidBody._mass, rigidBody._linearDrag, rigidBody._angularDrag, rigidBody._useGravity, rigidBody._isKinematic);
            }
            if (builder is ColliderBuilder)
            {
                ColliderBuilder collider = (ColliderBuilder)builder;
                IntPtr compPtr = InternalCalls.Builder_AddColliderComponent(entityId, ref collider.Offset, collider.IsTrigger);

                if (compPtr != IntPtr.Zero)
                {
                    switch (collider.ShapeType)
                    {
                        case Physics.ColliderShapeType.CUBE:
                            {
                                ColliderBuilder.CubeShapeBuilder cubeBuilder = (ColliderBuilder.CubeShapeBuilder)collider.Shape;
                                InternalCalls.Builder_ColliderComponent_SetShapeCube(compPtr, ref cubeBuilder.HalfSize);
                            }
                            break;
                        case Physics.ColliderShapeType.SPHERE:
                            {
                                ColliderBuilder.SphereShapeBuilder sphereBuilder = (ColliderBuilder.SphereShapeBuilder)collider.Shape;
                                InternalCalls.Builder_ColliderComponent_SetShapeSphere(compPtr, sphereBuilder.Radius);
                            }
                            break;
                        //case Physics.ColliderShapeType.CAPSULE:
                        //    {
                        //        ColliderBuilder.CapsuleShapeBuilder capsuleBuilder = (ColliderBuilder.CapsuleShapeBuilder)collider.Shape;
                        //        InternalCalls.Builder_ColliderComponent_SetShapeCapsule(compPtr, capsuleBuilder.Radius, capsuleBuilder.HalfHeight);
                        //    }
                        //    break;
                    }
                }

                NativeLog.Log("Does collider pointer exist? {0}", compPtr != IntPtr.Zero);
            }
            if (builder is RenderBuilder)
            {
                RenderBuilder render = (RenderBuilder)builder;
                InternalCalls.Builder_AddRenderComponent(entityId, render.ModelPath, render.DiffusePath, render.SpecularPath, render.Shininess);
            }
            if (builder is UIBuilder)
            {
                UIBuilder ui = (UIBuilder)builder;
                InternalCalls.Builder_AddUIComponent(entityId, (int)ui.Format, (int)ui.Layer, ui.Order, ui.Texture, ui.Opacity, ui.Text, ref ui.TextColor);
            }
            if (builder is BehaviourBuilder)
            {
                BehaviourBuilder behaviour = (BehaviourBuilder)builder;
                InternalCalls.Builder_AddBehaviourComponent(entityId, behaviour.ScriptNameList.ToArray());
            }
            if (builder is AudioBuilder)
            {
                AudioBuilder audio = (AudioBuilder)builder;
                InternalCalls.Builder_AddAudioComponent(entityId, (int)audio.ChannelType, audio.SoundFilepath, audio.Volume, audio.Pitch, audio.Pan);
            }
            if (builder is ParticlesBuilder)
            {
                ParticlesBuilder particles = (ParticlesBuilder)builder;
                InternalCalls.Builder_AddParticlesComponent(entityId, particles.DiffuseTexture, particles.SpecularTexture, particles.MaxParticles, particles.SpawnRate, particles.ParticleLife, ref particles.Gravity, ref particles.Offset, ref particles.MinVelocity, ref particles.MaxVelocity, particles.ModelPath);
            }
        }
    }
}
