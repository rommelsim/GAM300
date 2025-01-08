/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	AeonBehaviour.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines AeonBehaviour, the base
            Behaviour script from which all scripts will
            inherit from.
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/

namespace AeonScriptCore
{
    public abstract class AeonBehaviour
    {
        public readonly Entity entity;

        // Discouraged to create parameter-less constructor, instead use OnCreate, to initialize all variables

        /// <summary>
        /// Recommend to use OnCreate to initialize variables. Most importantly, retrieving components MUST be done in OnCreate
        /// </summary>
        protected AeonBehaviour()
        {
            entity = null;
            //Console.WriteLine("AeonBehaviour empty constructor invoked!");
        }

        internal AeonBehaviour(uint id)
        {
            entity = Entity.GetEntity(id);
            //Console.WriteLine("AeonBehaviour constructor of entityID invoked!");
        }

        /// <summary>
        /// Called only once when this script is instantiated. Instantiate occurs either on Play in editor mode or if the script was added during gameplay
        /// </summary>
        public virtual void OnCreate() {}

        /// <summary>
        /// Called only when a cached scene is made the active scene. It's not called when you load a scene for the first time
        /// </summary>
        public virtual void OnCachedSceneActive() {}

        /// <summary>
        /// Called only when a cached scene is no longer the active scene, that is when another scene replaces it as the active scene, making the current cached scene go into the background
        /// </summary>
        public virtual void OnCachedSceneInactive() {}

        /// <summary>
        /// Runs after the Collision and Trigger events
        /// </summary>
        public virtual void Update() {}

        /// <summary>
        /// Runs on the first frame that a collision occurred
        /// </summary>
        /// <param name="otherEntityID">The entity ID of the other collider</param>
        public virtual void OnCollisionEnter(uint otherEntityID) {}
        
        /// <summary>
        /// Runs while collision happens
        /// </summary>
        /// <param name="otherEntityID">The entity ID of the other collider</param>
        public virtual void OnCollisionUpdate(uint otherEntityID) {}
        
        /// <summary>
        /// Runs on the frame that a collision ended
        /// </summary>
        /// <param name="otherEntityID">The entity ID of the other collider</param>
        public virtual void OnCollisionExit(uint otherEntityID) {}
        
        /// <summary>
        /// Runs on the first frame that this entity's collider entered a trigger
        /// </summary>
        /// <param name="triggerEntityID">The entity ID of the trigger</param>
        public virtual void OnTriggerEnter(uint triggerEntityID) {}
        
        /// <summary>
        /// Runs while this entity's collider is inside a trigger
        /// </summary>
        /// <param name="triggerEntityID">The entity ID of the trigger</param>
        //public virtual void OnTriggerUpdate(uint triggerEntityID) {}
        // TODO: OnTriggerUpdate doesn't seem to be detected by PhysX
        
        /// <summary>
        /// Runs on the frame that this entity's collider exited a trigger
        /// </summary>
        /// <param name="triggerEntityID">The entity ID of the trigger</param>
        public virtual void OnTriggerExit(uint triggerEntityID) {}
    }
}
