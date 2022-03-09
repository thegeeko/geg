#pragma once

#include "renderer-api.hpp"
#include "assets/mesh.hpp"
#include "renderer/cams/perspective.hpp"

namespace Geg {

	struct GpuSceneData {
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 ProjView;
	};

	struct GpuModelData {
		glm::mat4 model;

		GpuModelData(glm::mat4 m) :
			model(m) {}
	};

	class Renderer {
	public:
		static void initAPI();
		static void deInitAPI();

		static void beginScene(Camera cam);
		static void endScene();
		static void submit(const Ref<Pipeline>& pipeline, bool isIndexed = true);
		static void submit(const Ref<Pipeline>& pipeline, GpuModelData model);
		static GraphicsAPI getAPI() { return GEG_CURRENT_API; }

	private:
		static RendererAPI* currentAPI;
	};
} // namespace Geg
