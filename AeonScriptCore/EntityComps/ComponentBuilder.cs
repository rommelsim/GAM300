/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	ComponentBuilder.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines the IComponentBuilder interface
            and the derived builders for all available components
            in Components.cs
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/
using AeonScriptCore.Physics;
using System;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;

namespace AeonScriptCore
{
    internal class ComponentBitAttribute : Attribute
    {
        internal int bit;

        internal ComponentBitAttribute(int _bit)
        {
            this.bit = _bit;
        }
    }

    /// <summary>
    /// Abstract class for all component builders
    /// </summary>
    public interface IComponentBuilder {}

    /// <summary>
    /// Create a builder containing information for the Transform component. Used in EntityBuilder.
    /// </summary>
    [ComponentBit(0)]
    public class TransformBuilder : IComponentBuilder
    {
        internal Vector3 _position, _scale, _rotation;

        /// <summary>
        /// Create a TransformBuilder from an existing entity's Transform component
        /// </summary>
        /// <param name="transComp">A</param>
        public TransformBuilder(Transform transComp)
        {
            _position = transComp.Position;
            _scale = transComp.Scale;
            _rotation = transComp.Rotation;
        }

        /// <summary>
        /// Sets the position and rotation to 0 and scale to 1
        /// </summary>
        public TransformBuilder()
        {
            _position = _rotation = Vector3.ZERO;
            _scale = Vector3.ONE;
        }

        /// <summary>
        /// Set the position
        /// </summary>
        /// <param name="_position"></param>
        /// <returns></returns>
        public TransformBuilder SetPosition(Vector3 _position)
        {
            this._position = _position;
            return this;
        }

        /// <summary>
        /// Set the scaling
        /// </summary>
        /// <param name="_scale"></param>
        /// <returns></returns>
        public TransformBuilder SetScale(Vector3 _scale)
        {
            this._scale = _scale;
            return this;
        }

        /// <summary>
        /// Set the rotation
        /// </summary>
        /// <param name="_rotation">The angle values are in radians</param>
        /// <returns></returns>
        public TransformBuilder SetRotation(Vector3 _rotation)
        {
            this._rotation = _rotation;
            return this;
        }
    }

    /// <summary>
    /// Create a builder containing information for the RigidBody component. Used in EntityBuilder.<br/>
    /// By default, the RigidBodyType is set to Static, the Mass to 1, Linear & Angular drag to 0.5, UseGravity & IsKinematic to false
    /// </summary>
    [ComponentBit(1)]
    public class RigidBodyBuilder : IComponentBuilder
    {
        internal Physics.RigidBodyType Type;

        internal float _mass, _linearDrag, _angularDrag;
        internal bool _useGravity, _isKinematic;

        /// <summary>
        /// Sets the type to static, mass to 1, linear & angular drag to 0.5, UseGravity and IsKinematic to false
        /// </summary>
        public RigidBodyBuilder()
        {
            Type = Physics.RigidBodyType.STATIC;
            _mass = 1.0f;
            _linearDrag = _angularDrag = 0.5f;
            _useGravity = _isKinematic = false;
        }

        public RigidBodyBuilder SetType(Physics.RigidBodyType type)
        {
            this.Type = type;
            return this;
        }

        public RigidBodyBuilder SetMass(float mass)
        {
            this._mass = mass;
            return this;
        }

        public RigidBodyBuilder SetLinearDrag(float linearDrag)
        {
            this._linearDrag = linearDrag;
            return this;
        }

        public RigidBodyBuilder SetAngularDrag(float angularDrag)
        {
            this._angularDrag = angularDrag;
            return this;
        }

        public RigidBodyBuilder SetUseGravity(bool useGravity)
        {
            this._useGravity = useGravity;
            return this;
        }

        public RigidBodyBuilder SetIsKinematic(bool isKinematic)
        {
            this._isKinematic = isKinematic;
            return this;
        }
    }

    [ComponentBit(2)]
    public class ColliderBuilder : IComponentBuilder
    {
        internal bool IsTrigger = false;
        internal Vector3 Offset = Vector3.ZERO;

        internal Physics.ColliderShapeType ShapeType;
        internal ColliderShapeBuilder Shape;

