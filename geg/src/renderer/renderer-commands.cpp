#include "renderer-commands.hpp"

#include "backends/opengl/renderer-api.hpp"
#include "backends/vulkan/renderer-api.hpp"
#include "renderer-api.hpp"
#include "vertex-array.hpp"

namespace Geg {
	RendererAPI *RendererCommands::API = new VulkanRendererAPI();
}		 // namespace Geg
