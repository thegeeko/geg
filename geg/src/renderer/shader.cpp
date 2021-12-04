#include "shader.hpp"
#include "renderer.hpp"
#include "backends/opengl/shader.hpp"
#include "backends/vulkan/shader.hpp"

namespace Geg {
	Shader* Shader::create(std::string vertPath, std::string fragPath) {
		switch (Renderer::getAPI()) {
			/* case GraphicsAPI::OpenGL : return new GLShader(vertPath, fragPath, context); */
			case GraphicsAPI::Vulkan : return new VulkanShader(vertPath, fragPath, App::get().getWindow().getGraphicsContext()); 
			default: GEG_CORE_ERROR("you must use renderer api");
		}
	}
}
