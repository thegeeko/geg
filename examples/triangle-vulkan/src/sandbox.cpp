#include "sandbox.hpp"
#include <cstdint>

#include "renderer/buffers.hpp"

// triangle lvl
Triangle::Triangle() {
	shader = Geg::Ref<Geg::Shader>(
			Geg::Shader::create(
					"/home/thegeeko/projects/geg/examples/triangle-vulkan/src/shaders/simple.vert.spv",
					"/home/thegeeko/projects/geg/examples/triangle-vulkan/src/shaders/simple.frag.spv"));

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
	
	pipeline = Geg::Ref<Geg::Pipeline>(Geg::Pipeline::create(vbo, ibo, shader));
}

void Triangle::onEvent(Geg::Event& event) {
	Geg::Dispatcher dis(event);
}

void Triangle::onUpdate(float deltaTime) {
	Geg::Renderer::beginScene();
	Geg::Renderer::submit(pipeline);
	Geg::Renderer::endScene();
}

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
