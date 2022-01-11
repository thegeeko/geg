#pragma once

#include "backends/opengl/renderer-api.hpp"
#include "backends/vulkan/renderer-api.hpp"
#include "renderer-api.hpp"
#include "pipeline.hpp"

namespace Geg {
	class RendererCommands {
	public:
		static RendererAPI* API;

		inline static void clear() {
			API->clear();
		};

		inline static void startFrame() {
			API->startFrame();
		}

		inline static void endFrame() {
			API->endFrame();
		}

		inline static void clear(glm::vec4 color) {
			API->clear(color);
		};

		inline static void drawIndexed(const Ref<Pipeline>& vertexArray) {
			API->drawIndexed(vertexArray);
		};

		inline static GraphicsAPI getAPI() { return GEG_CURRENT_API; }

	};		// namespace RendererCommands
}		 // namespace Geg
