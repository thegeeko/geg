#include "vertex-array.hpp"
#include "renderer.hpp"
#include "backends/opengl/vertex-array.hpp"


namespace Geg {
	VertexArray* VertexArray::create() {
		switch (Renderer::getAPI()) {
		case GraphicsAPI::OpenGL:
			return new GLVertexArray();
		case GraphicsAPI::Vulkan:
			GEG_ERROR("Vulkan not supported atm");
		default:
			GEG_ERROR("you must use renderer api");
		}
	}
}
