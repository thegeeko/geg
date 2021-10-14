#pragma once

#include <memory>

#include "renderer/renderer-api.hpp"

namespace Geg {
	class GLRendererAPI: public RendererAPI {
	 public:
		void clear() override;
		void clear(glm::vec4) override;
		void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}		 // namespace Geg
