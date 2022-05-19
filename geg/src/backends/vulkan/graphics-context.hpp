#pragma once

#include "descriptors.hpp"
#include "renderer/graphics-context.hpp"
#include "swap-chain.hpp"
#include "vk_mem_alloc.h"
#include "vulkan-device.hpp"

namespace Geg {

	class VulkanGraphicsContext: public GraphicsContext {
	public:
		VulkanGraphicsContext(GLFWwindow* _window);
		~VulkanGraphicsContext() override;

		static constexpr int MAX_FRAMES_IN_FLIGHT = 1;

		GLFWwindow* window;

		VulkanDevice* device;
		VulkanSwapChain* swapChain;
		VkCommandPool commandPool;
		VmaAllocator allocator;
		DescriptorLayoutCache* descriptorLayoutCache;
		DescriptorAllocator* descriptorsAlloc;

		void init() override{};
		void deInit();

		void swapBuffers() override{};
		void windowResized(int width, int height) override;

		// helpers functions
		static void uploadDataToBuffer(const void* data, size_t size, VkBuffer bufferHandle);
		static void uploadDataToImage(const void* data, size_t size, VkImage imageHandle, uint32_t width, uint32_t height);
		static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		static VkCommandBuffer beginSingleTimeCommand();
		static void endSingleTimeCommand(VkCommandBuffer commandBuffer);
	};
}		 // namespace Geg
