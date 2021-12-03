#include "graphics-context.hpp"

#include "vulkan-device.hpp"
#include "swap-chain.hpp"

namespace Geg {
	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow *_windowPtr) {
		VulkanDevice vkDevice (_windowPtr);
		VulkanSwapChain vkSwapChain (_windowPtr, &vkDevice);
	}
}		 // namespace Geg
