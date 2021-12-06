#pragma once

#include "glm/vec4.hpp"
#include "vertex-array.hpp"

namespace Geg {
	class RendererAPI {
	public:
		/* virtual ~RendererAPI(){}; */
		virtual void clear() = 0;
		virtual void clear(glm::vec4 color) = 0;
		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void drawIndexed() = 0;
		virtual void submit(const std::shared_ptr<Shader>& shader) = 0;
	};
}		 // namespace Geg
