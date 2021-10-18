#include "sandbox.hpp"

#include <memory>
#include "core/logger.hpp"

// triangle lvl
class Triangle: public Geg::Layer {
 private:
	std::shared_ptr<Geg::VertexArray> vao;
	std::shared_ptr<Geg::Shader> simpleShader;

 public:
	Triangle() {
		float vertices[] = {
				-0.5f,
				-0.5f,
				0.0f,
				//
				0.5f,
				-0.5f,
				0.0f,
				//
				0.0f,
				0.5f,
				0.0f};

		unsigned int indices[] = {0, 1, 2};
		// creating vbo, vio, buffer layout and a vao
		std::shared_ptr<Geg::VertexBuffer> vbo(
				Geg::VertexBuffer::create(&vertices[0], 9 * sizeof(float)));

		std::shared_ptr<Geg::IndexBuffer> vio(
				Geg::IndexBuffer::create(&indices[0], 3));

		Geg::BufferLayout vbl = Geg::BufferLayout();
		vao = std::shared_ptr<Geg::VertexArray>(Geg::VertexArray::create());

		// adding 3 floats as the layout of the vbo
		vbl.add(Geg::ShaderDataType::Float3, false);		// position
		vbo->setLayout(vbl);

		vao->addVertexBuffer(vbo);
		vao->setIndexBuffer(vio);

		// shader setup
		simpleShader = std::shared_ptr<Geg::Shader>(
				Geg::Shader::create(
						"/home/thegeeko/projects/geg/examples/triangle/src/shaders/simple.vert",
						"/home/thegeeko/projects/geg/examples/triangle/src/shaders/simple.frag"));
	}

	void onUpdate() override {
		Geg::Renderer::beginScene();

		Geg::RendererCommands::clear({0.34f, 0.9f, .61f, 1});
		simpleShader->bind();
		Geg::Renderer::submit(vao);

		Geg::Renderer::endScene();
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