        /// <summary>
        /// Sets is trigger to false, offset to Zero'ed values and collider shape to cube with half size of 0.5
        /// </summary>
        public ColliderBuilder()
        {
            SetShape(new CubeShapeBuilder());
        }

        /// <summary>
        /// Sets the is trigger property of the collider. If boolean is true, it means it is a trigger and no collision will occur. Otherwise, it means a collision will occur.
        /// </summary>
        /// <param name="isTrigger"></param>
        /// <returns></returns>
        public ColliderBuilder SetIsTrigger(bool isTrigger)
        {
            IsTrigger = isTrigger;
            return this;
        }

        /// <summary>
        /// Sets the offset of the collider shape relative to the entity's position
        /// </summary>
        /// <param name="offset"></param>
        /// <returns></returns>
        public ColliderBuilder SetOffet(Vector3 offset)
        {
            Offset = offset;
            return this;
        }

        /// <summary>
        /// Set the shape of the collider using one of three available shape builders: CubeColliderShapeBuilder, SphereColliderShapeBuilder, CapsuleColliderShapeBuilder
        /// </summary>
        /// <param name="shapeBuilder">  </param>
        /// <returns></returns>
        public ColliderBuilder SetShape(ColliderShapeBuilder shapeBuilder)
        {
            ShapeType = shapeBuilder.GetShapeType();
            Shape = shapeBuilder;

            return this;
        }

        /// <summary>
        /// Base class for all collider shape builders to inherit from
        /// </summary>
        public interface ColliderShapeBuilder {
            /// <summary>
            /// Quick way to get the shape type of the builder
            /// </summary>
            /// <returns>An enum value from ColliderShapeType</returns>
            ColliderShapeType GetShapeType();
        }

        public class CubeShapeBuilder : ColliderShapeBuilder
        {
            internal Vector3 HalfSize;

            /// <summary>
            /// Default constructor, sets the half size to 0.5
            /// </summary>
            public CubeShapeBuilder()
            {
                HalfSize = new Vector3(0.5f);
            }

            /// <summary>
            /// Set the size of the cube collision box, values are of half-size.<br/>
            /// So if your desired box size is a cube of size 1.0, the half size value is 0.5
            /// </summary>
            public CubeShapeBuilder SetHalfSize(Vector3 halfSize)
            {
                HalfSize = halfSize;
                return this;
            }

            public ColliderShapeType GetShapeType()
            {
                return ColliderShapeType.CUBE;
            }
        }

        public class SphereShapeBuilder : ColliderShapeBuilder
        {
            internal float Radius;

            /// <summary>
            /// Default constructor, sets the Radius to 1.0
            /// </summary>
            public SphereShapeBuilder()
            {
                Radius = 1.0f;
            }

            public SphereShapeBuilder SetRadius(float radius)
            {
                Radius = radius;
                return this;
            }

            public ColliderShapeType GetShapeType()
            {
                return ColliderShapeType.SPHERE;
            }
        }

        /*public class CapsuleShapeBuilder : ColliderShapeBuilder
        {
            internal float Radius, HalfHeight;

            public CapsuleShapeBuilder()
            {
                Radius = HalfHeight = 1.0f;
            }

            public CapsuleShapeBuilder SetRadius(float radius)
            {
                Radius = radius;
                return this;
            }

            public CapsuleShapeBuilder SetHalfHeight(float halfHeight)
            {
                HalfHeight = halfHeight;
                return this;
            }

            public ColliderShapeType GetShapeType()
            {
                return ColliderShapeType.CAPSULE;
            }
        }*/
    }

    /// <summary>
    /// Create a builder containing information for the Render component. Used in EntityBuilder.<br/>
    /// By default, the Shininess is set to 0.5, Specular Path to empty string
    /// </summary>
    [ComponentBit(3)]
    public class RenderBuilder : IComponentBuilder
    {
        internal string ModelPath, DiffusePath, SpecularPath = "";
        internal float Shininess = 0.5f;

