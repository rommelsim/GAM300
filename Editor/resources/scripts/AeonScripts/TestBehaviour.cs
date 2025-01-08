using AeonScriptCore;

namespace AeonScripts
{
    /// <summary>
    /// This TestBehaviour is simply our "test script".
    /// A means of getting and setting the class fields from the Editor, as well as correlate them to the ScriptDataType enum.
    /// And testing that the methods can be retrieved and invoked.
    /// </summary>
    public class TestBehaviour : AeonBehaviour
    {
        [ShowInEditor]
#pragma warning disable CS0414 // The field 'TestBehaviour.Name' is assigned but its value is never used
        private string Name = "Hello";
#pragma warning restore CS0414 // The field 'TestBehaviour.Name' is assigned but its value is never used
        //public int Health = -5;
        //public int MaxHealth;
        //public bool ShouldFly = true;

        /*public char myChar = 'a';
        /*public float myFloat = 6.9f;
        public double myDouble = 2.9;
        public uint myUInt = 50;

        public sbyte mySbyte;
        public byte myByte;
        public short myShort;
        public ushort myUShort;
        public long myLong;
        public ulong myULong;*/

        private bool _hasPrinted = false;

        public override void OnCreate()
        {
            Input.RegisterMapping("jump", Input.KBMInputType.Keyboard, Input.KEY_SPACE, Input.JoystickInput.Button_A);
        }

        public override void Update()
        {
            if (!_hasPrinted)
            {
                AutoMoveAround autoMoveScript = entity.GetScript<AutoMoveAround>();
                MoveCharacter moveCharScript = entity.GetScript<MoveCharacter>();
                CameraFollow camFollowScript = entity.GetScript<CameraFollow>();

                NativeLog.LogWarn("AutoMoveAround, MoveCharacter, CameraFollow: {0}, {1}, {2}", autoMoveScript != null, moveCharScript != null, camFollowScript != null);

                if (autoMoveScript != null)
                {
                    NativeLog.LogWarn("TestBehaviour - AutoMoveAround: {0}", autoMoveScript.TravelDistance);
                }
                if (moveCharScript != null)
                {
                    NativeLog.LogWarn("TestBehaviour - MoveCharacter: {0}", moveCharScript.useController);
                }
                if (camFollowScript != null)
                {
                    NativeLog.LogWarn("TestBehaviour - CameraFollow: {0}", camFollowScript.PlayerName);
                }

                _hasPrinted = true;
            }

            if (Input.GetMappingKeyPressed("jump"))
            {
                NativeLog.LogError("Jump mapping was pressed!");
            }

            if (Input.GetKeyPressed(Input.KEY_F))
            {
                NativeLog.LogWarn("Key F was pressed!");
            }
        }
    }
}
