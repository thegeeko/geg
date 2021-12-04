#include "graphics-context.hpp"


namespace Geg {
	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow *_windowPtr) {
		device = new VulkanDevice(_windowPtr);
		swapChain = new VulkanSwapChain(_windowPtr, device);
		pipelineLayout = new VulkanPipelineLayout(device, swapChain);
	}

	VulkanGraphicsContext::~VulkanGraphicsContext(){
		delete pipelineLayout;
		delete swapChain;
		delete device;
	}
}		 // namespace Geg
