#include "renderer-api.hpp"

#include "pipeline.hpp"

namespace Geg {

	void VulkanRendererAPI::init() {
		context = dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext());
	}

	void VulkanRendererAPI::beginRecording() {
		VkResult result;
		auto commandBuffers = context->commandBuffers->getCommandBuffers();
		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;		// Optional
			beginInfo.pInheritanceInfo = nullptr;		 // Optional

			result = vkBeginCommandBuffer(context->commandBuffers->getCommandBuffers()[i], &beginInfo);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "can't begin command buffer {}", i);
			GEG_CORE_TRACE(" FRAME : Began command buffer {}", i);

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = context->swapChain->getRenderPass();
			renderPassInfo.framebuffer = context->swapChain->getSwapChainFrameBuffers()[i];
			renderPassInfo.renderArea.offset = {0, 0};
			renderPassInfo.renderArea.extent = context->swapChain->getSwapChainExtent();
			VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		}
	}

	void VulkanRendererAPI::endRecording() {
		VkResult result;
		auto commandBuffers = context->commandBuffers->getCommandBuffers();
		for (size_t i = 0; i < commandBuffers.size(); i++) {
			vkCmdEndRenderPass(commandBuffers[i]);
			result = vkEndCommandBuffer(commandBuffers[i]);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't stop recording on command buffer {}", i);
		}
	}

	void VulkanRendererAPI::drawIndexed(const Ref<Pipeline>& _pipeline) {
		GEG_CORE_ASSERT(context, "You should call Renderer::beginScene() first");
		auto pipeline = std::dynamic_pointer_cast<VulkanPipeline>(_pipeline);

		if (a) {
			auto commandBuffers = context->commandBuffers->getCommandBuffers();
			beginRecording();
			for (size_t i = 0; i < commandBuffers.size(); i++) {
				vkCmdBindPipeline(
						commandBuffers[i],
						VK_PIPELINE_BIND_POINT_GRAPHICS,
						pipeline->getPipelineHandle());

				VkBuffer vertexBuffers[] = {pipeline->getVbo()->getBufferHandle()};
				VkDeviceSize offsets[] = {0};
				vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

				vkCmdBindIndexBuffer(
						commandBuffers[i],
						pipeline->getIbo()->getBufferHandle(),
						0,
						VK_INDEX_TYPE_UINT32);

				vkCmdDrawIndexed(
						commandBuffers[i],
						static_cast<uint32_t>(pipeline->getIbo()->getCount()),
						1,
						0,
						0,
						0);
			}

			endRecording();
			a = false;
		}

		vkWaitForFences(
				context->device->getDevice(),
				1,
				&context->commandBuffers->getInFlightFences()[currentFrame],
				VK_TRUE,
				UINT64_MAX);

		uint32_t imageIndex;

		vkAcquireNextImageKHR(
				context->device->getDevice(),
				context->swapChain->getSwapChain(),
				UINT64_MAX,
				context->commandBuffers->getImageAvailableSemaphores()[currentFrame],
				VK_NULL_HANDLE,
				&imageIndex);

		if (context->commandBuffers->getImagesInFlight()[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(
					context->device->getDevice(),
					1,
					&context->commandBuffers->getImagesInFlight()[imageIndex],
					VK_TRUE,
					UINT64_MAX);
		}

		context->commandBuffers->getImagesInFlight()[imageIndex] =
				context->commandBuffers->getInFlightFences()[currentFrame];

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {context->commandBuffers->getImageAvailableSemaphores()[currentFrame]};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &context->commandBuffers->getCommandBuffers()[imageIndex];
		VkSemaphore signalSemaphores[] = {context->commandBuffers->getRenderFinshedSemaphores()[currentFrame]};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(
				context->device->getDevice(),
				1,
				&context->commandBuffers->getInFlightFences()[currentFrame]);

		VkResult result = vkQueueSubmit(context->device->getGraphicsQueue(), 1, &submitInfo, context->commandBuffers->getInFlightFences()[currentFrame]);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't submit draw command buffer");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {context->swapChain->getSwapChain()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;		 // Optional

		vkQueuePresentKHR(context->device->getPresentQueue(), &presentInfo);

		currentFrame = (currentFrame + 1) % context->commandBuffers->MAX_FRAMES_IN_FLIGHT;
	}

}		 // namespace Geg
