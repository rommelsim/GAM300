#include "acpch.h"
/*!***************************************************************************
	\file			WindowWindows.cpp
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\brief			This source file consists of the window class and functions
					definitions to manage the engine window.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Window/WindowsWindow.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseKeyEvent.h"
#include "Events/KeyEvent.h"
#include "Core/Log.h"

#include <glad/glad.h>

#include "Tracy.hpp"

namespace AeonCore
{
	static bool IsGLFWInitialised = false;

	static void GLFWErrorCallback(int _error, const char* _desc)
	{
		AeonCore_ERROR("GLFW Error {0}: {1}", _error, _desc);
	}

	Window* Window::Create(const WindowProps& _props)
	{
		return new WindowsWindow(_props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& _props)
	{
		Init(_props);
	}
	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}
	void WindowsWindow::Init(const WindowProps& _props)
	{
		mData.Title = _props.Title;
		mData.Width = _props.Width;
		mData.Height = _props.Height;

		AeonCore_INFO("Creating window {0} ({1}, {2})", _props.Title, _props.Width, _props.Height);

		if (!IsGLFWInitialised)
		{
			int status = glfwInit();
			AeonCore_ASSERT(status, "Failed to initialise GLFW...");
			glfwSetErrorCallback(GLFWErrorCallback);
			IsGLFWInitialised = true;
		}

		mWindow = glfwCreateWindow((int)_props.Width, (int)_props.Height, mData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(mWindow);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AeonCore_ASSERT(status, "Failed to initialise GLAD...");

		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(true);

		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* _window, int _width, int _height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				//// Calculate the new width and height to maintain aspect ratio
				//float aspectRatio = static_cast<float>(data.Width) / static_cast<float>(data.Height);
				//int newWidth = static_cast<int>(aspectRatio * _height);

				data.Height = _height;
				//data.Width = newWidth;
				data.Width = _width;

				// Set the new window size
				//glfwSetWindowSize(_window, newWidth, _height);

				WindowResizeEvent event(_width, _height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* _window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});

		glfwSetKeyCallback(mWindow, [](GLFWwindow* _window, int _key, int _scanCode, int _action, int _mods) {
			(void)_mods;
			(void)_scanCode;
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

			switch (_action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(_key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(_key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(_key, 1);
				data.EventCallback(event);
				break;
			}
			}
			});


		glfwSetCharCallback(mWindow, [](GLFWwindow* _window, unsigned int _char)
			{

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				KeyTypedEvent event(_char);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* _window, int _button, int _action, int _mods)
			{
				(void)_mods;
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				switch (_action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(_button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(_button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(mWindow, [](GLFWwindow* _window, double _xOffset, double _yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				MouseScrolledEvent event((float)_xOffset, (float)_yOffset);
				data.EventCallback(event);
			}
		);

		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* _window, double _xPos, double _yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				MouseMovedEvent event((float)_xPos, (float)_yPos);
				data.EventCallback(event);
			});

	}
	void WindowsWindow::OnUpdate()
	{
		ZoneScopedN("Window Update");
		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}
	void WindowsWindow::SetVSync(bool _enabled)
	{
		if (_enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		mData.Vsync = _enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return mData.Vsync;
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
}