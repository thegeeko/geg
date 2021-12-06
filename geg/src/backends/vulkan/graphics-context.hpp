#pragma once

#include "renderer/graphics-context.hpp"
#include "swap-chain.hpp"
#include "vulkan-device.hpp"
#include "pipeline-layout.hpp"
#include "command-buffers.hpp"

namespace Geg {
	class VulkanGraphicsContext: public GraphicsContext {
	public:
		VulkanGraphicsContext(GLFWwindow* window);
		~VulkanGraphicsContext() override;

		VulkanDevice* device;
		VulkanSwapChain* swapChain;
		VulkanPipelineLayout* pipelineLayout;
		VulkanCommandBuffers* commandBuffers;

		void init() override{};
		void swapBuffers() override{};
	};
}		 // namespace Geg
