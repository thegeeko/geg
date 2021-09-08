#include "event.hpp"

namespace Geg::EvMan {

	void pushEvent(Event &event) {
		queue.push_back(event);
	}

	void addLayer(int layer) {
		if (!listeners[layer]) {
			auto* newLayer = new std::vector<EVENT_LISTENER>{};
			listeners[layer] = newLayer;
		} else {
			GEG_CORE_WARN("this event already exist {}", layer);
		}
	}
	void addEventListener(EventType eventType, int layer, EVENT_CALLBACK &callback) {
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

	/* @TODO implement remove eventlistener function
	void removeEventListener(EventType eventType, int layer, EVENT_CALLBACK &callback) {
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
*/

	//@TODO a better event system
	//make it loop through listeners not event
	void cleanQueue() {
		//loop through events
		while (!queue.empty()) {
			auto currentEvent = queue.front();

			//loop through layers
			for (unsigned short i = 0; i < (sizeof(listeners) / sizeof(listeners[0])); i++) {
				auto layer = listeners[i];
				if (layer) {
					//loop through listeners
					for (const auto &currentListener : *layer) {
						if (currentListener.first == currentEvent.eventType) {
							currentListener.second ? currentListener.second(currentEvent) : GEG_CORE_ERROR(
											"null_ptr as callback function for the event {}", currentEvent.ToString());
						}
					}
					//listeners
				}
				if (currentEvent.isHandeled) { break; }
			}
			//layers
			queue.pop_front();
		}
		//events
	}

}