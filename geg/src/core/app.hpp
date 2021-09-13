#pragma once

#include "window/window.hpp"
#include "layer-stack.hpp"
#include <memory>

#define GEG_BIND_CB(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Geg {
	class App {
	public:
		App();
		virtual ~App();

		void onEvent (Event& e);
		void start();

		bool closeCallback (const WindowCloseEvent& e);
	private:
		bool running = true;
		LayerStack layerStack{};
		std::unique_ptr<Window> window;
	};

	//client must implement
	App* createApp();
}
