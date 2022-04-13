#include "graphics-context.hpp"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace Geg {

	// const int VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT = 3;

	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow* _windowPtr) {
		// device and swapchain
		device = new VulkanDevice(_windowPtr);

		// command pool
		QueueFamilyIndices queueFamilyIndices;
		device->findQueueFamilies(device->getPhysicalDevice(), queueFamilyIndices);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;		 // Optional

		VkResult result = vkCreateCommandPool(device->getDevice(), &poolInfo, nullptr, &commandPool);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create command pool");

		// descriptors
		descriptorLayoutCache = new DescriptorLayoutCache(device);
		descriptorsAlloc = new DescriptorAllocator(device);

		// vma
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocatorInfo.physicalDevice = device->getPhysicalDevice();
		allocatorInfo.device = device->getDevice();
		allocatorInfo.instance = device->getInstance();

		vmaCreateAllocator(&allocatorInfo, &allocator);

		swapChain = new VulkanSwapChain(_windowPtr, device, allocator);
	}

	VulkanGraphicsContext::~VulkanGraphicsContext() {
		delete descriptorLayoutCache;
		delete descriptorsAlloc;
		vmaDestroyAllocator(allocator);
		vkDestroyCommandPool(device->getDevice(), commandPool, nullptr);
		delete swapChain;
		delete device;
	}

	// Static helper functions
	/**
	 * @brief
	 * 	creates staging buffer and map it and copy the data and the make a copy command to move
	 *	the data to the provided buffer
	 * @param data a void pointer to the data that will be uploaded
	 * @param size the size of the data
	 * @param bufferHandle vulkan buffer handle which the data will be copied to
	 */
	void VulkanGraphicsContext::uploadDataToGpuMem(void* data, size_t size, VkBuffer bufferHandle) {
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

	/**
	 * @brief
	 * 	record a copy command from @param srcBuffer to @param dstBuffer
	 * 	and submit it
	 * @param srcBuffer
	 * @param dstBuffer
	 * @param size
	 */
	void VulkanGraphicsContext::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		auto commandBuffer = beginSingleTimeCommand();

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;		 // Optional
		copyRegion.dstOffset = 0;		 // Optional
		copyRegion.size = size;

		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommand(commandBuffer);
	}

	/**
	 * @brief
	 * creates a command buffer and begins it
	 * u should call end single time command buffer after
	 *
	 * @return VkCommandBuffer
	 */
	VkCommandBuffer VulkanGraphicsContext::beginSingleTimeCommand() {
		auto context = dynamic_cast<VulkanGraphicsContext*>(
				App::get().getWindow().getGraphicsContext());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = context->commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(context->device->getDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		return commandBuffer;
	}

	/**
	 * @brief
	 *  ends the command buffer and submit it and then cleans it
	 * @param commandBuffer the command to be
	 */
	void VulkanGraphicsContext::endSingleTimeCommand(VkCommandBuffer commandBuffer) {
		auto context = dynamic_cast<VulkanGraphicsContext*>(
				App::get().getWindow().getGraphicsContext());

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(context->device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(context->device->getGraphicsQueue());

		vkFreeCommandBuffers(context->device->getDevice(), context->commandPool, 1, &commandBuffer);
	}
}		 // namespace Geg
