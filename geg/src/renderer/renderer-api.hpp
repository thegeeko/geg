#pragma once

#include "glm/vec4.hpp"
#include "pipeline.hpp"

namespace Geg {
	class RendererAPI {
	public:
		/* virtual ~RendererAPI(){}; */
		virtual void clear() = 0;
		virtual void init() = 0;
		virtual void clear(glm::vec4 color) = 0;
		virtual void drawIndexed(const Ref<Pipeline>& pipeline) = 0;
	};
}		 // namespace Geg
