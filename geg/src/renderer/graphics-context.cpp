#include "graphics-context.hpp"

#include "backends/vulkan/graphics-context.hpp"
#include "renderer.hpp"

namespace Geg {
	GraphicsContext* GraphicsContext::create(GLFWwindow* _windowPtr) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::Vulkan:
				return new VulkanGraphicsContext(_windowPtr);
			default:
				GEG_CORE_ERROR("you must use renderer api");
		}
	}
}		 // namespace Geg
