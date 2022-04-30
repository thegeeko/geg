#include "renderer.hpp"

#include "backends/vulkan/renderer-api.hpp"
#include "renderer/cams/perspective.hpp"
#include "renderer/renderer-api.hpp"

namespace Geg {
	RendererAPI* Renderer::currentAPI = nullptr;

	void Renderer::initAPI() {
		currentAPI = new VulkanRendererAPI;
	}

	void Renderer::deInitAPI() {
		delete currentAPI;
	}

	void Renderer::beginScene(const BeginSceneInfo& sceneInfo) {
		GpuSceneData cs;

		cs.proj = sceneInfo.cam->proj;
		cs.view = sceneInfo.cam->view;
		cs.invView = glm::inverse(sceneInfo.cam->view);
		cs.ProjView = cs.proj * cs.view;

		cs.globalLightDir = sceneInfo.globalLight->dir;
		cs.globalLightColor = sceneInfo.globalLight->color;
		cs.ambient = sceneInfo.globalLight->ambient;

		int i = 0;
		for (auto& pl : sceneInfo.pointLights) {
			cs.pointLights[i].light = pl.light->color;
			cs.pointLights[i].pos = {pl.pos->translation, 1.f};
			i++;
		}

		currentAPI->startFrame(cs);
	}

	void Renderer::endScene() {
		currentAPI->endFrame();
	}

	void Renderer::submit(const MeshRenderData& meshData) {
		currentAPI->drawMesh(meshData);
	}

}		 // namespace Geg
