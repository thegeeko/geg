#pragma once

#include "swap-chain.hpp"
#include "vulkan-device.hpp"

namespace Geg {
	class VulkanPipelineLayout {
	public:
		VulkanPipelineLayout(VulkanDevice* device, VulkanSwapChain* swapChain);
		~VulkanPipelineLayout();

		const VkPipelineLayout& getPipelineLayout() const { return pipelineLayout; };
		const VkPipelineVertexInputStateCreateInfo& getVertexInputInfo() const {
			return vertexInputInfo;
		};
		const VkPipelineInputAssemblyStateCreateInfo& getInputAssembly() const {
			return inputAssembly;
		};
		const VkPipelineViewportStateCreateInfo& getViewportState() const { return viewportState; };
		const VkPipelineRasterizationStateCreateInfo& getRasterizer() const { return rasterizer; };
		const VkPipelineMultisampleStateCreateInfo& getMultisampling() const {
			return multisampling;
		};
		const VkPipelineColorBlendStateCreateInfo& getColorBlending() const { return colorBlending; };

	private:
		VulkanDevice* device;
		VulkanSwapChain* swapChain;
		VkPipelineLayout pipelineLayout;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		VkViewport viewport{};
		VkRect2D scissor{};
		VkPipelineViewportStateCreateInfo viewportState{};
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		VkPipelineMultisampleStateCreateInfo multisampling{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo colorBlending{};

		VkDynamicState dynamicStates[2] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_LINE_WIDTH};
		VkPipelineDynamicStateCreateInfo dynamicState{};
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

	};
}		 // namespace Geg
