#pragma once

#include "backends/opengl/renderer-api.hpp"
#include "backends/vulkan/renderer-api.hpp"
#include "renderer-api.hpp"
#include "vertex-array.hpp"

namespace Geg {
	class RendererCommands {
	public:
		static RendererAPI *API;

		inline static void clear() {
			API->clear();
		};

		inline static void clear(glm::vec4 color) {
			API->clear(color);
		};

		inline static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) {
			API->drawIndexed(vertexArray);
		};

		inline static void drawIndexed() {
			API->drawIndexed();
		};

		inline static void submit(const std::shared_ptr<Shader> &shader) {
			API->submit(shader);
		};

		inline static GraphicsAPI getAPI() { return GEG_CURRENT_API; }

	};		// namespace RendererCommands
}		 // namespace Geg
