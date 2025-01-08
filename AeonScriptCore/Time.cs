/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	Time.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines functions for Time; elapsed
            and delta time
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/
namespace AeonScriptCore
{
    public class Time
    {
        /// <summary>
        /// Time taken in one frame
        /// </summary>
        public static float DeltaTime
        {
            get
            {
                return InternalCalls.Time_GetDeltaTime();
            }
        }

        /// <summary>
        /// Time since the application started, basically is the application's run time
        /// </summary>
        public static float ElapsedTime
        {
            get
            {
                return InternalCalls.Time_GetElapsedTime();
            }
        }
    }
}
