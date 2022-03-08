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

	Geg::BufferLayout layout{};
	layout.add(Geg::ShaderDataType::Float3, false);
	layout.add(Geg::ShaderDataType::Float3, false);

	// clang-format off

	float vertices[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
			0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f
	};

	uint32_t indecies[] = { 0, 1, 2, 2, 3, 0 };

	//clang-format on

	vbo = Geg::Ref<Geg::VertexBuffer>(Geg::VertexBuffer::create(&vertices[0], layout.getStride() * 4));
	vbo->setLayout(layout);

	ibo = Geg::Ref<Geg::IndexBuffer>(Geg::IndexBuffer::create(&indecies[0], 6 * sizeof(uint32_t)));

	mesh = Geg::Loader::loadModel("C:/projects/g/sandbox/models/cube.obj");

	pipeline = Geg::Ref<Geg::Pipeline>(Geg::Pipeline::create(mesh.vbo, mesh.ibo, shader));
	pipeline1 = Geg::Ref<Geg::Pipeline>(Geg::Pipeline::create(vbo, ibo, shader));

}

void Level::onEvent(Geg::Event& event) {
	Geg::Dispatcher dis(event);
}

void Level::onUpdate(float deltaTime) {
	Geg::Renderer::beginScene(camController.getCam());
	Geg::Renderer::submit(pipeline, false);
	// Geg::Renderer::submit(pipeline1, true);
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