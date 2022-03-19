#include "shader.hpp"
#include "renderer.hpp"
#include "backends/vulkan/shader.hpp"

namespace Geg {
	Shader* Shader::create(std::vector<uint32_t> vertSrc, std::vector<uint32_t> fragSrc) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::Vulkan : return new VulkanShader(vertSrc, fragSrc); 
			default: GEG_CORE_ERROR("you must use renderer api");
		}
	}
}
