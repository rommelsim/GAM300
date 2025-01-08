/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	EngineCallable.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines functions intended to be called
            from the Engine
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace AeonScriptCore
{
    using SceneID = System.Byte; // 8 bits unsigned integer

    internal class EngineCallable
    {
        internal static void ScriptPostUpdate()
        {
            foreach (Action action in EventHandler.ScriptPostUpdateEvent)
            {
                action();
            }
        }

        internal static void FrameEnd()
        {
            foreach (Action action in EventHandler.FrameEndEvent)
            {
                action();
            }

            if (SceneManager.RequestedSceneLoad.Length > 0)
            {
                EntityBuilder.entityCreateRequests.Clear();
                if (SceneManager.LoadInThread)
                {
                    InternalCalls.SceneManager_LoadSceneThread(SceneManager.RequestedSceneLoad, SceneManager.ShouldCache);
                }
                else
                    InternalCalls.SceneManager_LoadScene(SceneManager.RequestedSceneLoad, SceneManager.ShouldCache);
                SceneManager.RequestedSceneLoad = "";
            }

            //if (EntityBuilder.entityCreateRequests.Count > 0)
            //{
            //    NativeLog.LogWarn("C#'s Frame End!");
            //}

            foreach (EntityBuilder entity in EntityBuilder.entityCreateRequests)
            {
                entity.Create();
            }
            EntityBuilder.entityCreateRequests.Clear();

            if (GameState.GamePaused != GameState.GamePauseStateRequest)
            {
                GameState.GamePaused = GameState.GamePauseStateRequest;
            }
            if (InternalCalls.GameState_IsPaused() != GameState.EnginePauseStateRequest)
            {
                InternalCalls.GameState_SetIsPaused(GameState.EnginePauseStateRequest);
            }
        }

        internal static void OnScriptingStop()
        {
            GameState.EnginePauseStateRequest = GameState.GamePaused = GameState.GamePauseStateRequest = false;
        }

        internal static void SetHasSceneRead([MarshalAs(UnmanagedType.Bool)] bool isRead)
        {
            SceneManager.HasSceneRead = isRead;
            if (!isRead)
            {
                SceneManager.LoadResourceRequestCount = SceneManager.LoadResourceRequestLeft = 0;
            }
        }

        internal static void SetLoadResourceRequestCount(int count)
        {
            SceneManager.LoadResourceRequestCount = count;
            SceneManager.LoadResourceRequestLeft = count;
        }

        internal static void SetLoadResourceRequestLeft(int count)
        {
            SceneManager.LoadResourceRequestLeft = count;
        }

        // Would be called in C++ Scene's Destructor
        internal static void SceneDestroyed(SceneID sceneID)
        {
            NativeLog.LogWarn("SceneDestroyed {0}", sceneID);
            Entity._entities.Remove(sceneID);
        }
    }
}
