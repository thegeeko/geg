#include "event.hpp"

namespace Geg {
	// event queue ------------------------------------------------ //
	EventQueue::EventQueue() {
		std::deque<Event *> queue;
	}

	// event dispatcher ------------------------------------------- //
	EventDispatcher::EventDispatcher(EventQueue &_eventQueue) : eventQueue(_eventQueue) {}

	void EventDispatcher::pushEvent(Event &event) {
		eventQueue.queue.push_back(event);
	}

	// event loop ------------------------------------------------ //
	EventLoop::EventLoop(EventQueue &_eventQueue) : eventQueue(_eventQueue) {}

	void EventLoop::addLayer(int layer) {
		if (!listeners[layer]) {
			std::vector<std::pair<EventType, EVENT_CALLBACK>> newLayer;
			listeners[layer] = &newLayer;
		} else {
			GEG_CORE_WARN("this event already exist {}", layer);
		}
	}

	void EventLoop::addEventListener(EventType eventType, int layer, EVENT_CALLBACK &callback) {
		if (listeners[layer]) {
			if (callback) {
				std::pair<EventType, EVENT_CALLBACK > listener{eventType, callback};
				listeners[layer]->push_back(listener);
			} else {
				GEG_CORE_ERROR("invalid callback function");
			}
		} else {
			GEG_CORE_ERROR("layer {} doesn't exist", layer);
		}
	}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-too-small-loop-variable"

	void EventLoop::invokeListeners() {
		//loop through events
		while (!eventQueue.queue.empty()) {
			auto currentEvent = eventQueue.queue.front();

			//loop through layers
			for (unsigned short i = 0; i < (sizeof(listeners) / sizeof(listeners[0])); i++) {
				auto layer = listeners[i];
				if (layer) {
					//loop through listeners
					for (const auto &currentListener : *layer) {
						if (currentListener.first == currentEvent.eventType) {
							currentListener.second(currentEvent);
						}
					}
					//listeners
				}
				if (currentEvent.isHandeled) { break; }
			}
			//layers

			eventQueue.queue.pop_front();
		}
		//events
	}

#pragma clang diagnostic pop


}