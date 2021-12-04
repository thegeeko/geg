#pragma once

#include "renderer/graphics-context.hpp"
#include "swap-chain.hpp"
#include "vulkan-device.hpp"
#include "pipeline-layout.hpp"

namespace Geg {
	class VulkanGraphicsContext: public GraphicsContext {
	public:
		VulkanGraphicsContext(GLFWwindow* window);
		virtual ~VulkanGraphicsContext();

		VulkanDevice* device;
		VulkanSwapChain* swapChain;
		VulkanPipelineLayout* pipelineLayout;

		void init() override{};
		void swapBuffers() override{};
	};
}		 // namespace Geg
