#include "pipeline.hpp"

#include "renderer.hpp"
#include "backends/vulkan/pipeline.hpp"

namespace Geg {
	Pipeline* Pipeline::create(
			const Ref<VertexBuffer>& vbo,
			const Ref<IndexBuffer>& vio,
			const Ref<Shader>& shader) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::OpenGL:
				GEG_ERROR("OpenGL not supported atm");
			case GraphicsAPI::Vulkan:
				return new VulkanPipeline(vbo, vio, shader);
			default:
				GEG_ERROR("you must use renderer api");
		}
	}
}		 // namespace Geg
