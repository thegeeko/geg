
#include <iostream>
#include "app.hpp"

namespace Geg {
	App::App() {
		running = true;
		window = std::make_unique<Window>(WindowProps());
		window->setEventCallback(GEG_BIND_CB(App::onEvent));
	}
	App::~App() {}

	void App::onEvent(Event& e) {
		GEG_INFO("Event : {}", e.toString());
		Dispatcher dispatch(e);
		dispatch.dispatch<WindowCloseEvent>(GEG_BIND_CB(App::closeCallback));
	}

	void App::start() {
		while (running) {
			window->onUpdate();
		}
	}

	bool App::closeCallback(const WindowCloseEvent &e) {
		running = false;
		return true;
	}

}
