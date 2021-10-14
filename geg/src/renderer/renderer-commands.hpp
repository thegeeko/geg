#pragma once

#include "renderer-api.hpp"
#include "vertex-array.hpp"

namespace Geg {
	class RendererCommands {
	 public:
		inline static void clear() {
			API->clear();
		};

		inline static void clear(glm::vec4 color) {
			API->clear(color);
		};

		inline static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) {
			API->drawIndexed(vertexArray);
		};

		inline static RendererAPI::API getAPI() { return API->i_API; }

	 private:
		static RendererAPI *API;
	};
}		 // namespace Geg
