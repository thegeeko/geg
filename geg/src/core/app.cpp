
#include <iostream>
#include "app.hpp"

namespace Geg {
	App* App::i_Ins = nullptr;

	App::App() {
		running = true;
		WindowProps props{};
		window = std::make_unique<Window>(props);
		window->setEventCallback(GEG_BIND_CB(App::onEvent));
		i_Ins = this;
	}
	App::~App() {}

	void App::onEvent(Event& e) {
		GEG_INFO("Event : {}", e.toString());
		Dispatcher dispatch(e);
		dispatch.dispatch<WindowCloseEvent>(GEG_BIND_CB(App::closeCallback));
		for(Layer* l : layerStack) {
			l->onEvent(e);
		}
	}

	void App::start() {
		while (running) {
			for(Layer* l : layerStack) {
				l->onUpdate();
			}
			window->onUpdate();
		}
	}

	void App::pushLayer(Layer* l) {
		layerStack.pushLayer(l);
		l->onAttach();
	}

	void App::pushOverlay(Layer* l) {
		layerStack.pushOverlay(l);
		l->onAttach();
	}

	bool App::closeCallback(const WindowCloseEvent &e) {
		running = false;
		return true;
	}

}
