#pragma once

#include "renderer-api.hpp"
#include "ecs/components.hpp"
#include "renderer/cams/perspective.hpp"

namespace Geg {

	struct GpuSceneData {
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 ProjView;
	};

	struct MeshRenderData {
		TransformComponent* modelMat;
		MeshRendererComponent* material;
	};

	class Renderer {
	public:
		static void initAPI();
		static void deInitAPI();

		static void beginScene(Camera cam);
		static void endScene();
		static void submit(const MeshComponent* mesh, MeshRenderData meshData);
		static GraphicsAPI getAPI() { return GEG_CURRENT_API; }

	private:
		static RendererAPI* currentAPI;
	};
} // namespace Geg
