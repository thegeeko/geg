#pragma once

#include <cstdint>
#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/shader.hpp"
#include "backends/vulkan/vertex-buffer.hpp"
#include "renderer/renderer-api.hpp"



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
		~VulkanRendererAPI();

		void startFrame() override;
		void endFrame() override;
		void drawIndexed(const Ref<Pipeline>& _pipeline) override;

		void clear() override{};
		void clear(glm::vec4 color) override{};

	private:
		VulkanGraphicsContext* context;

		// syncing
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VulkanFrame> frames;
		std::vector<VkFence> busyFences;
		uint32_t currentInFlightFrame = 0;
		uint32_t nextFrame = 0;

		void initSyncObjects();
		void deInitSyncObjects();

		void recordCommands();
		void beginRecording();
		void endRecording();
	};
}		 // namespace Geg
