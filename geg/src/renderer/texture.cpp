#include "texture.hpp"

#include "renderer.hpp"
#include "backends/vulkan/texture.hpp"

namespace geg {
	Texture* Texture::create(const std::string& path) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::Vulkan: return new VulkanTexture(path);
			default: GEG_CORE_ASSERT(false, "you must use renderer api");
		}
	}
}		 // namespace geg
