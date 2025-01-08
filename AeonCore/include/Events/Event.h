#pragma once
/*!***************************************************************************
	\file			Event.h
	\author			Chua Yip Xuan
	\par			yipxuan.chua\@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of function declarations and
					definitions to handle events.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include <functional>
#include <ostream>
#include <string>

//#define BIT(x) (1u << x)

namespace AeonCore
{
	enum class EventType
	{
		None = 0,
		WindowClose, 
		WindowMinimize, 
		WindowResize, 
		WindowFocus, 
		WindowLostFocus, 
		WindowMoved, 
		WindowTitleBarHitTest,
		AppTick, 
		AppUpdate, 
		AppRender,
		KeyPressed, 
		KeyReleased, 
		KeyTyped,
		MouseButtonPressed, 
		MouseButtonReleased, 
		MouseMoved, 
		MouseScrolled,
		ScenePreStart, 
		ScenePostStart, 
		ScenePreStop, 
		ScenePostStop,
		EditorExitPlayMode,
		SelectionChanged
	};

	//more for 
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication =	BIT(0),
		EventCategoryInput =		BIT(1),
		EventCategoryKeyboard =		BIT(2),
		EventCategoryMouse =		BIT(3),
		EventCategoryMouseButton =	BIT(4),
		EventCategoryScene =		BIT(5),
		EventCategoryEditor =		BIT(6)
	};

#define EVENT_CLASS_TYPE(_type) static EventType GetStaticType() { return EventType::##_type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #_type; }

#define EVENT_CLASS_CATEGORY(_category) virtual int GetCategoryFlags() const override { return _category; }

	class Event
	{
	public:
		virtual ~Event() {}
		virtual EventType GetEventType() const = 0;
		virtual const char *GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory _category)
		{
			return GetCategoryFlags() & _category;
		}

		bool mHandled = false; //is the _event handled?
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T &)>;
	public:
		EventDispatcher(Event &_event)
			: mEvent(_event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if ( mEvent.GetEventType() == T::GetStaticType() && !mEvent.mHandled )
			{
				mEvent.mHandled = func(*(T *)&mEvent);
				return true;
			}
			return false;
		}
	private:
		Event &mEvent;
	};

	inline std::ostream &operator<<(std::ostream &os, const Event &e)
	{
		return os << e.ToString();
	}
}

