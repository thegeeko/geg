#pragma once

namespace Geg {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTapped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = 1 << 1, // 00001
		EventCategoryInput       = 1 << 2, // 00010
		EventCategoryKeyboard    = 1 << 3, // 00100
		EventCategoryMouse       = 1 << 4, // 01000
		EventCategoryMouseButton = 1 << 5  // 10000
	};

	class Event {
	public:
		Event()= default;

		EventType eventType = EventType::None;
		std::string name = "event";
		EventCategory categoryFlags = EventCategory::None;
		bool isHandeled = false;

		virtual EventType getEventType () const = 0;
		virtual std::string toString() const { return name; }

		inline bool IsInCategory(EventCategory category) {
			return categoryFlags & category;
		}
	};

	class Dispatcher {
	private:
		Event& e;
	public:
		Dispatcher(Event& _e) : e(_e) {};
		template<typename T, typename F>
		bool dispatch(const F& cb) {
			if (e.getEventType() == T::staticEventType){ e.isHandeled = cb(static_cast<T&>(e)); }
		}
	};
}
