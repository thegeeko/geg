#pragma once

#include "glm/vec4.hpp"
#include "vertex-array.hpp"

namespace Geg {
	

	class RendererAPI {
	public:
		virtual void clear() = 0;
		virtual void clear(glm::vec4 color) = 0;
		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
	};
}		 // namespace Geg
