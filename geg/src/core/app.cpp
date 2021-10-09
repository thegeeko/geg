#include "app.hpp"
#include "core/logger.hpp"
#include "renderer/renderer.hpp"
#include "renderer/vertex-array.hpp"
#include <memory>

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

  // --- trying renderer --- //
  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  unsigned int indices[] = {0, 1, 2};

  std::shared_ptr<VertexBuffer> vbo(
      VertexBuffer::create(&vertices[0], sizeof(vertices)));
  BufferLayout vbl = BufferLayout();
  vbl.add(ShaderDataType::Float, false);
  vbo->setLayout(vbl);

  std::shared_ptr<IndexBuffer> vio(
      IndexBuffer::create(&indices[0], sizeof(indices)));
  vao = std::shared_ptr<VertexArray>(VertexArray::create());

  vio->bind();

  vao->addVertexBuffer(vbo);
  vao->setIndexBuffer(vio);

  // --- trying renderer --- //
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
    Renderer::beginScene();
    RendererCommands::clear({0.34f, 0.9f, .61f, 1});
    Renderer::submit(vao);
    Renderer::endScene();
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

} // namespace Geg
