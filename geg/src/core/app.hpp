#pragma once

#include "window/window.hpp"
#include "events/event.hpp"
#include <memory>

namespace Geg {
	class App {
	public:
		App();
		virtual ~App();

		[[noreturn]] void start();

	private:
		std::unique_ptr<Window> window;
	};

	//client must implement
	App* createApp();
}
