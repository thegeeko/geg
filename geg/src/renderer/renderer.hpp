#pragma once

#include "renderer-commands.hpp"
#include "renderer/renderer-api.hpp"
#include <memory>

namespace Geg {
class Renderer {
public:
	static void beginScene();
	static void endScene();
	static void submit(const std::shared_ptr<VertexArray>& vertexArray);
	static inline RendererAPI::API getAPI(){ return RendererCommands::getAPI(); }
};
} // namespace Geg
