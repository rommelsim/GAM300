#pragma once
/*!***************************************************************************
	\file			Window.h
	\author			Chua Yip Xuan 
	\par			yipxuan.chua\@digipen.edu
	\brief			This header file consists of the window class and functions
					to manage the engine window.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/AeonAPI.h"
#include "Events/Event.h"

namespace AeonCore
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string &_title = "Aeon Core", unsigned int _width = 1200, unsigned int _height = 720) : Title(_title), Width(_width), Height(_height){}
	};

	class AEON_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event &)>;

		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void SetEventCallback(const EventCallbackFn &_callback) = 0;
		virtual void SetVSync(bool _enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void *GetNativeWindow() const = 0; //get from window pointer from window.h
		
		static Window *Create(const WindowProps &_props = WindowProps());
	};

}