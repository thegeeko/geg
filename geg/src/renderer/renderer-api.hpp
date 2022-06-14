#pragma once

#include "ecs/components.hpp"

namespace geg {
	struct GpuSceneData;
	struct MeshRenderData;

	class RendererAPI {
	public:
		virtual ~RendererAPI() = default;
		
		virtual void clear() = 0;
		virtual void clear(glm::vec4 color) = 0;
		
		virtual void startFrame(const GpuSceneData& sceneData) = 0;
		virtual void endFrame() = 0;
		
		virtual void drawMesh(const MeshRenderData& meshData) = 0;
		
	};
}		 // namespace geg
