#include "renderer-commands.hpp"
#include "backends/opengl/renderer-api.hpp"

namespace Geg {
	RendererAPI* RendererCommands::API = new GLRendererAPI(); 
}
