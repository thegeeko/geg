#include "renderer.hpp"

namespace Geg {
	void Renderer::beginScene(){
		RendererCommands::init();
	};

	void Renderer::endScene(){};

	void Renderer::submit(const Ref<Pipeline>& pipeline) {
		RendererCommands::drawIndexed(pipeline);
	};
}		 // namespace Geg
