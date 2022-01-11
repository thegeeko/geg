#pragma once

#include <vulkan/vulkan.hpp>

#include "swap-chain.hpp"
#include "vulkan-device.hpp"

namespace Geg {
	class VulkanCommandBuffers {
	public:
		VulkanCommandBuffers(VulkanDevice* _device, VulkanSwapChain* _swapChain);
		~VulkanCommandBuffers();

		const int MAX_FRAMES_IN_FLIGHT = 2;

		VkCommandBuffer beginSingleTimeCommand();
		void endSingleTimeCommand(VkCommandBuffer cb);

		/* void beginCommandBuffers(); */
		/* void endCommandBuffers(); */

		const VkCommandPool& getPool() const { return commandPool; }
		const std::vector<VkCommandBuffer>& getCommandBuffers() const { return commandBuffers; }
		const std::vector<VkSemaphore>& getImageAvailableSemaphores() const { return imageAvailableSemaphores; };
		const std::vector<VkSemaphore>& getRenderFinshedSemaphores() const { return renderFinishedSemaphores; };
		const std::vector<VkFence>& getInFlightFences() const { return inFlightFences; };
		std::vector<VkFence>& getImagesInFlight() { return imagesInFlight; };

	private:
		VulkanDevice* device;
		VulkanSwapChain* swapChain;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		void createSyncObjects();
	};
}		 // namespace Geg
