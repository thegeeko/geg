#include "vertex-array.hpp"
#include "renderer.hpp"
#include "backends/opengl/vertex-array.hpp"


namespace Geg {
	VertexArray* VertexArray::create() {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::OpenGL:
			return new GLVertexArray();
		case RendererAPI::API::Vulkan:
			GEG_ERROR("Vulkan not supported atm");
		default:
			GEG_ERROR("you must use renderer api");
		}
	}
}
