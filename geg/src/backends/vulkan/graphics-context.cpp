#include "graphics-context.hpp"
#include "backends/vulkan/command-buffers.hpp"


namespace Geg {
	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow *_windowPtr) {
		device = new VulkanDevice(_windowPtr);
		swapChain = new VulkanSwapChain(_windowPtr, device);
		pipelineLayout = new VulkanPipelineLayout(device, swapChain);
		commandBuffers = new VulkanCommandBuffers(device, swapChain);
	}

	VulkanGraphicsContext::~VulkanGraphicsContext(){
		delete commandBuffers;
		delete pipelineLayout;
		delete swapChain;
		delete device;
	}
}		 // namespace Geg
