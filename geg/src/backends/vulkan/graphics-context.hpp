#pragma once

#include "renderer/graphics-context.hpp"
#include "swap-chain.hpp"
#include "vulkan-device.hpp"
#include "command-buffers.hpp"
#include "vk_mem_alloc.h"
#include "descriptors.hpp"

namespace Geg {
	class VulkanGraphicsContext: public GraphicsContext {
	public:
		VulkanGraphicsContext(GLFWwindow* window);
		~VulkanGraphicsContext() override;

		VulkanDevice* device;
		VulkanSwapChain* swapChain;
		VulkanCommandBuffers* commandBuffers;
		VmaAllocator allocator;
		DescriptorLayoutCache* descriptorLayoutCache;
		DescriptorAllocator* descriptorsAlloc;

		void init() override{};
		void swapBuffers() override{};

		// vulkan related
		static void uploadDataToGpuMem(void* data, size_t size, VkBuffer bufferHandle);
		static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	};
}		 // namespace Geg
