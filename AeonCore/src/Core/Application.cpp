#include "acpch.h"
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
#include "Core/Application.h"
#include "Core/Engine.h"
#include "Core/SceneManager.h"
#include "Core/TimeStep.h"
#include "Audio/FmodCore.h"
#include "Camera/CameraManager.h"
#include "Input/Input.h"
#include "Physics/ACPhysicsInternal.hpp"
#include "Scripting/Scripting.h"
#include <ECS/Systems/Renderer2DSystem.h>
#include "Tracy.hpp"

namespace AeonCore
{
	Application* Application::sInstance = nullptr;
	//const float targetFPS = 120.0f;
	//const float targetFrameTime = 1.0f / targetFPS;

	Application::Application()
	{
		sInstance = this; //@todo-debug assert to check if its a singleton //todo a singleton class instead of static

		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(AEON_BIND_EVENT_FUNC(Application::OnEvent));
		
		Scripting::Init();
		SceneManager::Init();

		ACPhysics::PhysicsInternal::GetInstance().Init();
		{
			Engine::GetInstance().Init();
		}

		SceneManager::SetLoadingScreenScene(SceneManager::LoadScene("LoadPlaceHolder.scene", false));
	}
	Application::~Application()
	{
		//FMODCoreClass::GetInstance().CShutdown();
		Engine::GetInstance().Destroy();
		ACPhysics::PhysicsInternal::GetInstance().Shutdown();
		ResourceManager::GetInstance().Destroy();
		mLayerStack.~LayerStack();					// Need further testing...

		SceneManager::Destroy();
		Scripting::Exit();
	}

	void Application::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		dispatcher.Dispatch<WindowCloseEvent>(AEON_BIND_EVENT_FUNC(Application::OnWindowClose));

