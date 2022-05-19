#pragma once

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/shader.hpp"
#include "backends/vulkan/uniform-buffers.hpp"
#include "glm/fwd.hpp"
#include "renderer/renderer-api.hpp"
#include "renderer/renderer.hpp"
#include "texture.hpp"

namespace Geg {

	struct VulkanFrame {
		int index;

		VkFence fence;
		VkCommandBuffer commandBuffer;
	};

	class VulkanRendererAPI: public RendererAPI {
	public:
		VulkanRendererAPI();
		~VulkanRendererAPI() override;

		void startFrame(const GpuSceneData& _uboData) override;
		void endFrame() override;

		void drawMesh(const MeshRenderData& meshData) override;

		void clear() override{
				// will implement this later
		};
		void clear(glm::vec4 color) override{
				// will implement this later
		};

		void clearPipelineCache();

	private:
		VulkanGraphicsContext* context;

		// Syncing
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VulkanFrame> frames;
		std::vector<VkFence> busyFences;
		uint32_t currentInFlightFrame = 0;
		uint32_t nextFrame = 0;

		// ubos
		GpuSceneData globalUboData;
		Scope<VulkanUniform> globalUbo = nullptr;
		GpuModelData objectUboData;
		std::unordered_map<uint32_t, Scope<VulkanUniform>> objectUbo;

		std::unordered_map<size_t, Scope<VulkanPipeline>> pipelineCache;

		VulkanTexture dummyTexture;

		void initSyncObjects();
		void deInitSyncObjects();

		void initGlobalUbo();

		// helpers
		void beginRecording();
		void endRecording();
		void oncePerFrame();
	};
}		 // namespace Geg
