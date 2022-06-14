#pragma once

#include <vulkan/vulkan.h>
#include "backends/vulkan/graphics-context.hpp"

namespace geg {

	static void checkVulkanErrors(VkResult result) {
		GEG_CORE_ASSERT(result == VK_SUCCESS, "IMGUI ERR : ", result);
	};

	class ImGuiLayer{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		void initFrame();
		void render();

	private:
		void attach();
		void detach();
		void initDescriptorPool();
		void initRenderPass();

		VulkanGraphicsContext* context;
		GLFWwindow* window;

		VkDescriptorPool descriptorPool;
		VkRenderPass renderPass;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}		 // namespace geg
