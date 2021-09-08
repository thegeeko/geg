
#include "app.hpp"

namespace Geg {
	void callback (Event& e) {
		GEG_CORE_INFO("hello {}", e.ToString());
	}
	App::App() {

		std::function<void(Event&)> a = callback;
		EvMan::addLayer(0);
		EvMan::addEventListener(EventType::AppTick, 0, a);

		window = std::make_unique<Window>(WindowProps());

	}
	App::~App() {}

	[[noreturn]]
	void App::start() {
		while (true) {
			EvMan::cleanQueue();
			window->onUpdate();
		}
	}
}
