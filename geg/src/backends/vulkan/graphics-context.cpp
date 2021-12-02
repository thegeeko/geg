#include "graphics-context.hpp"

namespace Geg {
	VulkanGraphicsContext::VulkanGraphicsContext(GLFWwindow *_windowPtr) {
		VulkanDevice vkDevice (_windowPtr);
	}
}		 // namespace Geg
