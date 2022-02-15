#pragma once

#include "core/core.hpp"
#include "geg-pch.hpp"
#include "renderer-api.hpp"

namespace Geg {

	struct GpuSceneData {
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 viewProj;
	};

	struct SceneData {
	};

	class Renderer {
	public:
		static void initAPI();
		static void deInitAPI();

		static void beginScene();
		static void endScene();
		static void submit(const Ref<Pipeline>& pipeline);
		static inline GraphicsAPI getAPI() { return GEG_CURRENT_API; }

	private:
		static RendererAPI* currentAPi;
	};
}		 // namespace Geg
