#include "renderer.hpp"
#include "backends/vulkan/renderer-api.hpp"
#include "renderer/cams/perspective.hpp"
#include "renderer/renderer-api.hpp"

namespace Geg {
	RendererAPI* currentAPI = nullptr;

	void Renderer::initAPI(){
		currentAPI = new VulkanRendererAPI;
	}

	void Renderer::deInitAPI(){
		delete currentAPI;
	}

	void Renderer::beginScene(Camera cam){
		GpuSceneData currentScene{};
		currentScene.proj = cam.getProj();
		currentScene.view = cam.getView();
		currentScene.ProjView = cam.getProjView();

		currentAPI->startFrame(currentScene);
	};

	void Renderer::endScene(){
		currentAPI->endFrame();
	};

	void Renderer::submit(const Ref<Pipeline>& pipeline) {
		currentAPI->drawIndexed(pipeline);
	};


}		 // namespace Geg
