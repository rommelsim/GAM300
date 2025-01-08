using System;

namespace AeonScriptCore
{
    /// <summary>
    /// An attribute that hides a class's public field/variable from serialization and being exposed to the Editor
    /// </summary>
    public sealed class HideFromEditor : Attribute
    {
    }
}
