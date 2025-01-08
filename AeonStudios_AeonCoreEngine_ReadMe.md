M3 MILESTONE, README 3.0 

Repository
==

*   AeonCore/         - Includes external dependencies and source code for AeonCore Engine.
*   AeonCoreScript/   - Files needed to handle the scripting API.
*   bin/              - Contains Debug and Release x64 for external usage after building.
*   Editor/           - Contains the source code for the editor application.
*   resources/        - Contains audio, meshes, textures, scripts, and shaders.
*   Executable/       - Contains the executable for the editor.
*   Game/             - Contains the game executable.
*   Compiler/         - Contains the compiler to compile the assets.


How To Run Executable
==
*   Navigate to M03_T06_Catastrophe_AeonStudios > AeonCore Executable > Release > Editor.exe.

How To Build Project
== 
*   Please install git for .bat.
*   Double-click on DownloadDependencies.bat; it will automatically download external dependencies to AeonCore\dependencies.
*   Note: Please download Git, Python, and install pip as well as gdown for the batch file to work. 
*   Then open GAM300_Team6.sln to open Visual Studio 2022.
*   There are a total of 5 projects:
        1. AeonCore         - This is the main source for AeonCore Engine.
        2. AeonScriptCore   - Handles API for scripting, such as Mono.
        3. Editor           - Entry point for the Editor.
        4. Compiler         - To compile resources.
        5. Game             - Entry point for our game.


Editor Usage Guide
==
*   To load the editor, right-click on Editor and select "Set as Start Up Project".
*   Choose either Debug or Release, then click Run.
*   Once the editor is started you will see a skybox right in the editor viewport.
*   You can choose to drag and drop a preloaded model from the panel to the viewport which 
    will create a new entity.
*   Or go ahead and right click new entity, with that you will be able to add any components you wish
    from the inspector. Note that some component cannot be added again, since is not intended too.
    For eg, we will not have a model and a font rendered on the same entity. Hence if a render component is added than
    a font component should not be added.
*   At the bottom right corner, a settings panel can be seen. This is where designer can choose to change the different
    camera view, as well as setting the ambient lighting. Note that, the changes made in this setting will not be saved as
    it was meant to be a one time event. 
*   To check for performance, you can click on the menu bar -> performance -> diagnostic then a panel should show the fps counter.
*   In the event you have an obj file, then you can drag the obj file to the render part in the inspector. An error message will be shown after compiling.
*   The binary file should be seen in the asset browser -> objects or preloaded models. 
*   You can add camera to any entity and change the view via the settings panel. 
*   Right-click on the entity to delete it or its child.
*   Scripts can be added and is hot reloaded in automation, Play Pause Stop to run the scripts.
*   Select the entity in the scene hierarchy and adjust the intention component on the right(inspector).
*   Camera component can only be added to entity that is neither a parent nor a child.
*   Go to File > Open to load scenes or drag the scene file from Asset Browser -> scene 
*   Shortcuts was implemented, the full list can be accessed via the menu bar at shortcuts

🌷🌷🌷 KNOWN ISSUES 🌷🌷🌷
*   Guizmo offset in viewport
*   Certain stuff not rendering when camera is at a far distance
*   Application will have to run first, for controller to be detected
*   To compile the .obj file to .bin file in the editor. It is required for the device to have, no space no periods in the filepath.
    wsl installed as well ensuring the file is used on C drive.
*   The limitation for spatial audio is that it requires the audio listener to be close to it to hear the 3D spatial audio
*   Animation is setup but might crash when load with other fbx, animation is still work in progress.
*   Game movement speed is depended on the computer performance, hence it might move slower or faster
*   GameViewport has been disabled due to some unforseen circumstances.
By Aeon Studios

🌺 RTIS 🌺
Rommel Sim 	- Technical Lead
Wai Kit 	- Graphics Lead
Yip Xuan 	- Architecture Lead
Shiru 		- Editor Lead
Felicia 	- Physics Lead 

🌸 IMGD 🌸 
En Qing 	- Scripting Lead
Valerie 	- Serialization Lead
Roy 		- Audio Lead
Zhong En 	- Input Lead

🍀 UXGD 🍀
Nicole 		- Audio Design Lead	
Ze Zheng	- Designer
Wesley		- Design Lead

