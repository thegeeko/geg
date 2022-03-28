#pragma once

#include "descriptors.hpp"
#include "renderer/graphics-context.hpp"
#include "swap-chain.hpp"
#include "vk_mem_alloc.h"
#include "vulkan-device.hpp"

namespace Geg {

	class VulkanGraphicsContext: public GraphicsContext {
	public:
		VulkanGraphicsContext(GLFWwindow* window);
		~VulkanGraphicsContext() override;

		static constexpr int MAX_FRAMES_IN_FLIGHT = 3;

		VulkanDevice* device;
		VulkanSwapChain* swapChain;
		VkCommandPool commandPool;
		VmaAllocator allocator;
		DescriptorLayoutCache* descriptorLayoutCache;
		DescriptorAllocator* descriptorsAlloc;

		void init() override{};
		void deInit();

		void swapBuffers() override{};

		// memory related
		static void uploadDataToGpuMem(void* data, size_t size, VkBuffer bufferHandle);
		static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static VkCommandBuffer beginSingleTimeCommand();
		static void endSingleTimeCommand(VkCommandBuffer commandBuffer);
	};
}		 // namespace Geg
