#include "command-buffers.hpp"

#include <vulkan/vulkan_core.h>

namespace Geg {
	VulkanCommandBuffers::VulkanCommandBuffers(VulkanDevice* _device, VulkanSwapChain* _swapChain):
			device(_device), swapChain(_swapChain) {
		QueueFamilyIndices queueFamilyIndices;
		device->findQueueFamilies(device->getPhysicalDevice(), queueFamilyIndices);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = 0;		 // Optional

		VkResult result = vkCreateCommandPool(device->getDevice(), &poolInfo, nullptr, &commandPool);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create command pool");

		commandBuffers.resize(swapChain->getSwapChainFrameBuffers().size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		createSemaphores();

		result = vkAllocateCommandBuffers(device->getDevice(), &allocInfo, commandBuffers.data());
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't allocat command buffers")

		GEG_CORE_INFO("command pool and command buffers created");
	}

	VulkanCommandBuffers::~VulkanCommandBuffers() {
		vkDestroyCommandPool(device->getDevice(), commandPool, nullptr);
		vkDestroySemaphore(device->getDevice(), renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device->getDevice(), imageAvailableSemaphore, nullptr);
		GEG_CORE_INFO("command pool and command buffers destroyed");
	}

	void VulkanCommandBuffers::createSemaphores() {
		VkResult result;

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		result = vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create a semaphore");

		result = vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create a semaphore");
	};
}		 // namespace Geg
