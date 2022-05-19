#pragma once

#include "ecs/components.hpp"
#include "glm/fwd.hpp"
#include "renderer-api.hpp"
#include "renderer/cams/perspective.hpp"

namespace Geg {

	constexpr uint32_t MAX_POINT_LIGHTS = 100;

	struct PointLight {
		PointLightComponent* light;
		TransformComponent* pos;

		PointLight() = default;
		PointLight(PointLightComponent* l, TransformComponent* p):
				light(l), pos(p){};
	};

	struct PointLightData {
		glm::vec4 light = glm::vec4(0.f);
		glm::vec4 pos = glm::vec4(0.f);
	};

	// global ubo
	struct GpuSceneData {
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 invView;
		glm::mat4 ProjView;

		glm::vec4 globalLightDir;
		glm::vec4 globalLightColor;
		glm::vec4 ambient;

		std::array<PointLightData, MAX_POINT_LIGHTS> pointLights;
	};

	//object ubo
	struct GpuModelData {
		glm::vec4 color;
		bool useTex;
	};

	struct BeginSceneInfo {
		Camera* cam;
		GlobalLightComponent* globalLight;

		std::vector<PointLight> pointLights;
	};

	struct MeshRenderData {
		uint32_t id;
		MeshComponent* meshC;
		TransformComponent* transformC;
		MeshRendererComponent* rendererC;
	};

	class Renderer {
	public:
		static void initAPI();
		static void deInitAPI();

		static void beginScene(const BeginSceneInfo& sceneInfo);
		static void endScene();
		static void submit(const MeshRenderData& meshData);
		static GraphicsAPI getAPI() { return GEG_CURRENT_API; }
		static auto getApiHandel() { return currentAPI; };

	private:
		static RendererAPI* currentAPI;
	};
}		 // namespace Geg
