#pragma once

#include "imgui/im-gui.hpp"
#include "layer-stack.hpp"
#include "window/window.hpp"

// the main class which every game required
// to drives from and it's a singleton

namespace geg {
	class App {
	public:
		App();
		virtual ~App();

		static inline App &get() { return *i_Ins; }
		inline Window &getWindow() { return *window; }

		void onEvent(Event &e);
		void start();

		void pushLayer(Layer *l);
		void pushOverlay(Layer *l);

		bool closeCallback(const WindowCloseEvent &e);

	private:
		static App *i_Ins;
		bool running = true;
		Scope<Window> window;
		Scope<ImGuiLayer> ui;
		LayerStack layerStack;
	};

	// client must implement this function
	// the engine will call it .. check main-entry.hpp
	App *createApp();

}		 // namespace geg
