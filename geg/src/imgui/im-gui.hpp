#pragma once

#include <vulkan/vulkan.h>
#include "backends/vulkan/graphics-context.hpp"

namespace Geg {

	static void checkVulkanErrors(VkResult result) {
		GEG_CORE_ASSERT(result == VK_SUCCESS, "IMGUI ERR : ", result);
	};

	class ImGuiLayer{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		
		void init();
		void render();

	private:
		void attach();
		void detach();

		VulkanGraphicsContext* context;
		GLFWwindow* window;

		VkDescriptorPool descriptorPool;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}		 // namespace Geg
