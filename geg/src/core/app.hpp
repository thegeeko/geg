#pragma once

#include "imgui/im-gui.hpp"
#include "layer-stack.hpp"
#include "renderer/vertex-array.hpp"
#include "window/window.hpp"
#include <memory>

namespace Geg {
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
  std::unique_ptr<Window> window;
  ImGuiLayer *imgui;
  LayerStack layerStack;
	std::shared_ptr<VertexArray> vao;
};

// client must implement
App *createApp();
} // namespace Geg
