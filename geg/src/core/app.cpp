#include "app.hpp"

namespace Geg {
	App *App::i_Ins = nullptr;

	App::App() {
		running = true;

		// setup window
		WindowProps props{};
		window = std::make_unique<Window>(props);
		window->setEventCallback(GEG_BIND_CB(App::onEvent));

		i_Ins = this;

		// setup imgui
		pushOverlay(new ImGuiLayer{});
	}
	App::~App() {}

	void App::onEvent(Event &e) {
		// GEG_INFO("Event : {}", e.toString());
		Dispatcher dispatch(e);
		dispatch.dispatch<WindowCloseEvent>(GEG_BIND_CB(App::closeCallback));
		for (Layer *l : layerStack) {
			l->onEvent(e);
		}
	}

	void App::start() {
		while (running) {
			for (Layer *l : layerStack) {
				l->onUpdate();
				imgui->begin();
				l->onUiUpdate();
				imgui->end();
			}
			window->onUpdate();
		}
	}

	void App::pushLayer(Layer *layer) {
		layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void App::pushOverlay(Layer *layer) {
		layerStack.pushOverlay(layer);
		layer->onAttach();
	}

	bool App::closeCallback(const WindowCloseEvent &e) {
		running = false;
		return true;
	}

}		 // namespace Geg
