#pragma once

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/shader.hpp"
#include "backends/vulkan/uniform-buffers.hpp"
#include "backends/vulkan/vertex-buffer.hpp"
#include "core/core.hpp"
#include "glm/fwd.hpp"
#include "renderer/renderer-api.hpp"
#include "renderer/renderer.hpp"

namespace Geg {

	struct UboTest {
		glm::vec4 cam = glm::vec4(80.f);
		glm::vec4 color = glm::vec4(80.f);
	};

	struct VulkanFrame {
		int index;
		int globalUboOffset;

		VkFence fence;
		VkCommandBuffer commandBuffer;
	};

	class VulkanRendererAPI: public RendererAPI {
	public:
		VulkanRendererAPI();
		~VulkanRendererAPI();

		void startFrame(GpuSceneData _uboData) override;
		void endFrame() override;
		void drawIndexed(const Ref<Pipeline>& _pipeline) override;
		void draw(const Ref<Pipeline>& _pipeline) override;

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

		void initSyncObjects();
		void deInitSyncObjects();

		void initGlobalUbo();
		void deInitGlobalUbo(){};

		void recordCommands();
		void beginRecording();
		void endRecording();
	};
}		 // namespace Geg
