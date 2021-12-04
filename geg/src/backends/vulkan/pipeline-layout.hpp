#pragma once

#include "swap-chain.hpp"
#include "vulkan-device.hpp"

namespace Geg {
	class VulkanPipelineLayout {
	public:
		VulkanPipelineLayout(VulkanDevice* device, VulkanSwapChain* swapChain);
		~VulkanPipelineLayout();

		const VkPipelineLayout getPipelineLayout() const { return pipelineLayout; };
		const VkRenderPass getRenderPass() const { return renderPass; };

	private:
		VulkanDevice* device;
		VulkanSwapChain* swapChain;
		VkPipelineLayout pipelineLayout;
		VkRenderPass renderPass;

		void createRenderPass();
	};
}		 // namespace Geg