		for (auto iter = mLayerStack.end(); iter != mLayerStack.begin(); )
		{
			(*--iter)->OnEvent(_event);
			if (_event.mHandled)
				break;
		}
	}

	void Application::PushLayer(Layer* _layer)
	{
		mLayerStack.pushLayer(_layer);
		_layer->OnAttach();
	}

	void Application::PushOverlay(Layer* _layer)
	{
		mLayerStack.pushOverlay(_layer);
		_layer->OnAttach();
	}

	void Application::OnRuntimeStart() {
		RuntimeState prevState = mRuntimeState;
		mRuntimeState = RuntimeState::Start;

		CameraManager::GetInstance().SetRequestSwitch(true);
		CameraManager::GetInstance().SetEnableEntityCam(true);

		if (prevState == RuntimeState::Stop) {
			Scripting::OnStart();
		} else { // it was from pause state, so it's resuming
			FMODCoreClass::GetInstance().ResumeAllChannels();
		}
	}

	void Application::OnRuntimePause() {
		mRuntimeState = RuntimeState::Pause;
		CameraManager::GetInstance().SetActiveCamera(&EditorCamera::GetInstance());
		FMODCoreClass::GetInstance().PauseAllChannels();
	}

	void Application::OnRuntimeStop() {
		mRuntimeState = RuntimeState::Stop;

		while (!mThreadLoadResourceRequests.empty())
			mThreadLoadResourceRequests.pop();

		CameraManager::GetInstance().SetActiveCamera(&EditorCamera::GetInstance());
		CameraManager::GetInstance().SetEnableEntityCam(false);
		Scripting::OnStop();
		SceneManager::GetInstance().ClearSceneCache();
		FMODCoreClass::GetInstance().StopAllChannels();

		SetIsPaused(false);
	}

	RuntimeState Application::GetRuntimeState() {
		return mRuntimeState;
	}

	LoadStatus Application::GetLoadStatus() {
		return mLoadStatus;
	}

	bool Application::IsPaused() {
		return mIsPaused;
	}

	void Application::SetIsPaused(bool value) {
		mIsPaused = value;
	}

	// renderQuad() renders a 1x1 XY quad in NDC
	// // -----------------------------------------
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void Application::Run()
	{
		AeonCore::Coordinator& gCoordinator = AeonCore::Coordinator::GetInstance();

		while (mIsRunning)
		{
			//////////////////////////////
			//			Update			//
			//////////////////////////////

			/*if (mRequestUnloadScene) {
				SceneManager::UnloadScene();
				mRequestUnloadScene = false;
			}

			if (!mRequestedSceneLoad.empty()) {
				SceneManager::LoadScene(mRequestedSceneLoad);
				mRequestedSceneLoad = "";
			}*/

			Scripting::RunScriptInstancesOnCreate();

			for (Layer* layer : mLayerStack)
				layer->OnUpdate();		// Does this actually do anything? - Rommel

			mWindow->OnUpdate();

			TimerStep::GetInstance().update();

			ExecuteMainThreadQueue();

 			Engine::GetInstance().Update();
 			Update();
			if (mImGuiLayer) {
				ZoneScopedN("IMGUI Render Update");
				mImGuiLayer->Begin();
				for (Layer* layer : mLayerStack)
				{
					layer->OnImGuiRender();
				}
				mImGuiLayer->End();
			}

			// Game is running
			else /*if (GetApplication().GetLoadStatus() == AeonCore::LoadStatus::NotRunning)*/
			{
				ZoneScopedN("App else part Update");
				unsigned int textureID = FramebufferManager::GetInstance().GetFrameTexture("f1");
				auto FramebufferID = FramebufferManager::GetInstance().GetFrameBufferID("f1");

				// Bind back to default framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, mWindow->GetWidth(), mWindow->GetHeight());
				glScissor(0, 0, mWindow->GetWidth(), mWindow->GetHeight());
				glEnable(GL_SCISSOR_TEST);

				// Optionally clear the default framebuffer if needed
				glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);

				// // Render floating point color buffer to 2d quad and tone map hdr colors to default framebufffer's clamped color range
				// int slot = 0;
				// float exposure = 1.0f;
				// bool hdr = true;
				// auto hdrShader = ResourceManager::GetInstance().GetShader("AEON_HDR");
				// hdrShader->use();
				// glActiveTexture(GL_TEXTURE0 + slot);
				// glBindTexture(GL_TEXTURE_2D, textureID);
				// hdrShader->setInt("hdrBuffer", slot);
				// hdrShader->setInt("hdr", hdr);
				// hdrShader->setFloat("exposure", exposure);
				// 
				// renderQuad();
				// 
				// GLint currentFramebuffer = 0;
				// glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebuffer);

				//// Blit the framebuffer
				glBindFramebuffer(GL_READ_FRAMEBUFFER, FramebufferID);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
				glBlitFramebuffer(0, 0, mWindow->GetWidth(), mWindow->GetHeight(), 0, 0, mWindow->GetWidth(), mWindow->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);


			}

			if (mRuntimeState == RuntimeState::Start)
				Scripting::InvokeEventFunction("FrameEnd");
			gCoordinator.DeleteRequestedEntities();

			if (mLoadStatus == LoadStatus::Finish) {
				if (mSceneLoadThread.joinable())
					mSceneLoadThread.join();

				bool hasRead = false;
				void* params[1];
				params[0] = &hasRead;
				Scripting::InvokeEventFunction("SetHasSceneRead", params);
				
				SceneManager::SetActiveScene(SceneManager::GetInstance().GetLoadThreadScene());
				mLoadStatus = LoadStatus::NotRunning;
			}
		}
	}


	void Application::SubmitToLoadResourcesQueue(const std::function<void()>& func) {
		mThreadLoadResourceRequests.emplace(func);
	}

	void Application::Close()
	{
		mIsRunning = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& _event)
	{
		mIsRunning = false;
		return true;
	}

	void Application::Update() {}

	void Application::SubmitToMainThread(const std::function<void()>& func)
	{
		// so say you have multiple threads that call this function, only ONE of the invocation will run. The other invocations must wait in line for their turn.
		std::scoped_lock<std::mutex> lock(mMainThreadQueueMutex);

		mMainThreadQueue.emplace_back(func);
	}

	void Application::ExecuteMainThreadQueue()
	{
		ZoneScopedN("Execute Main Thread Queue Update");

		if (mLoadStatus == LoadStatus::LoadResources) {
			const float maxTime = 1.0f/60.0f;
			const auto frameStartTime = TimerStep::GetInstance().get_frame_start_time();

			auto timeTaken = TimerStep::calc_time_duration(frameStartTime, TimerStep::get_time_now());
			while (!mThreadLoadResourceRequests.empty() && timeTaken < maxTime) {
				auto& func = mThreadLoadResourceRequests.front();

				func();

				mThreadLoadResourceRequests.pop();
				timeTaken = TimerStep::calc_time_duration(frameStartTime, TimerStep::get_time_now());
			}

			int requestLeft = static_cast<int>(mThreadLoadResourceRequests.size());
			void* params = &requestLeft;
			Scripting::InvokeEventFunction("SetLoadResourceRequestLeft", &params);

			/*if (!mThreadLoadResourceRequests.empty()) {
				auto& func = mThreadLoadResourceRequests.front();

				func();

				mThreadLoadResourceRequests.pop();
				int requestLeft = static_cast<int>(mThreadLoadResourceRequests.size());
				void* params = &requestLeft;
				Scripting::InvokeEventFunction("SetLoadResourceRequestLeft", &params);
			}*/
		}

		// run all the functions in the queue
		for (auto& func : mMainThreadQueue)
			func();
		mMainThreadQueue.clear(); // clear functions, since they ran
	}
}
