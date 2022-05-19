#include "graphics-context.hpp"

#include "backends/vulkan/renderer-api.hpp"
#include "renderer/renderer.hpp"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace Geg {

	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow* _window):
			window(_window) {
		// device and swapchain
		device = new VulkanDevice(window);

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

		swapChain = new VulkanSwapChain(window, device, allocator);
	}

	VulkanGraphicsContext::~VulkanGraphicsContext() {
		delete descriptorLayoutCache;
		delete descriptorsAlloc;
		delete swapChain;
		vmaDestroyAllocator(allocator);
		vkDestroyCommandPool(device->getDevice(), commandPool, nullptr);
		delete device;
	}

	void VulkanGraphicsContext::windowResized(int width, int height) {
		// @FIXME I need to do this .. it crashes rn

		GEG_CORE_TRACE("Window resize {} / {}", width, height);
		// auto apiHandle = dynamic_cast<VulkanRendererAPI*>(Renderer::getApiHandel());
		// vkDeviceWaitIdle(device->getDevice());
		// swapChain->handleResize();
		// apiHandle->clearPipelineCache();
	}

	//============= Static helper functions ======================

	/**
	 * @brief
	 * 	creates staging buffer and map it and copy the data and the make a copy command to move
	 *	the data to the provided buffer
	 * @param data a void pointer to the data that will be uploaded
	 * @param size the size of the data
	 * @param bufferHandle vulkan buffer handle which the data will be copied to
	 */
	void VulkanGraphicsContext::uploadDataToBuffer(const void* data, size_t size, VkBuffer bufferHandle) {
		auto context = dynamic_cast<VulkanGraphicsContext*>(
				App::get().getWindow().getGraphicsContext());

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
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

	void VulkanGraphicsContext::uploadDataToImage(const void* data, size_t size, VkImage imageHandle, uint32_t width, uint32_t height) {
		auto context = dynamic_cast<VulkanGraphicsContext*>(
				App::get().getWindow().getGraphicsContext());

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
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

		transitionImageLayout(
				imageHandle,
				VK_FORMAT_R8G8B8A8_SRGB,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		copyBufferToImage(stagingBuffer, imageHandle, width, height);

		vmaDestroyBuffer(context->allocator, stagingBuffer, stagingAlloc);

		transitionImageLayout(
				imageHandle,
				VK_FORMAT_R8G8B8A8_SRGB,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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

	void VulkanGraphicsContext::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommand();
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags srcStage;
		VkPipelineStageFlags dstStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else {
			GEG_CORE_ERROR("Unsupported image transition");
		}

		vkCmdPipelineBarrier(
				commandBuffer,
				srcStage,
				dstStage,
				0,
				0,
				nullptr,
				0,
				nullptr,
				1,
				&barrier);

		endSingleTimeCommand(commandBuffer);
	}

	void VulkanGraphicsContext::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommand();
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = {0, 0, 0};
		region.imageExtent = {
				width,
				height,
				1};

		vkCmdCopyBufferToImage(
				commandBuffer,
				buffer,
				image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&region);

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