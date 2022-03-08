#pragma once

#include "renderer-api.hpp"
#include "renderer/cams/perspective.hpp"

namespace Geg {

	struct GpuSceneData {
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 ProjView;
	};

	class Renderer {
	public:
		static void initAPI();
		static void deInitAPI();

		static void beginScene(Camera cam);
		static void endScene();
		static void submit(const Ref<Pipeline>& pipeline, bool isIndexed = true);
		static inline GraphicsAPI getAPI() { return GEG_CURRENT_API; }

	private:
		static RendererAPI* currentAPi;
	};
}		 // namespace Geg
