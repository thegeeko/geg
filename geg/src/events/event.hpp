#ifndef GEG_EVENT_HPP
#define GEG_EVENT_HPP

#define EVENT_CALLBACK std::function<void(Event&)>

#include "src/core/logger.hpp"

#include <string>
#include <queue>
#include <functional>

namespace Geg {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication    = 1 << 1, // 00001
		EventCategoryInput          = 1 << 2, // 00010
		EventCategoryKeyboard       =	1 << 3, // 00100
		EventCategoryMouse          = 1 << 4, // 01000
		EventCategoryMouseButton    = 1 << 5  // 10000
	};

	class Event {
	public:
		Event();

		EventType eventType;
		const char* name;
		int categoryFlags = -1;
		bool isHandeled = false;

		virtual std::string ToString() const { return name; }
		inline bool IsInCategory(EventCategory category)
		{
			return categoryFlags & category;
		}
	};

	 class EventQueue {
	 private:
		 std::deque<Event> queue;

	 friend class EventDispatcher;
	 friend class EventLoop;

	 public:
	 	EventQueue();
	 };

	class EventDispatcher {
	public:
		EventDispatcher(EventQueue& _eventQueue);
		void pushEvent(Event& event);

	protected:
		EventQueue eventQueue;
	};

	class EventLoop {
	public:
		EventLoop(EventQueue& _eventQueue);
		void addLayer(int layer);
		void addEventListener(EventType event, int layer, EVENT_CALLBACK& callback);
		void invokeListeners();

	private:
		//void pullEvent();

	protected:
		std::vector<std::pair<EventType, EVENT_CALLBACK>>* listeners[4] = {nullptr, nullptr, nullptr, nullptr};
		EventQueue eventQueue;
	};

}

#endif //GEG_EVENT_HPP
