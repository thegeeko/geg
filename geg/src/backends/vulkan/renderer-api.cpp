#include "renderer-api.hpp"

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/pipeline.hpp"
#include "backends/vulkan/uniform-buffers.hpp"
#include "imgui.h"
#include "renderer/graphics-context.hpp"
#include "renderer/renderer.hpp"
#include "vendor/imgui/backends/imgui_impl_vulkan.h"

namespace Geg {

	VulkanRendererAPI::VulkanRendererAPI() {
		context = dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext());
		initSyncObjects();
		initGlobalUbo();
	}

	VulkanRendererAPI::~VulkanRendererAPI() {
		deInitSyncObjects();
	}

	void VulkanRendererAPI::initSyncObjects() {
		VkResult result;
		const uint32_t frameBuffersCount = context->swapChain->getSwapChainFrameBuffers().size();

		// Semaphores
		imageAvailableSemaphores.resize(VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		for (size_t i = 0; i < VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT; i++) {
			result = vkCreateSemaphore(context->device->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create a semaphore");

			result = vkCreateSemaphore(context->device->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create a semaphore");
		}

		// Command buffers
		VkCommandBuffer* commandBuffers = new VkCommandBuffer[frameBuffersCount];
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = context->commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = frameBuffersCount;

		result = vkAllocateCommandBuffers(
				context->device->getDevice(),
				&allocInfo,
				commandBuffers);

		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't allocate command buffers")

		// Frames
		frames.resize(frameBuffersCount);

		for (int i = 0; i < frameBuffersCount; i++) {
			VulkanFrame frame;
			frame.index = i;
			frame.globalUboOffset = 0;
			frame.commandBuffer = commandBuffers[i];

			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			result = vkCreateFence(context->device->getDevice(), &fenceInfo, nullptr, &frame.fence);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create a fences")

			frames[i] = frame;
		}

		// busy fences
		busyFences.resize(frameBuffersCount, VK_NULL_HANDLE);
	}

	void VulkanRendererAPI::initGlobalUbo() {
		globalUbo = std::make_unique<VulkanUniform>(VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT, sizeof(GpuSceneData));
		uboData = GpuSceneData{};
	}

	void VulkanRendererAPI::deInitSyncObjects() {
		for (size_t i = 0; i < VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(context->device->getDevice(), renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(context->device->getDevice(), imageAvailableSemaphores[i], nullptr);
		}
		for (const auto& frame : frames) {
			vkDestroyFence(context->device->getDevice(), frame.fence, nullptr);
		}
	}

	void VulkanRendererAPI::startFrame(GpuSceneData _uboData) {
		uboData = _uboData;
		globalUbo->write(&uboData, sizeof(uboData), currentInFlightFrame + 1);

		beginRecording();
	}

	void VulkanRendererAPI::beginRecording() {
		VkResult result;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		result = vkBeginCommandBuffer(frames[nextFrame].commandBuffer, &beginInfo);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't begin a command buffer");
		/* GEG_CORE_TRACE(" FRAME : Began command buffer"); */

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = context->swapChain->getRenderPass();
		renderPassInfo.framebuffer = context->swapChain->getSwapChainFrameBuffers()[frames[nextFrame].index];
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = context->swapChain->getSwapChainExtent();
		VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(frames[nextFrame].commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanRendererAPI::endRecording() {
		VkResult result;

		vkCmdEndRenderPass(frames[nextFrame].commandBuffer);
		result = vkEndCommandBuffer(frames[nextFrame].commandBuffer);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't stop recording on a command buffer");
	}

	void VulkanRendererAPI::oncePerFrame() {
		// render ui
		ImDrawData* drawdata = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(drawdata, frames[nextFrame].commandBuffer);
	}

	void VulkanRendererAPI::drawIndexed(const Ref<Pipeline>& _pipeline) {
		GEG_CORE_ASSERT(context, "You should call Renderer::beginScene() first");
		auto pipeline = std::dynamic_pointer_cast<VulkanPipeline>(_pipeline);

		vkCmdBindPipeline(
				frames[nextFrame].commandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipeline->getPipelineHandle());

		globalUbo->bindAtOffset(
				pipeline,
				frames[nextFrame].commandBuffer,
				currentInFlightFrame);

		VkBuffer vertexBuffers[] = {pipeline->getVbo()->getBufferHandle()};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(frames[nextFrame].commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(
				frames[nextFrame].commandBuffer,
				pipeline->getIbo()->getBufferHandle(),
				0,
				VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(
				frames[nextFrame].commandBuffer,
				static_cast<uint32_t>(pipeline->getIbo()->getCount()),
				1,
				0,
				0,
				0);
	}


	void VulkanRendererAPI::drawIndexed(const Ref<Pipeline>& _pipline, GpuModelData model) {
		GEG_CORE_ASSERT(context, "You should call Renderer::beginScene() first");
		const auto pipeline = std::dynamic_pointer_cast<VulkanPipeline>(_pipline);
		const auto commandBuffer = frames[nextFrame].commandBuffer;

		vkCmdBindPipeline(
				commandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipeline->getPipelineHandle());

		globalUbo->bindAtOffset(
				pipeline,
				commandBuffer,
				currentInFlightFrame);

		vkCmdPushConstants(
				commandBuffer,
				pipeline->getLayout(),
				VK_SHADER_STAGE_VERTEX_BIT,
				0,
				ShaderDataTypeSize(ShaderDataType::Mat4),
				&model.model[0]);

		VkBuffer vertexBuffers[] = {pipeline->getVbo()->getBufferHandle()};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(
				commandBuffer,
				pipeline->getIbo()->getBufferHandle(),
				0,
				VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(
				commandBuffer,
				static_cast<uint32_t>(pipeline->getIbo()->getCount()),
				1,
				0,
				0,
				0);

		oncePerFrame();
	}

	void VulkanRendererAPI::draw(const Ref<Pipeline>& _pipeline) {
		GEG_CORE_ASSERT(context, "You should call Renderer::beginScene() first");
		auto pipeline = std::dynamic_pointer_cast<VulkanPipeline>(_pipeline);

		vkCmdBindPipeline(
				frames[nextFrame].commandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipeline->getPipelineHandle());

		globalUbo->bindAtOffset(
				pipeline,
				frames[nextFrame].commandBuffer,
				currentInFlightFrame);

		VkBuffer vertexBuffers[] = {pipeline->getVbo()->getBufferHandle()};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(frames[nextFrame].commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdDraw(
				frames[nextFrame].commandBuffer,
				static_cast<uint32_t>(pipeline->getVbo()->getVerticesCount()),
				1,
				0,
				0);

	}

	void VulkanRendererAPI::endFrame() {
		endRecording();

		uint32_t imageIndex = 0;

		// Wait for the previous frame
		vkWaitForFences(
				context->device->getDevice(),
				1,
				&frames[imageIndex].fence,
				VK_TRUE,
				UINT64_MAX);

		// Get the next image
		vkAcquireNextImageKHR(
				context->device->getDevice(),
				context->swapChain->getSwapChain(),
				UINT64_MAX,
				imageAvailableSemaphores[currentInFlightFrame],
				VK_NULL_HANDLE,
				&imageIndex);

		// Check if the frame is busy
		if (busyFences[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(
					context->device->getDevice(),
					1,
					&busyFences[imageIndex],
					VK_TRUE,
					UINT64_MAX);
		}

		// Add the frame to the busy list
		busyFences[imageIndex] = frames[imageIndex].fence;

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentInFlightFrame]};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &frames[imageIndex].commandBuffer;
		VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentInFlightFrame]};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(
				context->device->getDevice(),
				1,
				&frames[imageIndex].fence);

		VkResult result = vkQueueSubmit(
				context->device->getGraphicsQueue(),
				1,
				&submitInfo,
				frames[imageIndex].fence);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't submit draw command buffer");
		vkQueueWaitIdle(context->device->getGraphicsQueue());

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {context->swapChain->getSwapChain()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		vkQueuePresentKHR(context->device->getPresentQueue(), &presentInfo);
		vkQueueWaitIdle(context->device->getPresentQueue());

		nextFrame = (imageIndex + 1) % frames.size();
		currentInFlightFrame = (currentInFlightFrame + 1) % VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT;
	}

} // namespace Geg
