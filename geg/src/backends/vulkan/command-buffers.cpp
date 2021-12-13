#include "command-buffers.hpp"

namespace Geg {
	VulkanCommandBuffers::VulkanCommandBuffers(VulkanDevice* _device, VulkanSwapChain* _swapChain):
			device(_device), swapChain(_swapChain) {
		QueueFamilyIndices queueFamilyIndices;
		device->findQueueFamilies(device->getPhysicalDevice(), queueFamilyIndices);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;		 // Optional

		VkResult result = vkCreateCommandPool(device->getDevice(), &poolInfo, nullptr, &commandPool);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create command pool");

		commandBuffers.resize(swapChain->getSwapChainFrameBuffers().size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		createSyncObjects();

		result = vkAllocateCommandBuffers(device->getDevice(), &allocInfo, commandBuffers.data());
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't allocat command buffers")

		GEG_CORE_INFO("command pool and command buffers created");
	}

	VulkanCommandBuffers::~VulkanCommandBuffers() {
		vkDestroyCommandPool(device->getDevice(), commandPool, nullptr);
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(device->getDevice(), renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(device->getDevice(), imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(device->getDevice(), inFlightFences[i], nullptr);
		}
		GEG_CORE_INFO("command pool and command buffers destroyed");
	}

	void VulkanCommandBuffers::createSyncObjects() {
		VkResult result;

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(swapChain->getSwapChainImages().size(), VK_NULL_HANDLE);
		

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			result = vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create a semaphore");

			result = vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create a semaphore");

			result = vkCreateFence(device->getDevice(), &fenceInfo, nullptr, &inFlightFences[i]);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create fences")
		}
	};
}		 // namespace Geg
