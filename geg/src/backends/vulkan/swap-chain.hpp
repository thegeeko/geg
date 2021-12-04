#pragma once

#include <vulkan/vulkan.hpp>

#include "vulkan-device.hpp"

namespace Geg {

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanSwapChain {
	public:
		VulkanSwapChain(GLFWwindow* window, VulkanDevice* _device);
		~VulkanSwapChain();

		const VkExtent2D getSwapChainExtent() const { return swapChainExtent; };
		const VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; };

	private:
		VulkanDevice* device;
		GLFWwindow* window;
		VkSwapchainKHR swapChain;	
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent; 

		void querySwapChainSupport(SwapChainSupportDetails& details);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);	
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void createImageViews();
	};
}		 // namespace Geg
