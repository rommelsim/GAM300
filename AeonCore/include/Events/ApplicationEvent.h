#pragma once
/*!***************************************************************************
	\file			ApplicationEvent.h
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declarations and 
					definitions to handle Applications events.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Event.h"

#include <sstream>
#include "Core/AeonAPI.h"

namespace AeonCore
{
	AEON_API class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int _width, unsigned int _height)
			: mWidth(_width), mHeight(_height)
		{
		}

		inline unsigned int GetWidth() const { return mWidth; }
		inline unsigned int GetHeight() const { return mHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int mWidth, mHeight;
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent(bool minimized)
			: mMinimized(minimized)
		{
		}

		bool IsMinimized() const { return mMinimized; }

		EVENT_CLASS_TYPE(WindowMinimize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		bool mMinimized = false;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowTitleBarHitTestEvent : public Event
	{
	public:
		WindowTitleBarHitTestEvent(int _x, int _y, int &_hit)
			: mX(_x), mY(_y), mHit(_hit)
		{
		}

		inline int GetX() const { return mX; }
		inline int GetY() const { return mY; }
		inline void SetHit(bool _hit) { mHit = (int)_hit; }

		EVENT_CLASS_TYPE(WindowTitleBarHitTest)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int mX;
		int mY;
		int &mHit;
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}