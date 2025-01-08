/*---------------------------------------------------------
 * project:	Aeon Studios - Cat-astrophe
 * file:	Graphics.cs
 * authors:
		Lam En Qing				(enqing.lam@digipen.edu)
 *
 * brief:	This file defines a Graphics class and various
            Graphics-related types such as Format and Layer enums
 *
 *
 * All content © 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
-----------------------------------------------------------*/
namespace AeonScriptCore
{
    public class Graphics
    {
        public static Vector3 EnvironmentLightColor
        {
            get
            {
                InternalCalls.Graphics_GetEnvironmentLightColor(out var lightColor);
                return lightColor;
            }
            set
            {
                InternalCalls.Graphics_SetEnvironmentLightColor(ref value);
            }
        }

        /// <summary>
        /// Gamma is clamped between 1.0f and 10.0f
        /// </summary>
        public static float Gamma
        {
            get
            {
                return InternalCalls.Graphics_GetGamma();
            }
            set
            {
                InternalCalls.Graphics_SetGamma(value);
            }
        }

        /// <summary>
        /// A boolean, to enable or disable HDR
        /// </summary>
        public static bool HasHDR
        {
            get
            {
                return InternalCalls.Graphics_GetHDR();
            }
            set
            {
                InternalCalls.Graphics_SetHDR(value);
            }
        }

        public static void RequestSwitchCamera(uint ownerIdOfCamera)
        {
            InternalCalls.Graphics_RequestSwitchCamera(ownerIdOfCamera);
        }
    }

    /// <summary>
    /// LightType is for Lighting component, it decides the type of light that should render
    /// </summary>
    public enum LightType
    {
        Spotlight = 0, Point, Directional
    }

    /// <summary>
    /// Format is for UI component, it decides whether UI should display an image or text
    /// </summary>
    public enum FormatType
    {
        Image = 0, Word
    }

    /// <summary>
    /// Layer is for UI component, to group several UI entities together on one layer. Default layer will render on top the others, followed by Foreground then Background
    /// </summary>
    public enum LayerType
    {
        Default = 0, Foreground, Background
    }
}
