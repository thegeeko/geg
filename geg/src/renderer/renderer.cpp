#include "renderer.hpp"

namespace Geg {
void Renderer::beginScene(){};

void Renderer::endScene(){};

void Renderer::submit(const std::shared_ptr<VertexArray> &vertexArray){
	RendererCommands::drawIndexed(vertexArray);
};

} // namespace Geg
