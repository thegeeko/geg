#include "app.hpp"

#include "renderer/renderer.hpp"
#include "time/time.hpp"

namespace Geg {
	App *App::i_Ins = nullptr;

	App::App() {
		running = true;

		// Setup window
		WindowProps props{};
		window = std::make_unique<Window>(props);
		window->setEventCallback(GEG_BIND_CB(App::onEvent));

		i_Ins = this;		 // Singleton

		// Init rendering api
		// this should be owned by the graphics context
		// will fix
		Renderer::initAPI();

		// Setup imgui
		ui = std::make_unique<ImGuiLayer>();
	}

	App::~App() {
		Renderer::deInitAPI();
	}

	void App::onEvent(Event &e) {
		// GEG_INFO("Event : {}", e.toString());

		Dispatcher::dispatch<WindowCloseEvent>(e, GEG_BIND_CB(App::closeCallback));
		Dispatcher::dispatch<WindowResizeEvent>(e, [&](const WindowResizeEvent& e) {
			window->getGraphicsContext()->windowResized(e.getW(), e.getH());
			return true;
		});

		for (Layer *l : layerStack) {
			l->onEvent(e);
		}
	}

	void App::start() {
		while (running) {
			Time::updateTime();

			for (Layer *l : layerStack) {
				ui->initFrame();
				l->onUiUpdate(Time::getDeltaTime());
				ui->render();

				l->onUpdate(Time::getDeltaTime());
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
