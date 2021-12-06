#include "sandbox.hpp"

// triangle lvl
class Triangle: public Geg::Layer {
private:
public:
	Triangle() {
		std::shared_ptr<Geg::Shader> shader(Geg::Shader::create(
				"/home/thegeeko/projects/geg/examples/triangle-vulkan/src/shaders/simple.vert.spv",
				"/home/thegeeko/projects/geg/examples/triangle-vulkan/src/shaders/simple.frag.spv"));

		Geg::RendererCommands::submit(shader);
	}

	void onEvent(Geg::Event& event) override {
		Geg::Dispatcher dis(event);
	}

	void onUpdate(float deltaTime) override {
		Geg::RendererCommands::drawIndexed();
	}
};

// main app
sandboxApp::sandboxApp() {
	Triangle* triangleLayer = new Triangle();
	pushLayer(triangleLayer);
};
sandboxApp::~sandboxApp(){};

// main fn
Geg::App* Geg::createApp() {
	Geg::App* a = new sandboxApp();
	return a;
}
