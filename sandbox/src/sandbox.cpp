#include "sandbox.hpp"

#include "imgui.h"

Level::Level() {
	light = scene.createEntity("light 1");
	light2 = scene.createEntity("light 2");

	floor = scene.createEntity("floor");
	vase = scene.createEntity("teapot");

	meshPath = "assets/models/teapot.gltf";
	shaderPath = "assets/shaders/dir-light.glsl";

	vase.addComponent<Geg::MeshComponent>(meshPath);
	vase.addComponent<Geg::MeshRendererComponent>(shaderPath);

	auto& vaseTransform = vase.getComponent<Geg::TransformComponent>();
	vaseTransform.translation = {0, 0, -25};
	vaseTransform.rotation = {0, 0, 180};

	meshPath = "assets/models/cube.obj";

	floor.addComponent<Geg::MeshComponent>(meshPath);
	floor.addComponent<Geg::MeshRendererComponent>(shaderPath);
	auto& floorTransfrom = floor.getComponent<Geg::TransformComponent>();
	floorTransfrom.translation = {0, 6, -30};
	floorTransfrom.scale = {30, 1, 30};

	light.addComponent<Geg::MeshComponent>(meshPath);
	light.addComponent<Geg::MeshRendererComponent>(shaderPath);
	light.addComponent<Geg::GlobalLightComponent>(glm::vec4(1.f, -3.f, -1.f, 0.f));
	light.addComponent<Geg::PointLightComponent>(glm::vec4(1.f, 0.f, 0.f, 0.f));
	light.getComponent<Geg::TransformComponent>().scale = {0.2, 0.2, 0.2};

	light2.addComponent<Geg::MeshComponent>(meshPath);
	light2.addComponent<Geg::MeshRendererComponent>(shaderPath);
	light2.addComponent<Geg::PointLightComponent>(glm::vec4(1.f, 0.f, 0.f, 0.f));
	light2.getComponent<Geg::TransformComponent>().scale = {0.2, 0.2, 0.2};
}

void Level::drawComponents(Geg::Entity entity) {
	auto& name = entity.getComponent<Geg::NameComponent>().name;
	ImGui::Begin(name.c_str());

	// draw transform
	if (entity.hasComponent<Geg::TransformComponent>()) {
		ImGui::Text("Transform Componenet :");

		auto& trans = entity.getComponent<Geg::TransformComponent>();
		ImGui::DragFloat3("Translation", &trans.translation[0]);
		ImGui::DragFloat3("Scale", &trans.scale[0]);
		ImGui::DragFloat3("Rotation", &trans.rotation[0]);
	}

	ImGui::Separator();
	ImGui::Text("Mesh Componenet :");

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
	ImGui::Text("Mesh Renderer Componenet :");

	ImGui::InputText("shader path", shaderPath.data(), 256);
	if (entity.hasComponent<Geg::MeshRendererComponent>()) {
	auto& meshRenderer = entity.getComponent<Geg::MeshRendererComponent>();
		if (ImGui::Button("reload")) {
			const Geg::ShaderAsset shader = Geg::ShaderLoader::load(shaderPath);
			meshRenderer.shader = shader;
		}
		ImGui::ColorEdit3("color", &meshRenderer.color[0]);
		ImGui::DragFloat("sharpness", &meshRenderer.color[3]);
	} else {
		if (ImGui::Button("Add mesh renderer component")) {
			entity.addComponent<Geg::MeshRendererComponent>(shaderPath);
		}
	}

	ImGui::Separator();
	ImGui::Text("Global Light Componenet :");

	if (entity.hasComponent<Geg::GlobalLightComponent>()) {
		auto& light = entity.getComponent<Geg::GlobalLightComponent>();
		ImGui::DragFloat3("light direction", &light.dir.x);

		ImGui::ColorEdit3("light color", &light.color.r);
		ImGui::SliderFloat("light intensity", &light.color.w, 0, 1);

		ImGui::ColorEdit3("ambient color", &light.ambient.r);
		ImGui::SliderFloat("ambient intensity", &light.ambient.w, 0, 1);
	}

	ImGui::Separator();
	ImGui::Text("Point Light Componenet :");

	if (entity.hasComponent<Geg::PointLightComponent>()) {
		auto& light = entity.getComponent<Geg::PointLightComponent>();

		ImGui::ColorEdit3("point light color", &light.color.r);
		ImGui::SliderFloat("point light intensity", &light.color.w, 0, 10);
	}

	ImGui::End();
}

void Level::onEvent(Geg::Event& event) {
	Geg::Dispatcher dis(event);
}

void Level::onUpdate(float deltaTime) {
	camController.update(deltaTime);
	scene.onUpdate(deltaTime, camController.getCam());
}

void Level::onUiUpdate(float deltaTime) {
	camController.drawUi();

	const auto view = scene.getReg().view<Geg::NameComponent>();
	for (const auto e : view) {
		Geg::Entity ent{&scene, e};
		const auto& name = ent.getComponent<Geg::NameComponent>().name;
		const auto id = static_cast<uint32_t>(e);
		const bool isActive = id == activeId;

		ImGuiTreeNodeFlags flags =
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_NoTreePushOnOpen | (isActive ? ImGuiTreeNodeFlags_Selected : 0);

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
