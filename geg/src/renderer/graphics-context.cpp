#include "graphics-context.hpp"

#include "backends/opengl/graphics-context.hpp"
#include "renderer.hpp"

namespace Geg {
	GraphicsContext* GraphicsContext::create(GLFWwindow* _windowPtr) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::OpenGL:
				return new GLGraphicsContext(_windowPtr);
			case RendererAPI::API::Vulkan:
				GEG_CORE_ERROR("Vulkan not supported atm");
			default:
				GEG_CORE_ERROR("you must use renderer api");
		}
	}
}		 // namespace Geg
