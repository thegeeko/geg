#pragma once

#include "ecs/components.hpp"

namespace Geg {
	struct GpuSceneData;
	struct MeshRenderData;

	class RendererAPI {
	public:
		virtual ~RendererAPI(){};
		
		virtual void clear() = 0;
		virtual void clear(glm::vec4 color) = 0;
		
		virtual void startFrame(GpuSceneData sceneData) = 0;
		virtual void endFrame() = 0;
		
		virtual void drawMesh(const MeshComponent* mesh, MeshRenderData meshData) = 0;
		
	};
}		 // namespace Geg
