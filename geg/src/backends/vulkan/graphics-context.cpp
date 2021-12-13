#include "graphics-context.hpp"

#include "backends/vulkan/command-buffers.hpp"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace Geg {
	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow* _windowPtr) {
		device = new VulkanDevice(_windowPtr);
		swapChain = new VulkanSwapChain(_windowPtr, device);
		commandBuffers = new VulkanCommandBuffers(device, swapChain);

		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocatorInfo.physicalDevice = device->getPhysicalDevice();
		allocatorInfo.device = device->getDevice();
		allocatorInfo.instance = device->getInstance();

		vmaCreateAllocator(&allocatorInfo, &allocator);
	}

	VulkanGraphicsContext::~VulkanGraphicsContext() {
		vmaDestroyAllocator(allocator);
		delete commandBuffers;
		delete swapChain;
		delete device;
	}

	void VulkanGraphicsContext::uploadDataToGpuMem(
			void* data, size_t size, VkBuffer bufferHandle) {
		auto context = dynamic_cast<VulkanGraphicsContext*>(
				App::get().getWindow().getGraphicsContext());

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = static_cast<size_t>(size);
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_COPY;
		allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
															VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VkBuffer stagingBuffer;
		VmaAllocation stagingAlloc;

		VkResult result = vmaCreateBuffer(
				context->allocator,
				&bufferInfo,
				&allocInfo,
				&stagingBuffer,
				&stagingAlloc,
				nullptr);

		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create staging buffer");

		void* mappingAddr;
		vmaMapMemory(context->allocator, stagingAlloc, &mappingAddr);
		memcpy(mappingAddr, data, size);
		vmaUnmapMemory(context->allocator, stagingAlloc);

		copyBuffer(stagingBuffer, bufferHandle, size);

		vmaDestroyBuffer(context->allocator, stagingBuffer, stagingAlloc);
	}

	void VulkanGraphicsContext::copyBuffer(
			VkBuffer srcBuffer,
			VkBuffer dstBuffer,
			VkDeviceSize size) {
		auto context = dynamic_cast<VulkanGraphicsContext*>(
				App::get().getWindow().getGraphicsContext());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = context->commandBuffers->getPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(context->device->getDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;		 // Optional
		copyRegion.dstOffset = 0;		 // Optional
		copyRegion.size = size;

		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(context->device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(context->device->getGraphicsQueue());

		vkFreeCommandBuffers(
				context->device->getDevice(),
				context->commandBuffers->getPool(),
				1,
				&commandBuffer);
	}
}		 // namespace Geg
