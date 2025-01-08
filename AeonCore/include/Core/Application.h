#pragma once
/*!***************************************************************************
	\file			Application.h
	\author			Team
	\par			Team
	\brief			This header file consists of functions to initialize,
					run the Engine loop, and receieve events for engine to act on.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "AeonAPI.h"
#include "Window/Window.h"
#include "Rendering/Layer.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Editor/ImGuiLayer.h"
#include "Editor/RenderLayer.h"

#include <functional> // function
#include <thread> // scoped_lock, mutex

namespace AeonCore
{
	/// <summary>
	/// Defines the runtime states, primarily used for Editor. But can be used in systems' Update to prevent their update code from running depending on the runtime state.\n
	///	Start means runtime is running, Pause means runtime is paused, Stop means runtime is stopped/not running.
	/// </summary>
	enum class AEON_API RuntimeState {
		Start, Pause, Stop
	};

	/// <summary>
	/// Defines the load status, used when loading scene via scripts.
	/// </summary>
	enum class LoadStatus {
		ReadScene, // Load thread is reading scene file and also makes requests to load resource(s) at a later stage
		LoadResources, // Load thread waits for main thread to load resources one per frame by fulfilling all the load resource requests
		Finish, // All load resource requests have been fulfilled, so loading is complete. Main thread will attempt to join the load thread (to close the thread)
		NotRunning // Load thread is not running currently
	};

	class AEON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		/// <summary>
		/// This function allows implementations of Application to "hook" into the Run loop.
		/// This function will be called in the run loop just before the layers start rendering
		/// </summary>
		virtual void Update();

		void OnEvent(Event &_event);

		void PushLayer(Layer *_layer);
		void PushOverlay(Layer *_layer);

		/// <summary>
		/// Start the runtime, sets the runtime state to Run. Calls OnStart function from other parts, like Scripting::OnStart.\n
		///	If runtime state is Pause when this is called, OnStart functions from other parts won't be called, since this logic is considered "unpausing"
		/// </summary>
		void OnRuntimeStart();
		/// <summary>
		/// Pauses the runtime, sets the runtime state to Pause
		/// </summary>
		void OnRuntimePause();
		/// <summary>
		/// Stops the runtime, sets the runtime state to Stop. Calls OnStop function from other parts, like Scripting::OnStop
		/// </summary>
		void OnRuntimeStop();

		/// <summary>
		/// Returns the runtime state of the engine 
		/// </summary>
		/// <returns></returns>
		RuntimeState GetRuntimeState();

		/// <summary>
		/// Returns the load status of the engine
		/// </summary>
		/// <returns></returns>
		LoadStatus GetLoadStatus();

		/// <summary>
		/// Get the game's paused state
		/// </summary>
		/// <returns></returns>
		bool IsPaused();

		void SetIsPaused(bool value);

		inline Window &GetWindow()					{ return *mWindow; }
		inline static Application &GetApplication() { return *sInstance; }

		// Intended to be called from non-main threads
		void SubmitToMainThread(const std::function<void()>& func);

		/// <summary>
		/// Submit a lambda function that loads resources to run when LoadStatus is LoadResources on the main thread
		/// </summary>
		/// <param name="func">A lambda function that will contain a short code block to load resources</param>
		void SubmitToLoadResourcesQueue(const std::function<void()>& func);

		//Exit
		void Close();

		//	boolean to differentiate between editor and game
		inline bool IsEditorOn() { return mImGuiLayer != nullptr; }
	protected:
		//	overlay
		ImGuiLayer* mImGuiLayer	{nullptr};

		// list of functions to invoke on main thread, primarily would be used in the filewatch's lambda functions
		std::vector<std::function<void()>>	mMainThreadQueue; // when Script file change is detected, it will send it's reload assembly invocation wrapped in a lambda into this vector, then PreUpdate runs
	private:
		bool OnWindowClose(WindowCloseEvent& _event);
		void ExecuteMainThreadQueue();

		LoadStatus					mLoadStatus = LoadStatus::NotRunning;
		std::thread					mSceneLoadThread;
		std::queue<std::function<void()>> mThreadLoadResourceRequests;

		std::mutex					mMainThreadQueueMutex; // to lock a function's running to one thread

		std::unique_ptr<Window>		mWindow;
		bool						mIsRunning = true; // run loop's while condition
		RuntimeState				mRuntimeState = RuntimeState::Stop; // runtime state, by default initialized to Stop state
		bool						mIsPaused = false; // used to pause the engine, typically only physics should pause
		LayerStack					mLayerStack;
		RenderLayer*				mRenderLayer;
		static Application			*sInstance;

		friend class SceneManager;
	};

	Application *CreateApplication();
}