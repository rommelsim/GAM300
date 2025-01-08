/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	GameState.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines functions for a game's state
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/

using System.ComponentModel.Design;

namespace AeonScriptCore
{
    public class GameState
    {
        // We only modify this value in the EngineCallable
        internal static bool GamePaused = false;
        // holds the value written by IsPaused, to later be used in Engine callable
        internal static bool GamePauseStateRequest = false;
        // a request state to pause the engine
        internal static bool EnginePauseStateRequest = false;

        public static bool IsPaused
        {
            get
            {
                return GamePaused;
            }
            set
            {
                // when script sets IsPaused, they are only making a request. The request is fulfilled at the end of the frame
                GamePauseStateRequest = value;
            }
        }

        /// <summary>
        /// This will make a request to pause the engine side. The request will be fulfilled at the end of the frame.<br/>
        /// Currently, it only pauses the physics simulation.
        /// </summary>
        /// <param name="pauseState"></param>
        public static void SetEnginePauseState(bool pauseState)
        {
            EnginePauseStateRequest = pauseState;
        }

        /// <summary>
        /// Retrieve the engine's pause state
        /// </summary>
        /// <returns>A boolean</returns>
        public static bool GetEnginePauseState()
        {
            return InternalCalls.GameState_IsPaused();
        }

        /// <summary>
        /// Quit the game!
        /// </summary>
        public static void QuitGame()
        {
            InternalCalls.GameState_QuitGame();
        }
    }
}
