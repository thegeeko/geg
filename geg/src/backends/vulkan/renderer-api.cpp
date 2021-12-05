#include "renderer-api.hpp"

#include <vulkan/vulkan_core.h>

#include <memory>

namespace Geg {
	void VulkanRendererAPI::submit(const std::shared_ptr<Shader>& shader) {
		vulkanShader = std::dynamic_pointer_cast<VulkanShader>(shader);
		vulkanContext = dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext());

		createPipeline(vulkanShader, vulkanContext);
		GEG_CORE_TRACE(" FRAME : Created pipeline");

		VkResult result;
		auto commandBuffers = vulkanContext->commandBuffers->getCommandBuffers();
		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;		// Optional
			beginInfo.pInheritanceInfo = nullptr;		 // Optional

			result = vkBeginCommandBuffer(vulkanContext->commandBuffers->getCommandBuffers()[i], &beginInfo);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "can't begin command buffer {}", i);
			GEG_CORE_TRACE(" FRAME : Began command buffer {}", i);

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vulkanContext->swapChain->getRenderPass();
			renderPassInfo.framebuffer = vulkanContext->swapChain->getSwapChainFrameBuffers()[i];
			renderPassInfo.renderArea.offset = {0, 0};
			renderPassInfo.renderArea.extent = vulkanContext->swapChain->getSwapChainExtent();
			VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			GEG_CORE_TRACE(" FRAME : Recoreded starting render passes {}", i);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, currentPipeline);
			GEG_CORE_TRACE(" FRAME : Recoreded binding the pipeline {}", i);

			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
			GEG_CORE_TRACE(" FRAME : Recoreded draw command {}", i);

			vkCmdEndRenderPass(commandBuffers[i]);
			GEG_CORE_TRACE(" FRAME : Recoreded ending the render pass {}", i);

			result = vkEndCommandBuffer(commandBuffers[i]);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't stop recording on command buffer {}", i);
			GEG_CORE_TRACE(" FRAME : Done recording {}", i);
		}
	};

	void VulkanRendererAPI::drawIndexed() {
		uint32_t imageIndex;
		vkAcquireNextImageKHR(
				vulkanContext->device->getDevice(),
				vulkanContext->swapChain->getSwapChain(),
				UINT64_MAX,
				vulkanContext->commandBuffers->getImageAvailableSemaphore(),
				VK_NULL_HANDLE,
				&imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {vulkanContext->commandBuffers->getImageAvailableSemaphore()};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vulkanContext->commandBuffers->getCommandBuffers()[imageIndex];
		VkSemaphore signalSemaphores[] = {vulkanContext->commandBuffers->getRenderFinshedSemaphore()};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VkResult result = vkQueueSubmit(vulkanContext->device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't submit draw command buffer");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {vulkanContext->swapChain->getSwapChain()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		vkQueuePresentKHR(vulkanContext->device->getPresentQueue(), &presentInfo);
	}


	void VulkanRendererAPI::createPipeline(const std::shared_ptr<VulkanShader>& shader, VulkanGraphicsContext* context) {

	VkPipelineShaderStageCreateInfo shaderStages[] = {shader->getStages()[0], shader->getStages()[1]};

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = shader->getStages().size();
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &context->pipelineLayout->getVertexInputInfo();
		pipelineInfo.pInputAssemblyState = &context->pipelineLayout->getInputAssembly();
		pipelineInfo.pViewportState = &context->pipelineLayout->getViewportState();
		pipelineInfo.pRasterizationState = &context->pipelineLayout->getRasterizer();
		pipelineInfo.pMultisampleState = &context->pipelineLayout->getMultisampling();
		pipelineInfo.pDepthStencilState = nullptr;		// Optional
		pipelineInfo.pColorBlendState = &context->pipelineLayout->getColorBlending();
		pipelineInfo.pDynamicState = nullptr;		 // Optional
		pipelineInfo.layout = context->pipelineLayout->getPipelineLayout();
		pipelineInfo.renderPass = context->swapChain->getRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;		 // Optional
		pipelineInfo.basePipelineIndex = -1;		// Optional

		VkResult result = vkCreateGraphicsPipelines(
				context->device->getDevice(),
				VK_NULL_HANDLE,
				1,
				&pipelineInfo,
				nullptr,
				&currentPipeline);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't create pipeline");
	};

	void VulkanRendererAPI::destroyPipeline(VulkanGraphicsContext* context) {
		vkDestroyPipeline(context->device->getDevice(), currentPipeline, nullptr);
	};

}		 // namespace Geg
