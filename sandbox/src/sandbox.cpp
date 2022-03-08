#include "sandbox.hpp"

#include <memory>

#include "assets/loaders/loader.hpp"
#include "imgui.h"

// triangle lvl
Level::Level() {
	shader = Geg::Ref<Geg::Shader>(
			Geg::Shader::create(
					"C:/projects/g/sandbox/shaders/simple.vert.spv",
					"C:/projects/g/sandbox/shaders/simple.frag.spv"));

	mesh = Geg::Loader::loadModel("C:/projects/g/sandbox/models/cube.obj");
	pipeline = Geg::Ref<Geg::Pipeline>(Geg::Pipeline::create(mesh.vbo, mesh.ibo, shader));

}

void Level::onEvent(Geg::Event& event) {
	Geg::Dispatcher dis(event);
}

void Level::onUpdate(float deltaTime) {
	Geg::Renderer::beginScene(camController.getCam());
	Geg::Renderer::submit(pipeline, true);
	Geg::Renderer::endScene();
}

void Level::onUiUpdate(float deltaTime) {
	camController.handleUpdates();
}

// main app
sandboxApp::sandboxApp() {
	lvl= new Level();
	pushLayer(lvl);
};
sandboxApp::~sandboxApp(){};

// main fn
Geg::App* Geg::createApp() {
	Geg::App* a = new sandboxApp();
	return a;
}
