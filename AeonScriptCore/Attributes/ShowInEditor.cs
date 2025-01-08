using System;

namespace AeonScriptCore
{
    /// <summary>
    /// An attribute that makes a class's public field/variable visible for serialization and exposed to the Editor
    /// </summary>
    public sealed class ShowInEditor : Attribute
    {
    }
}
