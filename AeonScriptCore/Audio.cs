namespace AeonScriptCore
{
    public class Audio
    {
        public enum ChannelType
        {
            SFX, BGM, Voice
        }

        public static float MasterVolume
        {
            get
            {
                return InternalCalls.Audio_GetMasterVolume();
            }
            set
            {
                InternalCalls.Audio_SetMasterVolume(value);
            }
        }

        public static void SetChannelTypeVolume(ChannelType channelType, float volume)
        {
            InternalCalls.Audio_SetChannelTypeVolume((int)channelType, volume);
        }

        public static float GetChannelTypeVolume(ChannelType channelType)
        {
            return InternalCalls.Audio_GetChannelTypeVolume((int)channelType);
        }

        /// <summary>
        /// You should call it only when you pause the game via GameState.IsPaused
        /// </summary>
        public static void GamePauseAllSounds()
        {
            InternalCalls.Audio_OnGamePause();
        }

        /// <summary>
        /// You should call it only when you unpause the game via GameState.IsPaused
        /// </summary>
        public static void GameUnpauseAllSounds()
        {
            InternalCalls.Audio_OnGameResume();
        }

        public static void StopAllSounds()
        {
            InternalCalls.Audio_StopAllSounds();
        }
    }
}