        /// <summary>
        /// Construct the RenderBuilder by providing the model and diffuse texture filepaths
        /// </summary>
        /// <param name="modelPath">Path to a model file, it is relative to the game directory, that is you must provide the path that begins with "./resources/..."</param>
        /// <param name="diffusePath">Path to a diffuse texture, it is relative to the game directory, that is you must provide the path that begins with "./resources/..."</param>
        public RenderBuilder(string modelPath, string diffusePath)
        {
            ModelPath = modelPath;
            DiffusePath = diffusePath;
        }

        public RenderBuilder SetShininess(float shininess) {
            Shininess = shininess;
            return this;
        }

        /// <summary>
        /// Sets the specular of the model
        /// </summary>
        /// <param name="specularPath">Path to a specular texture, it is relative to the game directory, that is you must provide the path that begins with "./resources/..."</param>
        /// <returns></returns>
        public RenderBuilder SetSpecularPath(string specularPath)
        {
            SpecularPath = specularPath;
            return this;
        }
    }

    /// <summary>
    /// Create a builder containing information for the UI component. Used in EntityBuilder.<br/>
    /// By default, the FormatType is Word, Layer is Default, Order is 0, Texture and Text strings are empty, Opacity is 1.0, and TextColor is White.
    /// </summary>
    [ComponentBit(4)]
    public class UIBuilder : IComponentBuilder
    {
        internal FormatType Format = FormatType.Word;
        internal LayerType Layer = LayerType.Default;
        internal uint Order = 0;
        internal string Texture = "";
        internal float Opacity = 1.0f;
        internal string Text = "";
        internal Vector3 TextColor = Vector3.ONE;

        public UIBuilder() {}

        /// <summary>
        /// Set the type of UI to render, an image or text
        /// </summary>
        /// <param name="format"></param>
        /// <returns></returns>
        public UIBuilder SetFormat(FormatType format)
        {
            Format = format;
            return this;
        }

        /// <summary>
        /// Sets the layer that the UI should render in.
        /// </summary>
        /// <param name="layer"></param>
        /// <returns></returns>
        public UIBuilder SetLayer(LayerType layer)
        {
            Layer = layer;
            return this;
        }

        /// <summary>
        /// Set the order of the UI component
        /// </summary>
        /// <param name="order">The smaller the number, the closer it is to the screen. The bigger the number, the further it is, that is it will be behind other UI that has a smaller order number</param>
        /// <returns></returns>
        public UIBuilder SetOrder(uint order)
        {
            Order = order;
            return this;
        }

        /// <summary>
        /// Set the image being rendered. This will take effect only if the Format is set to Image.
        /// </summary>
        /// <param name="texture">Path to an image, it is relative to the game directory, that is you must provide the path that begins with "./resources/..."</param>
        /// <returns></returns>
        public UIBuilder SetTexture(string texture)
        {
            Texture = texture;
            return this;
        }

        /// <summary>
        /// Sets the opacity of the UI component, regardless of Format. 
        /// </summary>
        /// <param name="opacity"></param>
        /// <returns></returns>
        public UIBuilder SetOpacity(float opacity)
        {
            Opacity = opacity;
            return this;
        }

        /// <summary>
        /// Set the text that will be displayed. This will take effect only if the Format is set to Word.
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        public UIBuilder SetText(string text)
        {
            Text = text;
            return this;
        }

        /// <summary>
        /// Set the color of the text being rendered. This will take effect only if the Format is set to Word.
        /// </summary>
        /// <param name="color">Color of the text, in rgb values, ranges from 0.0f to 1.0f</param>
        /// <returns></returns>
        public UIBuilder SetTextColor(Vector3 color)
        {
            TextColor = color;
            return this;
        }
    }

    /// <summary>
    /// Create a builder containing information for the Behaviour component. Used in EntityBuilder.<br/>
    /// </summary>
    [ComponentBit(5)]
    public class BehaviourBuilder : IComponentBuilder
    {
        internal List<string> ScriptNameList;

        public BehaviourBuilder()
        {
            ScriptNameList = new List<string>();
        }

        /// <summary>
        /// Adds the full name of a AeonBehaviour class into a List of strings
        /// </summary>
        /// <typeparam name="T">A type that extends AeonBehaviour, the desired script you wish to add to the Behaviour component.</typeparam>
        /// <returns></returns>
        public BehaviourBuilder AddScript<T>() where T : AeonBehaviour
        {
            ScriptNameList.Add(typeof(T).FullName);
            return this;
        }
    }

