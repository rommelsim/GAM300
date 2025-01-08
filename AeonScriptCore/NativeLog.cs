/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	NativeLog.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines logging functions
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/

namespace AeonScriptCore
{
    public class NativeLog
    {
        /// <summary>
        /// Logs a message to the console
        /// </summary>
        /// <param name="message">The message you wish to log</param>
        public static void Log(string message)
        {
            InternalCalls.NativeLog_Log(message);
        }

        /// <summary>
        /// A variant of Log that takes in an additional argument to format the string
        /// </summary>
        /// <param name="message">The message you wish to log</param>
        /// <param name="args">Format arguments to apply to the message</param>
        public static void Log(string message, params object[] args)
        {
            InternalCalls.NativeLog_Log(string.Format(message, args));
        }
        
        /// <summary>
        /// A varaint of NativeLog.Log that logs a warning message to the console
        /// </summary>
        /// <param name="message">The message you wish to log</param>
        public static void LogWarn(string message)
        {
            InternalCalls.NativeLog_LogWarn(message);
        }

        /// <summary>
        /// A variant of LogWarn that takes in an additional argument to format the string
        /// </summary>
        /// <param name="message">The message you wish to log</param>
        /// <param name="args">Format arguments to apply to the message</param>
        public static void LogWarn(string message, params object[] args)
        {
            InternalCalls.NativeLog_LogWarn(string.Format(message, args));
        }
        
        /// <summary>
        /// A varaint of NativeLog.Log that logs an error message to the console
        /// </summary>
        /// <param name="message">The message you wish to log</param>
        public static void LogError(string message)
        {
            InternalCalls.NativeLog_LogError(message);
        }

        /// <summary>
        /// A variant of LogError that takes in an additional argument to format the string
        /// </summary>
        /// <param name="message">The message you wish to log</param>
        /// <param name="args">Format arguments to apply to the message</param>
        public static void LogError(string message, params object[] args)
        {
            InternalCalls.NativeLog_LogError(string.Format(message, args));
        }
    }
}
