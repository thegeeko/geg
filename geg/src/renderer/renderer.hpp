#pragma once

#include <memory.h>

#include "renderer-api.hpp"
#include "renderer-commands.hpp"

namespace Geg {
	struct Renderer {
		static void beginScene();
		static void endScene();
		static void submit(const std::shared_ptr<VertexArray>& vertexArray);
		static inline RendererAPI::API getAPI() { return RendererCommands::getAPI(); }
	};
}		 // namespace Geg
