#pragma once

#include "glm/vec4.hpp"
#include "pipeline.hpp"

namespace Geg {
	struct GpuSceneData;

	class RendererAPI {
	public:
		virtual ~RendererAPI(){};
		virtual void clear() = 0;
		virtual void startFrame(GpuSceneData sceneData) = 0;
		virtual void endFrame() = 0;
		virtual void clear(glm::vec4 color) = 0;
		virtual void drawIndexed(const Ref<Pipeline>& pipeline) = 0;
		virtual void draw(const Ref<Pipeline>& pipeline) = 0;
	};
}		 // namespace Geg
