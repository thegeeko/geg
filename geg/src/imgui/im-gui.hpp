#pragma once

#include <vulkan/vulkan.h>
#include "backends/vulkan/graphics-context.hpp"

namespace Geg {

	static void checkVulkanErrors(VkResult result) {
		GEG_CORE_ASSERT(result == VK_SUCCESS, "IMGUI ERR : ", result);
	};

	class ImGuiLayer: public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;
		void onAttach() override;
		void onDetach() override;
		void onUiUpdate() override;

		void begin();
		void end();

	private:
		VulkanGraphicsContext* context;
		GLFWwindow* window;

		VkDescriptorPool descriptorPool;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}		 // namespace Geg
