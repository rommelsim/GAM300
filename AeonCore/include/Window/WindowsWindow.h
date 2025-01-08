#pragma once
/*!***************************************************************************
	\file			WindowsWindow.h
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\brief			This header file consists of the window class and functions
					to manage the windows window.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Window.h"
#define GLFW3_DLL
#include <GLFW/glfw3.h>

namespace AeonCore
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps &_props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return mData.Width; }
		inline unsigned int GetHeight() const override { return mData.Height; }

		inline void SetEventCallback(const EventCallbackFn &_callback) override { mData.EventCallback = _callback; }
		void SetVSync(bool _enabled) override;
		bool IsVSync() const override;

		inline virtual void *GetNativeWindow() const { return mWindow; }

	private:
		virtual void Init(const WindowProps &_props);
		virtual void ShutDown();

		GLFWwindow* mWindow;
		

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool Vsync;
			EventCallbackFn EventCallback;
		};

		WindowData mData;
	};
}