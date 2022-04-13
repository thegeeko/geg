#include "renderer.hpp"
#include "backends/vulkan/renderer-api.hpp"
#include "renderer/cams/perspective.hpp"
#include "renderer/renderer-api.hpp"

namespace Geg {
	RendererAPI* Renderer::currentAPI = nullptr;

	void Renderer::initAPI(){
		currentAPI = new VulkanRendererAPI;
	}

	void Renderer::deInitAPI(){
		delete currentAPI;
	}

	void Renderer::beginScene(Camera cam, GlobalLightComponent& globalLight){
		GpuSceneData currentScene;
		currentScene.proj = cam.proj;
		currentScene.view = cam.view;
		currentScene.ProjView = cam.proj * cam.view;
		currentScene.lightDir = globalLight.dir;

		currentAPI->startFrame(currentScene);
	}

	void Renderer::endScene(){
		currentAPI->endFrame();
	}

	void Renderer::submit(const MeshComponent* mesh, MeshRenderData meshData) {
		currentAPI->drawMesh(mesh, meshData);
	}

}		 // namespace Geg
