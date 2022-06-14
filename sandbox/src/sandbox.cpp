#include "sandbox.hpp"

#include "imgui.h"

Level::Level() {
	light = scene.createEntity("light 1");
	light2 = scene.createEntity("light 2");

	floor = scene.createEntity("floor");
	vase = scene.createEntity("teapot");

	meshPath = "assets/models/teapot.gltf";
	shaderPath = "assets/shaders/dir-light.glsl";
	std::string texPath = "assets/textures/texture.jpg";

	vase.addComponent<geg::MeshComponent>(meshPath);
	vase.addComponent<geg::MeshRendererComponent>(shaderPath);

	auto& vaseTransform = vase.getComponent<geg::TransformComponent>();
	vaseTransform.translation = {0, 0, -25};
	vaseTransform.rotation = {0, 0, 180};

	meshPath = "assets/models/cube.obj";

	floor.addComponent<geg::MeshComponent>(meshPath);
	floor.addComponent<geg::MeshRendererComponent>(shaderPath);
	auto& floorTransfrom = floor.getComponent<geg::TransformComponent>();
	floorTransfrom.translation = {0, 6, -30};
	floorTransfrom.scale = {30, 1, 30};

	light.addComponent<geg::MeshComponent>(meshPath);
	light.addComponent<geg::MeshRendererComponent>(shaderPath);
	light.getComponent<geg::MeshRendererComponent>().tex = geg::Ref<geg::Texture>(geg::Texture::create(texPath));
	light.addComponent<geg::GlobalLightComponent>(glm::vec4(1.f, -3.f, -1.f, 0.f));
	light.addComponent<geg::PointLightComponent>(glm::vec4(1.f, 0.f, 0.f, 0.f));
	// light.getComponent<geg::TransformComponent>().scale = {0.2, 0.2, 0.2};

	light2.addComponent<geg::MeshComponent>(meshPath);
	light2.addComponent<geg::MeshRendererComponent>(shaderPath);
	light2.addComponent<geg::PointLightComponent>(glm::vec4(1.f, 0.f, 0.f, 0.f));
	light2.getComponent<geg::TransformComponent>().scale = {0.2, 0.2, 0.2};
}

void Level::drawComponents(geg::Entity entity) {
	auto& name = entity.getComponent<geg::NameComponent>().name;
	ImGui::Begin(name.c_str());

	// draw transform
	if (entity.hasComponent<geg::TransformComponent>()) {
		ImGui::Text("Transform Componenet :");

		auto& trans = entity.getComponent<geg::TransformComponent>();
		ImGui::DragFloat3("Translation", &trans.translation[0]);
		ImGui::DragFloat3("Scale", &trans.scale[0]);
		ImGui::DragFloat3("Rotation", &trans.rotation[0]);
	}

	ImGui::Separator();
	ImGui::Text("Mesh Componenet :");

	// draw mesh
	ImGui::InputText("mesh path", meshPath.data(), 256);

	if (entity.hasComponent<geg::MeshComponent>()) {
		if (ImGui::Button("load new mesh")) {
			const auto mesh = geg::MeshLoader::loadModel(meshPath);
			entity.getComponent<geg::MeshComponent>().mesh = mesh;
		}
	} else {
		if (ImGui::Button("Add mesh component")) {
			entity.addComponent<geg::MeshComponent>(meshPath);
		}
	}

	// draw renderer

	ImGui::Separator();
	ImGui::Text("Mesh Renderer Componenet :");

	ImGui::InputText("shader path", shaderPath.data(), 256);
	if (entity.hasComponent<geg::MeshRendererComponent>()) {
	auto& meshRenderer = entity.getComponent<geg::MeshRendererComponent>();
		if (ImGui::Button("reload")) {
			const geg::ShaderAsset shader = geg::ShaderLoader::load(shaderPath);
			meshRenderer.shader = shader;
		}
		ImGui::ColorEdit3("color", &meshRenderer.color[0]);
		ImGui::DragFloat("sharpness", &meshRenderer.color[3]);
		ImGui::Checkbox("use textures", &meshRenderer.useTex);
	} else {
		if (ImGui::Button("Add mesh renderer component")) {
			entity.addComponent<geg::MeshRendererComponent>(shaderPath);
		}
	}

	ImGui::Separator();
	ImGui::Text("Global Light Componenet :");

	if (entity.hasComponent<geg::GlobalLightComponent>()) {
		auto& light = entity.getComponent<geg::GlobalLightComponent>();
		ImGui::DragFloat3("light direction", &light.dir.x);

		ImGui::ColorEdit3("light color", &light.color.r);
		ImGui::SliderFloat("light intensity", &light.color.w, 0, 1);

		ImGui::ColorEdit3("ambient color", &light.ambient.r);
		ImGui::SliderFloat("ambient intensity", &light.ambient.w, 0, 1);
	}

	ImGui::Separator();
	ImGui::Text("Point Light Componenet :");

	if (entity.hasComponent<geg::PointLightComponent>()) {
		auto& light = entity.getComponent<geg::PointLightComponent>();

		ImGui::ColorEdit3("point light color", &light.color.r);
		ImGui::SliderFloat("point light intensity", &light.color.w, 0, 10);
	}

	ImGui::End();
}

void Level::onEvent(geg::Event& event) {
	geg::Dispatcher dis(event);
}

void Level::onUpdate(float deltaTime) {
	camController.update(deltaTime);
	scene.onUpdate(deltaTime, camController.getCam());
}

void Level::onUiUpdate(float deltaTime) {
	camController.drawUi();

	const auto view = scene.getReg().view<geg::NameComponent>();
	for (const auto e : view) {
		geg::Entity ent{&scene, e};
		const auto& name = ent.getComponent<geg::NameComponent>().name;
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
geg::App* geg::createApp() {
	App* a = new sandboxApp();
	return a;
}
