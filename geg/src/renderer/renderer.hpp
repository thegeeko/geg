#pragma once

#include "geg-pch.hpp"

#include "renderer-api.hpp"
#include "renderer-commands.hpp"

namespace Geg {

	struct GpuSceneData{
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 viewProj;
	};

	struct SceneData{
	};

	class Renderer {
		public:
		static void beginScene();
		static void endScene();
		static void submit(const Ref<Pipeline>& pipeline);
		static inline GraphicsAPI getAPI() { return RendererCommands::getAPI(); }
	};
}		 // namespace Geg
