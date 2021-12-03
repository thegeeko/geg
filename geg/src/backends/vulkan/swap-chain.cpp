#include "swap-chain.hpp"
#include <vulkan/vulkan_core.h>

#include <algorithm>
#include <cstdint>

namespace Geg {

	VulkanSwapChain::VulkanSwapChain(GLFWwindow* _window, VulkanDevice* _device):
			device(_device), window(_window){
		SwapChainSupportDetails supportDetails;
		querySwapChainSupport(supportDetails);
		GEG_CORE_ASSERT(!supportDetails.formats.empty() && !supportDetails.presentModes.empty(), "Swap chain formats and present modes arn't supported");

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(supportDetails.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(supportDetails.presentModes);
		VkExtent2D extent = chooseSwapExtent(supportDetails.capabilities);
		unsigned int imageCount = supportDetails.capabilities.minImageCount + 1;
		if (supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount) {
			imageCount = supportDetails.capabilities.maxImageCount;
		}

		QueueFamilyIndices indices; 
		device->findQueueFamilies(device->getPhysicalDevice(), indices);
		uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = device->getSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.preTransform = supportDetails.capabilities.currentTransform;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		VkResult result = vkCreateSwapchainKHR(device->getDevice(), &createInfo, nullptr, &swapChain);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't craete swap chain error code : ", result);

		vkGetSwapchainImagesKHR(device->getDevice(), swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device->getDevice(), swapChain, &imageCount, swapChainImages.data());
		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		createImageViews();

		GEG_CORE_INFO("swap chain created");
	}

	VulkanSwapChain::~VulkanSwapChain() {
		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(device->getDevice(), imageView, nullptr);
		}
		vkDestroySwapchainKHR(device->getDevice(), swapChain, nullptr);	
	}

	void VulkanSwapChain::querySwapChainSupport(SwapChainSupportDetails& details) {
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->getPhysicalDevice(), device->getSurface(), &details.capabilities);

		unsigned int formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device->getPhysicalDevice(), device->getSurface(), &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device->getPhysicalDevice(), device->getSurface(), &formatCount, details.formats.data());
		}

		unsigned int presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device->getPhysicalDevice(), device->getSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device->getPhysicalDevice(), device->getSurface(), &presentModeCount, details.presentModes.data());
		}
	}

	VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return availableFormats[0];
	}

	VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		} else {
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
	
			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};
	
			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	
			return actualExtent;
		}
	}

	void VulkanSwapChain::createImageViews() {
		for (unsigned int i = 0; i < swapChainImages.size(); i++) {

			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];	
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainImageFormat;	
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VkResult result = vkCreateImageView(device->getDevice(), &createInfo, nullptr, &swapChainImageViews[i]);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "can't create image view for image index {} in the swap chain images", i);
		}	
	}

}		 // namespace Geg
