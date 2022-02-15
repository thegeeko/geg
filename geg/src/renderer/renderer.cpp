#include "renderer.hpp"
#include "backends/vulkan/renderer-api.hpp"
#include "renderer/renderer-api.hpp"

namespace Geg {
	RendererAPI* currentAPI = nullptr;

	void Renderer::initAPI(){
		currentAPI = new VulkanRendererAPI;
	}

	void Renderer::deInitAPI(){
		delete currentAPI;
	}

	void Renderer::beginScene(){
		currentAPI->startFrame();
	};

	void Renderer::endScene(){
		currentAPI->endFrame();
	};

	void Renderer::submit(const Ref<Pipeline>& pipeline) {
		currentAPI->drawIndexed(pipeline);
	};


}		 // namespace Geg
