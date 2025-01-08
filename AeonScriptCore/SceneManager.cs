namespace AeonScriptCore
{
    /// <summary>
    /// Allows you to manage the scene loading
    /// </summary>
    public class SceneManager
    {
        internal static string RequestedSceneLoad = "";
        internal static bool LoadInThread = false;
        internal static bool ShouldCache = false;

        internal static int LoadResourceRequestCount = 0;
        internal static int LoadResourceRequestLeft = 0;
        internal static bool HasSceneRead = false;

        public static int LoadingProgress
        {
            get
            {
                int progress = LoadResourceRequestCount - LoadResourceRequestLeft;
                float progressPercent = (progress / (float)LoadResourceRequestCount) * 99.0f; // multiply by 99, the 
                
                if (HasSceneRead)
                {
                    progressPercent += 1.0f;
                }

                progress = (int)progressPercent;
                if (progress < 0)
                    progress = 0;
                else if (progress > 100)
                    progress = 100;

                return progress;
            }
        }

        /// <summary>
        /// Load a scene. The scene loads only at the beginning of the next frame. By proxy, loading a scene will also make the existing scene "inactive".<br/>
        /// <b>NOTE:</b> You are making a REQUEST, so if you make another LoadScene call within the same frame, that will OVERWRITE your previous request.
        /// </summary>
        /// <param name="sceneFilepath">The filepath to the scene file, it must be relative to the "./resources/scene" folder. So if the scene file is inside the aforementioned folder just provide the scene filename, no need to prepend with "./"</param>
        /// <param name="loadInThread">Pass true only if you are loading a huge scene like a game, that can take awhile to load or you want to display a loading screen.</param>
        /// <param name="shouldCache">Caching will create a unique identifier based on the scene filepath that begins after "./resources/scene/", so if you gave a filepath like "./resources/scene/hello.scene", the cached scene's identifier is "hello.scene"</param>
        public static void LoadScene(string sceneFilepath, bool loadInThread = false, bool shouldCache = false)
        {
            RequestedSceneLoad = sceneFilepath;
            LoadInThread = loadInThread;
            ShouldCache = shouldCache;
        }

        /// <summary>
        /// Remove a scene from cache. This will not unload or disrupt the current active scene IF it was the targeted scene to "remove"
        /// </summary>
        /// <param name="sceneFilepath"></param>
        public static void RemoveSceneFromCache(string sceneFilepath)
        {
            InternalCalls.SceneManager_RemoveSceneFromCache(sceneFilepath);
        }
    }
}
