#include "sandbox.hpp"

#include "backends/opengl/shader.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "renderer/cams/ortho-cam.hpp"

// triangle lvl
class Triangle: public Geg::Layer {
 private:
	std::shared_ptr<Geg::VertexArray> vao;
	std::shared_ptr<Geg::GLShader> simpleShader;
	std::shared_ptr<Geg::OrthoCam> cam;
	glm::vec3 camPos{0.5f, 0.5f, 0.0f};

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

		// casting the shader so we can accses the uniforms opengl spasifc functions
		Geg::Shader* toBeCasted = Geg::Shader::create(
				"/home/thegeeko/projects/geg/examples/triangle/src/shaders/simple.vert",
				"/home/thegeeko/projects/geg/examples/triangle/src/shaders/simple.frag");

		Geg::GLShader* casted = dynamic_cast<Geg::GLShader*>(toBeCasted);

		// shader setup
		simpleShader = std::shared_ptr<Geg::GLShader>(casted);

		// creating a cam
		cam = std::shared_ptr<Geg::OrthoCam>(new Geg::OrthoCam(1.f, -1.f, 1.f, -1.f));
	}

	void onEvent(Geg::Event& event) override {
		Geg::Dispatcher dis(event);
	}

	void onUiUpdate() override {
		ImGui::SliderFloat3("CamPos", &camPos.x, 0, 5);
	}

	void onUpdate(float deltaTime) override {
		Geg::Renderer::beginScene();

		Geg::RendererCommands::clear({0.34f, 0.9f, .61f, 1});
		cam->setPosition(camPos);
		simpleShader->uploadUniformMat4f("cam", glm::value_ptr(cam->getProjVeiwM()));
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
