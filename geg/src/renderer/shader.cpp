#include "shader.hpp"
#include "renderer.hpp"
#include "backends/vulkan/shader.hpp"

namespace geg {
	Shader* Shader::create(const std::vector<uint32_t>& vertSrc, const std::vector<uint32_t>& fragSrc) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::Vulkan : return new VulkanShader(vertSrc, fragSrc); 
			default: GEG_CORE_ASSERT(false, "you must use renderer api");
		}
	}
}
