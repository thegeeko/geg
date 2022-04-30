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
		BeginSceneInfo sc;

		sc.cam = &cam;

		const auto globalLight = registry.view<GlobalLightComponent>();
		auto& gl = globalLight.get<GlobalLightComponent>(globalLight.back());
		sc.globalLight = &gl;

		const auto pLights = registry.group<PointLightComponent>(entt::get<TransformComponent>);
		int i = 0;
		for (const auto& entity : pLights) {
			auto& pointLightComponent = pLights.get<PointLightComponent>(entity);
			auto& transformComponent = pLights.get<TransformComponent>(entity);
			sc.pointLights.emplace_back(&pointLightComponent, &transformComponent);
			i++;
		}

		Renderer::beginScene(sc);

		const auto group = registry.group<MeshRendererComponent>(entt::get<TransformComponent, MeshComponent>);
		for (const auto& entity : group) {
			auto& trans = group.get<TransformComponent>(entity);
			auto& mesh = group.get<MeshComponent>(entity);
			auto& meshr = group.get<MeshRendererComponent>(entity);

			MeshRenderData renderData{};
			renderData.id = (uint32_t)entity;
			renderData.meshC = &mesh;
			renderData.rendererC = &meshr;
			renderData.transformC = &trans;

			Renderer::submit(renderData);
		}

		Renderer::endScene();
	}
}		 // namespace Geg
