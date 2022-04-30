#pragma once

#include <vulkan/vulkan.hpp>

#include "vk_mem_alloc.h"
#include "vulkan-device.hpp"

namespace Geg {

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanSwapChain {
	public:
		VulkanSwapChain(GLFWwindow* window, VulkanDevice* _device, VmaAllocator alloc);
		~VulkanSwapChain();

		[[nodiscard]] const VkSwapchainKHR& getSwapChain() const { return swapChain; }
		[[nodiscard]] const VkRenderPass& getRenderPass() const { return renderPass; }
		[[nodiscard]] const VkExtent2D& getSwapChainExtent() const { return swapChainExtent; }
		[[nodiscard]] const VkFormat& getSwapChainImageFormat() const { return swapChainImageFormat; }
		[[nodiscard]] const std::vector<VkFramebuffer>& getSwapChainFrameBuffers() const { return swapChainFramebuffers; }
		[[nodiscard]] const std::vector<VkImage>& getSwapChainImages() const { return swapChainImages; }

		void handleResize();

	private:
		VulkanDevice* device;
		GLFWwindow* window;
		VmaAllocator vmaAllocator;

		VkSwapchainKHR swapChain;

		VkFormat swapChainImageFormat;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkImage depthImage;
		VkFormat depthImageFormat;
		VkImageView depthImageView;
		VmaAllocation depthAllocation;

		VkExtent2D swapChainExtent;
		VkRenderPass renderPass;

		void querySwapChainSupport(SwapChainSupportDetails& details) const;
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void createFramebuffers();
		void createDepthResources();
		void cleanUp();
	};
}		 // namespace Geg
