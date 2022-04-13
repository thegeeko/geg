#include "sandbox.hpp"
#include "imgui.h"


Level::Level() {
	vase = scene.createEntity("teapot");
	light = scene.createEntity("global light");

	meshPath = "assets/models/teapot.gltf";
	Geg::MeshAsset mesh = Geg::MeshLoader::loadModel(meshPath);
	vase.addComponent<Geg::MeshComponent>(mesh);
	vase.getComponent<Geg::TransformComponent>().translation = {0, 0, -3};

	shaderPath = "assets/shaders/dir-light.glsl";
	Geg::ShaderAsset shader = Geg::ShaderLoader::load(shaderPath);
	vase.addComponent<Geg::MeshRendererComponent>(shader);

	light.addComponent<Geg::GlobalLightComponent>(glm::vec4(1.f, -3.f, -1.f, 0.f));
}

void Level::drawComponents(Geg::Entity entity) {
	auto& name = entity.getComponent<Geg::NameComponent>().name;
	ImGui::Begin(name.c_str());

	// draw transform
	if (entity.hasComponent<Geg::TransformComponent>()) {
		auto& trans = entity.getComponent<Geg::TransformComponent>();
		ImGui::DragFloat3("Translation", &trans.translation[0]);
		ImGui::DragFloat3("Scale", &trans.scale[0]);
		ImGui::DragFloat3("Rotation", &trans.rotation[0]);

	}

	ImGui::Separator();

	// draw mesh
	ImGui::InputText("mesh path", meshPath.data(), 256);
	if (entity.hasComponent<Geg::MeshComponent>()) {
		if (ImGui::Button("load new mesh")) {
			const auto mesh = Geg::MeshLoader::loadModel(meshPath);
			entity.getComponent<Geg::MeshComponent>().mesh = mesh;
		}
	} else {
		if (ImGui::Button("Add mesh component")) {
			entity.addComponent<Geg::MeshComponent>(meshPath);
		}
	}

	// draw renderer

	ImGui::Separator();

	ImGui::InputText("shader path", shaderPath.data(), 256);
	if (entity.hasComponent<Geg::MeshRendererComponent>()) {

		if (ImGui::Button("reload")) {
			const Geg::ShaderAsset shader = Geg::ShaderLoader::load(shaderPath);
			entity.getComponent<Geg::MeshRendererComponent>().shader = shader;
		}
		if (ImGui::Button("load new shader")) {
			const Geg::ShaderAsset shader = Geg::ShaderLoader::load(shaderPath);
			entity.getComponent<Geg::MeshRendererComponent>().shader = shader;
		}
	} else {
		if (ImGui::Button("Add mesh renderer component")) {
			entity.addComponent<Geg::MeshRendererComponent>(shaderPath);
		}
	}

	ImGui::Separator();

	if (entity.hasComponent<Geg::GlobalLightComponent>()) {
		auto& light = entity.getComponent<Geg::GlobalLightComponent>();
		ImGui::DragFloat3("light direction", &light.dir.x);
		ImGui::ColorEdit3("light direction", &light.color.r);
	}

	ImGui::End();
}

void Level::onEvent(Geg::Event& event) {
	Geg::Dispatcher dis(event);
}

void Level::onUpdate(float deltaTime) {
	scene.onUpdate(deltaTime, camController.getCam());
}

void Level::onUiUpdate(float deltaTime) {
	camController.handleUpdates();

	const auto view = scene.getReg().view<Geg::NameComponent>();
	for (const auto e : view) {
		Geg::Entity ent{&scene, e};
		const auto& name = ent.getComponent<Geg::NameComponent>().name;
		const auto id = static_cast<uint32_t>(e);
		const bool isActive = id == activeId;

		ImGuiTreeNodeFlags flags =
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_NoTreePushOnOpen | (
					isActive ?
						ImGuiTreeNodeFlags_Selected :
						0);

		ImGui::TreeNodeEx((void*)(uint64_t)id, flags, name.c_str());
		if (ImGui::IsItemClicked()) {
			activeId = id;
		}

		if (isActive) {
			drawComponents(ent);
		}
	}

}

// main app
sandboxApp::sandboxApp() {
	// layers should be heap allocated .. the app will clean them up
	lvl = new Level();
	pushLayer(lvl);
}

sandboxApp::~sandboxApp() {}

// main fn
Geg::App* Geg::createApp() {
	App* a = new sandboxApp();
	return a;
}
