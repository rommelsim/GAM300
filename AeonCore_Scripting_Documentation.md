# Welcome to AeonCore's Scripting Documentation!
This documentation will not cover nor describe all functions that every class has, it will only describe how to use some of the functions.
The AeonScriptCore project is available for you to look through in more in-depth details, like exploring the available functions that each class has.  
You can also explore a class's function from the AeonScripts project via the Intellisense tool. Write `Vector3.` and a list of options should appear. If it does not appear, use the shortcut `Ctrl + Space`

## Table of Contents
1. [Available Classes](#available-classes)
1. [AeonBehaviour's Functions](#aeonbehaviours-functions)
1. [Basic workflow of a script](#basic-workflow-of-a-script)
1. [Detecting Inputs](#detecting-inputs)
1. [Create an Entity](#create-an-entity)
1. [Create Timers](#create-timers)
1. [Pausing Game](#pausing-game)
1. [Scene Cache](#scene-cache)

## Available Classes
There are several classes and structs that you can use. Some of them  exposes static functions and variables. Classes with static functions are marked with *, classes with static variables are marked with ^.  
They are:  
* Entity*
* Audio*
* GameState*^
* Graphics*^
* Input*
* Keys^
* Vector2*
* Vector3*^
* MathHelper*^
* NativeLog*
* SceneManager*^
* Time^

For the Entity-Component classes, there are:
* Entity
* AudioComponent
* Transform
* Camera
* RigidBody
* Collider
* Render
* Lighting
* Particles
* UI

## AeonBehaviour's Functions
AeonBehaviour has a public variable `entity` whose type is `Entity`. `entity` represents the script's owner entity, that is the entity that the script is attached to. There are 4 functions and 5 physics-event functions. The 4 functions are: OnCreate, OnCachedSceneActive, OnCachedSceneInactive, Update. The 5 physics-event functions are: OnCollisionEnter, OnCollisionUpdate, OnCollisionExit, OnTriggerEnter, OnTriggerExit.

It is my understanding that the physics-event functions will always run before `Update` does, because the Physics system will update first, then the Scripting system updates.
* `OnCollisionXXX` will occur when 2 entities that have RigidBody & Collider, as well as the IsTrigger flag is not enabled for both.
* `OnTriggerXXX` will occur when an entity's collider (IsTrigger disabled) enters another entity's trigger collider (IsTrigger enabled)  
* `XXXEnter` will run on the first frame that a collision or collider entered a trigger.  
* `OnCollisionUpdate` runs while collision still occurs
* `XXXExit` will run on the frame that a collision ended or a collider exited a trigger.

`OnCreate` is called **only once** when the script's instance is created. Instantiation occurs either on Play in Editor application, or at the start of a new frame after all entities have been created in the previous frame.  
`OnCachedSceneActive` is called only when a cached scene is made the active scene. It's not called when you load a scene for the first time.  
`OnCachedSceneInactive` is called only when a cached scene is no longer the active scene, that is when another scene replaces it as the active scene, making the current cached scene go into the background.  
`Update` is called once every frame.

Scripts' Update running order is determined by:
1. entities arrangement in the scene hierarchy, first entity at the top gets called, and so on.
2. order of which the scripts are attached to the Behaviour component, if a script is at the top, it runs first.

## Basic workflow of a script
A typical Behaviour script should be structured in a way that you do not call `GetScript` or `GetComponent` every frame! Instead, you want to save them to a class-level variable and use the getter when necessary! Such an example could be:
<details>

<summary>Code snippet</summary>

```csharp
public class Example : AeonBehaviour
{
	Transform transComp = null; // Must initialize to null first!!

	public override void OnCreate()
	{
		transComp = entity.GetComponent<Transform>(); // Simply pass in the type of the component between the < >
	}

	public override void Update()
	{
		// If Transform component is required for the rest of your code.
		// This GetComponent will keep running until transComp has a value, but not recommended. By engine's design, all entities has a Transform component.
		if (transComp == null)
		{
			// If you want to do a null-safety check, by all means please do. But I don't recommend GetComponent in Update.
			transComp = entity.GetComponent<Transform>();
			return;
		}

		// Rest of the code
	}
}
```
</details>
&nbsp;

Let's say you have a public variable that you wish to expose to other scripts but you don't want it to be exposed to the Editor, you can use the `HideFromEditor` attribute.
```csharp
public class Example : AeonBehaviour
{
	[HideFromEditor]
	public float Health;
}
```

Similarly, if you have a private variable that you do not wish to expose to other scripts but you want to expose it to the Editor, you can use the `ShowInEditor` attribute.
```csharp
public class Example : AeonBehaviour
{
	[ShowInEditor]
	private string secret;
}
```

## Detecting Inputs
AeonScriptCore has an `Input` class which provides function to detect key & gamepad input, a `Keys` class which defines static members of most keys, a `Input.JoystickInput` enum for gamepad buttons. There's 2 ways to detect input, either by directly checking the input or via mappings. You can think "mappings" as like how games have keybindings settings for you to change the underlying keybind for a specific action.

### Direct Input
To directly detect keyboard/mouse input, `Input` provides the following functions:
* `GetKeyPressed`: Returns true in the first frame that a user presses down on a key
* `GetKeyDown`: Returns true while user is holding down on a key
* `GetMouseButtonHeld`: Returns true while user is holding on a mouse button

Each of these functions takes in an integer. You will use the static members defined in `Keys` class to pass to these functions. Mouse buttons are also defined inside the `Keys` class.

To directly detect gamepad input, `Input` provides the following functions:
* `GetConnectedGamepadIds`: Returns an array of integers. The integers represent the respective detected gamepads' ids.
* `IsGamepadConnected`: Returns true if the provided gamepad id integer is detected to be connected
* `IsGamePadButtonPressed`: Returns true if, for a valid gamepad id, a gamepad button was pressed. The functions parameters are `int gamePadId, JoystickInput joystickInput`
* `GetGamePadAxisValue`: Returns a float value in the range of -1.0 to 1.0 for a given gamepad input axis. If the provided gamepad id is not valid, 0.0 is returned. The function paremeters are `int gamePadId, JoystickInput joystickInput`

`JoystickInput` is an enum, it's values represents the various gamepad buttons and axes. Axes are the left & right thumbsticks' x & y, and the left & right triggers.  
**Beware** that you need to ensure that `GetConnectedGamepadIds`'s array length is more or equals to 1 if you want to extract one id. If you want 2, you need to check that the length is more or equals to 2.

### Mappings
For mappings, you need to register a mapping first, then you can detect input via the mappings.

To register, you need a mapping name, to uniquely identify the mapping. Then you also need to provide the keyboard or mouse inputs and the gamepad input. This allows you to detect for both keyboard and gamepad input under a single mapping. The function is as described:
`public static void RegisterMapping(string mappingName, KBMInputType kbmInputType, int kbmCode, JoystickInput joystickInput)`  
The parameter descriptions is as follows:
* `mappingName`: The mapping name
* `kbmInputType`: The type of input for non-joystick, is it keyboard or mouse
* `kbmCode`: The key to assign to the mapping, can either be a Key's code or a Mouse's code. Use the `Keys` class
* `joystickInput`: The gamepad input for the mapping.

To detect the mapping input, there are 4 functions:
* `GetMappingKeyPressed`: Has only a string parameter, the mapping name. Returns true in the first frame that a user pressed the key input
* `GetMappingKeyHeld`: Has only a string parameter, the mapping name. Returns true while the user is holding on a key.
* `GetMappingGamePadButtonPressed`: Has a string and an integer parameters, the mapping name and gamepad id respectively. Returns true in the first frame that a user pressed the joystick button input. **Note:** If the registered joystick input is not a button or the provided gamepad id is invalid, the function will return false.
* `GetMappingGamePadAxisValue`: Has a string and an integer parameters, the mapping name and gamepad id respectively. Returns a float value in the range of -1.0 to 1.0 for a given gamepad input axis. **Note:** If the registered gamepad input is not an axis input or the provided gamepad id is invalid, 0.0 is returned.

## Create an Entity
To create an entity, you'll need to use `EntityBuilder` to construct information for your entity. The constructor takes in a string, the name of an entity.
```csharp
EntityBuilder myEnt = new EntityBuilder("My new entity");
```
Then to add components to your new entity, you need to use the `AddComponent` function which takes in an instance of `IComponentBuilder`.
<details>
	<summary>List of component builders</summary>

	* TransformBuilder
	* RigidBodyBuilder
	* ColliderBuilder
	* RenderBuilder
	* UIBuilder
	* BehaviourBuilder
	* AudioBuilder
	* ParticlesBuilder
</details>

Let's add a Transform component to our entity.  
**Note:** If you did not add a TransformBuilder to the EntityBuilder, a TransformBuilder will automatically be created for you with default values. To see what are the default values, you can hover over the constructor of TransformBuilder to see the documentation.
```csharp
TransformBuilder transform = new TransformBuilder();
myEnt.AddComponent(transform);
```

Next, let's give our entity some collision. We'll have to add both a RigidBody and Collider in order for collision to work. That's how our engine's physics is designed.
```csharp
// You can chain AddComponent calls together!! Similarly for the component builders, you can chain the setters together! Pretty convenient huh?
myEnt.AddComponent(new RigidBodyBuilder().SetUseGravity(true).SetType(AeonScriptCore.Physics.RigidBodyType.DYNAMIC))
	.AddComponent(new ColliderBuilder().SetShape(new ColliderBuilder.CubeShapeBuilder()));
```

Let's also give our entity some logic using the `BehaviourBuilder`!
```csharp
// You can also chain AddScript calls together too!
myEnt.AddComponent(new BehaviourBuilder().AddScript<MoveCharacter>().AddScript<GameBGM>());
```

Lastly, to "register" our entity for creation, we use `Entity.Create`. Entities will be created at the end of a frame. You can consider this as making a "entity create" request which gets fulfilled at the end of a frame.
```csharp
// second parameter is optional, you do not need to write it at all. You could instead write: Entity.Create(myEnt); And this will compile fine.
Entity.Create(myEnt, (entity) => { // This is called a Statement Lambda, it creates an anonymous function.
	// entity is just a variable name, you can call it however you like.

	// Do something after the creation of the entity and it's components, like say setting a Behaviour's fields
	MoveCharacter moveChar = entity.GetScript<MoveCharacter>();

	// Null-safety check!!
	if (moveChar != null) {
		NativeLog.Log("Our new entity has a MoveCharacter script!");
	}
})
```

If you got confused, don't worry here's the full code snippet.
<details>
<summary>Click Me</summary>

```csharp
EntityBuilder myEnt = new EntityBuilder("My new entity");

TransformBuilder transform = new TransformBuilder();
myEnt.AddComponent(transform);

// You can also chain AddComponent calls together!! Similarly for the component builders, you can chain the setters together! Pretty convenient huh?
myEnt.AddComponent(new RigidBodyBuilder().SetUseGravity(true).SetType(AeonScriptCore.Physics.RigidBodyType.DYNAMIC))
	.AddComponent(new ColliderBuilder().SetShape(new ColliderBuilder.CubeShapeBuilder()));

// You can also chain AddScript calls together too!
myEnt.AddComponent(new BehaviourBuilder().AddScript<MoveCharacter>().AddScript<GameBGM>());

// second parameter is optional, you do not need to write it at all. You could instead write: Entity.Create(myEnt); And this will compile fine.
Entity.Create(myEnt, (entity) => { // This is called a Statement Lambda, it creates an anonymous function.
	// entity is just a variable name, you can call it however you like.

	// Do something after the creation of the entity and it's components, like say setting a Behaviour's fields
	MoveCharacter moveChar = entity.GetScript<MoveCharacter>();

	// Null-safety check!!
	if (moveChar != null) {
		NativeLog.Log("Our new entity has a MoveCharacter script!");
	}
})
```
</details>

## Create Timers
To create timers, we need a way to measure how much time has passed. An accurate way to measure time in our game is by using the delta time. The delta time is the time taken to complete the previous frame, that is, the time measured from the start of the last frame to the start of the current frame.

To get the delta time, we use the `Time` class to access the `DeltaTime` variable. Then for the actual timer, we simply accumulate the time passed.  
**Note:** The time unit in our engine is seconds!!
<details>
<summary>Code snippet</summary>

```csharp
public class TimerExample : AeonBehaviour
{
	float timer = 0.0f;
	float countdown = 5.0f;

	public override void Update()
	{
		// On next frame, when time exceeded limit.
		// You can put this before or after the accumulation, it's only a frame delay difference, but that could be very important! So plan your logic well!
		if (timer > 10.0f) {
			// Do something
		}

		timer += Time.DeltaTime;

		// I personally prefer subtracting from the timer for countdown timers, it just logically makes sense to me
		countdown -= Time.DeltaTime;
	}
}
```
</details>

## Pausing Game
There are 2 pause states, one for engine, the other for game scripts.
When you pause a state, you are **only** making a _request_ to change the pause state. The state will change at the end of the frame.  
**Note:** When you set the pause state and retrieve the pause state in the _same_ frame, you are retrieving the **unchanged** pause state. Meaning that the pause state you retrieve is not the one you are changing to.

Engine's pause state will only pause the physics simulation, so the physics won't run when engine is paused.
Game scripts' pause state is to pause any running logic of the script when paused.

### Engine's pause state
Pausing the engine is done by calling the `GameState.SetEnginePauseState` function. The function has a boolean parameter, the new pause state.

Retrieving the engine's pause state can be done by calling `GameState.GetEnginePauseState`.

### Game scripts' pause state
Pausing the game scripts is in 2 parts: setting a boolean flag, using the boolean flag to pause any logic in the Update and physics functions.

Setting and getting the boolean flag is by accessing the `IsPaused` boolean variable from `GameState` class. `GameState.IsPaused`.

Then to pause your logic.
```csharp
public override void Update()
{
	// Some pre-requisite code that must run before the pause, like say updating a UI or something

	if (GameState.IsPaused) return; // return means you exit the function at this point, ignoring the rest of the function's code.

	// Rest of the code that should not run when in paused state!
}
```

## Scene Cache
A new feature for M5. A scene is cached when you load a scene for the first time. The idea is that you want to save the state of your scene when you switch to another scene. For example we paused our game scene then we want to switch to our settings scene, but we want to save the state of the game scene, how can we do that? That is achieved by saving the game scene into the cache when we load. Another purpose of scene cache is to not keep loading scenes again and again. Instead, simply retrieve the scene from the cache. So how do we get started?

Before I list and explain the functions available, you **must be aware** of the limitations with regards to scene cache.
1. You can only have a maximum of 252 cached scenes. The active scene is counted as the 253rd. If you try to load more scenes after that, the game will crash or lead to unexpected behaviour.
2. You cannot load another physics-based scene while a physics-based scene is already loaded either as an active scene or in the cache. You need to remove the prior physics scene from cache, then when you set a new active scene, the formerly-cached scene will be destroyed. If it's not cached, don't make it the active scene before loading the new scene.

### Functions Explanation
The `SceneManager` class provides 2 functions:
* `LoadScene`: to load a scene (you can configure how to load it)
* `RemoveSceneFromCache`: to remove a scene from cache

Let's talk about the first function, `LoadScene`. The function will load a scene. The scene loads only at the beginning of the next frame. By proxy, loading a scene will also make the current scene "inactive". If the current scene is _**not cached**_, it will be destroyed rather than become "inactive".  
**NOTE:** You are making a <u>request</u>, so if you make another LoadScene call within the same frame, that will <u>overwrite</u> your previous request.
```csharp
public static void LoadScene(string sceneFilepath, bool loadInThread = false, bool shouldCache = false) {}
```
It has 3 parameters, of which the last 2 are optional.
The parameters breakdown:
* `sceneFilepath`: The filepath to the scene file, it must be relative to the "./resources/scene" folder. So if the scene file is inside the aforementioned folder just provide the scene filename, no need to prepend with "./resources/scene". But if you do have that prefix path, no worries the LoadScene will "sanitise" the filepath such that the filepath would be the one that comes after "scene/" folder
* `loadInThread`: Pass true **only** if you are loading a huge scene like a game, that can take awhile to load. When loading the scene, the loading process is delegated to a thread. In the meantime, a loading scene becomes the active scene.
* `shouldCache`: Caching will create a unique identifier based on the scene filepath that begins after `./resources/scene/`, so if you gave a filepath like `./resources/scene/hello.scene`, the cached scene's identifier is "hello.scene".

Now let's talk about the second function, `RemoveSceneFromCache`. The function will remove a scene from cache. If the removed scene is not the current active scene, the scene will be destroyed. But if the removed scene is the active scene, then the scene will be destroyed when it's no longer the active scene.
```csharp
public static void RemoveSceneFromCache(string sceneFilepath) {}
```
It has 1 parameter, the scene filepath. The filepath to the scene file, it must be relative to the "./resources/scene" folder. So if the scene file is inside the aforementioned folder just provide the scene filename, no need to prepend with "./resources/scene". But if you do have that prefix path, no worries the LoadScene will "sanitise" the filepath such that the filepath would be the one that comes after "scene/" folder

### Example scenario
Let's say I have the following scenes:
* Main Menu
* Settings Menu
* Game Level - Has physics
* Score Results - Has physics, for the cats

When I start the game, I cache & load the Main Menu scene. Then going from Main Menu to Game Level, I cache & load on thread the Game Level scene. Next, I pause the game. Then in the pause menu, I cache & load Settings Menu. After changing the settings, I go back to the Game Level scene and unpaused the game.  
So far, all these are calling only the `LoadScene` function.  
Next, I finished the game and the game brings me from Game Level scene to Score Results scene. This is **important**, you must remove the Game Level scene from cache **before** loading the Score Results scene