#ifndef GEG_EVENT_TYPES_HPP
#define GEG_EVENT_TYPES_HPP

#include "event.hpp"

namespace Geg {
	class WindowCloseEvent : public Event{
	public:
		const static EventType staticEventType = EventType::WindowClose;
		EventType eventType = EventType::WindowClose;
		std::string name = "Window close event";
		EventCategory categoryFlags = EventCategoryApplication;
		std::string toString() const override {return name;}
		EventType getEventType () const override{return eventType;}
	};
}
#endif //GEG_EVENT_TYPES_HPP
