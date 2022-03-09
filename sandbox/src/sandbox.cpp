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

	mesh = Geg::Ref<Geg::Mesh>(Geg::Loader::loadModel("C:/projects/g/sandbox/models/smooth_vase.obj"));
	mesh1 = Geg::Ref<Geg::Mesh>(Geg::Loader::loadModel("C:/projects/g/sandbox/models/smooth_vase.obj"));
	pipeline = Geg::Ref<Geg::Pipeline>(Geg::Pipeline::create(mesh->vbo, mesh->ibo, shader));
	pipeline1 = Geg::Ref<Geg::Pipeline>(Geg::Pipeline::create(mesh->vbo, mesh->ibo, shader));

	mesh->setScale({2.f, 2.f, 2.f});
	mesh->setPosition({0.5, 0.5, 0});
}

void Level::onEvent(Geg::Event& event) {
	Geg::Dispatcher dis(event);
}

void Level::onUpdate(float deltaTime) {
	Geg::Renderer::beginScene(camController.getCam());
	Geg::Renderer::submit(pipeline, mesh->getModelMatrix());
	Geg::Renderer::submit(pipeline1, mesh1->getModelMatrix());
	Geg::Renderer::endScene();
}

void Level::onUiUpdate(float deltaTime) {
	camController.handleUpdates();

	ImGui::Begin("objects");
	{
		if (ImGui::InputFloat3("Pos", &meshPos[0])) {
			mesh->setPosition(meshPos);
		}
		if (ImGui::InputFloat3("scale", &meshScale[0])) {
			mesh->setScale(meshScale);
		}
	}
	ImGui::End();
}

// main app
sandboxApp::sandboxApp() {
	lvl = new Level();
	pushLayer(lvl);
};
sandboxApp::~sandboxApp() {};

// main fn
Geg::App* Geg::createApp() {
	Geg::App* a = new sandboxApp();
	return a;
}
