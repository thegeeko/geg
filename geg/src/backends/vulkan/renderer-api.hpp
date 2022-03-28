#pragma once

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/shader.hpp"
#include "backends/vulkan/uniform-buffers.hpp"
#include "core/core.hpp"
#include "glm/fwd.hpp"
#include "renderer/renderer-api.hpp"
#include "renderer/renderer.hpp"

namespace Geg {

	struct VulkanFrame {
		int index;
		int globalUboOffset;

		VkFence fence;
		VkCommandBuffer commandBuffer;
	};

	class VulkanRendererAPI: public RendererAPI {
	public:
		VulkanRendererAPI();
		~VulkanRendererAPI() override;

		void startFrame(GpuSceneData _uboData) override;
		void endFrame() override;
		
		void drawMesh(const MeshComponent* mesh, MeshRenderData meshData) override;

		void clear() override{};
		void clear(glm::vec4 color) override{};

	private:
		VulkanGraphicsContext* context;

		// Syncing
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VulkanFrame> frames;
		std::vector<VkFence> busyFences;
		uint32_t currentInFlightFrame = 0;
		uint32_t nextFrame = 0;

		// Global ubo
		Scope<VulkanUniform> globalUbo;
		GpuSceneData uboData;
		// look into making this a cache system or smth
		std::unordered_map<size_t, VulkanPipeline*> framePipelines;

		void initSyncObjects();
		void deInitSyncObjects();

		void initGlobalUbo();
		void deInitGlobalUbo(){};

		// helpers
		void beginRecording();
		void endRecording();
		void oncePerFrame();
	};
}		 // namespace Geg
