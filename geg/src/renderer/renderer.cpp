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
		currentScene.proj = cam.proj;
		currentScene.view = cam.view;
		currentScene.ProjView = cam.proj * cam.view;

		currentAPI->startFrame(currentScene);
	};

	void Renderer::endScene(){
		currentAPI->endFrame();
	};

	void Renderer::submit(const Ref<Pipeline>& pipeline, bool isIndexed) {
		if (isIndexed)
			currentAPI->drawIndexed(pipeline);
		else
			currentAPI->draw(pipeline);
	};


}		 // namespace Geg