    /// <summary>
    /// Create a builder containing information for the Audio component. Used in EntityBuilder.<br/>
    /// By default, the Volume, Pitch and Pan are 1.0
    /// </summary>
    [ComponentBit(6)]
    public class AudioBuilder : IComponentBuilder
    {
        internal Audio.ChannelType ChannelType;
        internal string SoundFilepath;
        internal float Volume, Pitch, Pan;

        /// <summary>
        /// Construct the AudioBuilder by providing the channel type and sound's filepath, note that the audio will not be play automatically, you need to do it in the Entity.Create function call
        /// </summary>
        /// <param name="channelType">The type of audio, bgm or sfx</param>
        /// <param name="soundFilepath">Need to begin your filepath with ".\\resources\\audios\\..."</param>
        /// <param name="volume">Optional parameter to set the volume</param>
        public AudioBuilder(Audio.ChannelType channelType, string soundFilepath, float volume = 1.0f)
        {
            ChannelType = channelType;
            SoundFilepath = soundFilepath;
            Volume = 1.0f;
            Pan = 1.0f;
            Pitch = 1.0f;
        }

        /// <summary>
        /// Sets the panning property of the audio
        /// </summary>
        /// <param name="pan">Range from 0.0 to 1.0</param>
        /// <returns></returns>
        public AudioBuilder SetPan(float pan)
        {
            Pan = pan;
            return this;
        }

        /// <summary>
        /// Sets the pitch property of the audio
        /// </summary>
        /// <param name="pitch">Range from 0.0 to 1.0</param>
        /// <returns></returns>
        public AudioBuilder SetPitch(float pitch)
        {
            Pitch = pitch;
            return this;
        }
    }

    /// <summary>
    /// Create a builder containing information for the Particles component. Used in EntityBuilder.<br/>
    /// By default, the Max Particles is 200, Particle Spawn Rate is 2, Particle Life is 1
    /// </summary>
    [ComponentBit(7)]
    public class ParticlesBuilder : IComponentBuilder
    {
        internal string DiffuseTexture = "", SpecularTexture = "", ModelPath = "";
        internal uint MaxParticles, SpawnRate;
        internal float ParticleLife = 1.0f;
        internal Vector3 Gravity, Offset, MinVelocity, MaxVelocity;

        /// <summary>
        /// Constructor sets the max particles and spawn rate
        /// </summary>
        /// <param name="maxParticles">Max number of particles that can spawn at any one time</param>
        /// <param name="particleSpawnRate">The number of particles that spawn per frame</param>
        public ParticlesBuilder(uint maxParticles = 200, uint particleSpawnRate = 2)
        {
            MaxParticles = maxParticles;
            SpawnRate = particleSpawnRate;
        }

        public ParticlesBuilder SetDiffuseTexture(string diffuseTexture)
        {
            DiffuseTexture = diffuseTexture;
            return this;
        }

        public ParticlesBuilder SetSpecularTexture(string specularTexture)
        {
            SpecularTexture = specularTexture;
            return this;
        }

        public ParticlesBuilder SetModelPath(string modelPath)
        {
            ModelPath = modelPath;
            return this;
        }

        /// <summary>
        /// Set the particle life. How long can the particles survive?
        /// </summary>
        /// <param name="particleLife">Life duration of the particles, time unit in seconds</param>
        /// <returns></returns>
        public ParticlesBuilder SetParticleLife(float particleLife)
        {
            ParticleLife = particleLife;
            return this;
        }

        public ParticlesBuilder SetGravity(Vector3 gravity)
        {
            Gravity = gravity;
            return this;
        }

        public ParticlesBuilder SetOffset(Vector3 offset)
        {
            Offset = offset;
            return this;
        }

        public ParticlesBuilder SetMinVelocity(Vector3 minVelocity)
        {
            MinVelocity = minVelocity;
            return this;
        }

        public ParticlesBuilder SetMaxVelocity(Vector3 maxVelocity)
        {
            MaxVelocity = maxVelocity;
            return this;
        }
    }
}
