/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	EngineCallable.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines collection variables and functions
            for events.
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/
using System.Collections.Generic;
using System;

namespace AeonScriptCore
{
    /// <summary>
    /// Centralized class to create events and subscribe to them
    /// </summary>
    public class EventHandler
    {
        internal static List<Action> FrameEndEvent = new List<Action>();
        internal static List<Action> ScriptPostUpdateEvent = new List<Action>();

        /// <summary>
        /// Add an Action to run at the end of a frame
        /// </summary>
        /// <param name="action">Parameter of type Action, an anonymous function</param>
        public static void AddHandlerToFrameEndEvent(Action action)
        {
            FrameEndEvent.Add(action);
        }

        /// <summary>
        /// Add an Action that runs after all scripts' <b>Update()</b> were called
        /// <param name="action">Parameter of type Action, an anonymous function</param>
        /// </summary>
        public static void AddHandlerToScriptPostUpdateEvent(Action action)
        {
            ScriptPostUpdateEvent.Add(action);
        }
    }
}
