#include "renderer.hpp"

namespace Geg {
	void Renderer::beginScene(){
		RendererCommands::startFrame();
	};

	void Renderer::endScene(){
		RendererCommands::endFrame();
	};

	void Renderer::submit(const Ref<Pipeline>& pipeline) {
		RendererCommands::drawIndexed(pipeline);
	};
}		 // namespace Geg
