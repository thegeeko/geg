#pragma once

#include <vulkan/vulkan.hpp>

#include "swap-chain.hpp"
#include "vulkan-device.hpp"

namespace Geg {
	class VulkanCommandBuffers {
	public:
		VulkanCommandBuffers(VulkanDevice* _device, VulkanSwapChain* _swapChain);
		~VulkanCommandBuffers();

		const VkCommandPool& getPool() const { return commandPool; }
		const std::vector<VkCommandBuffer>& getCommandBuffers() const { return commandBuffers; }
		const VkSemaphore& getImageAvailableSemaphore() const { return imageAvailableSemaphore; };
		const VkSemaphore& getRenderFinshedSemaphore() const { return renderFinishedSemaphore; };

	private:
		VulkanDevice* device;
		VulkanSwapChain* swapChain;

		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		void createSemaphores();
	};
}		 // namespace Geg
