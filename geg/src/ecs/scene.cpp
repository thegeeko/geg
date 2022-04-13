#include "scene.hpp"

#include "components.hpp"
#include "entity.hpp"
#include "renderer/renderer.hpp"

namespace Geg {
	Scene::Scene() {}

	Scene::~Scene() {}

	Entity Scene::createEntity() {
		Entity e(this, registry.create());
		e.addComponent<NameComponent>();
		e.addComponent<TransformComponent>();

		return e;
	}

	Entity Scene::createEntity(const std::string& name) {
		Entity e(this, registry.create());
		e.addComponent<NameComponent>(name);
		e.addComponent<TransformComponent>();

		return e;
	}

	void Scene::onUpdate(float deltaTime, Camera cam) {
		// render stuff

		const auto lights = registry.view<GlobalLightComponent>();
		auto& gl = lights.get<GlobalLightComponent>(lights.back());
		Renderer::beginScene(cam, gl);

		const auto group = registry.group<MeshRendererComponent>(entt::get<TransformComponent, MeshComponent>);
		for (const auto& entity : group) {
			auto& trans = group.get<TransformComponent>(entity);
			auto& mesh = group.get<MeshComponent>(entity);
			auto& meshr = group.get<MeshRendererComponent>(entity);

			MeshRenderData renderData{};
			renderData.material = &meshr;
			renderData.modelMat = &trans;

			Renderer::submit(&mesh, renderData);
		}

		Renderer::endScene();
	}
}		 // namespace Geg
