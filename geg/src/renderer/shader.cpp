#include "shader.hpp"
#include "renderer.hpp"
#include "backends/opengl/shader.hpp"

namespace Geg {
	Shader* Shader::create(std::string vertPath, std::string fragPath) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::OpenGL : return new GLShader(vertPath, fragPath);
			case RendererAPI::API::Vulkan : GEG_CORE_ERROR("Vulkan is not supported atm"); 
			default: GEG_CORE_ERROR("you must use renderer api");
		}
	}
};
