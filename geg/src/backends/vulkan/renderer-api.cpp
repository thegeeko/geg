#include "renderer-api.hpp"

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/pipeline.hpp"
#include "backends/vulkan/uniform-buffers.hpp"
#include "imgui.h"
#include "index-buffer.hpp"
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
		clearPipelineCache();
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
		// there's no leak here cuz the command buffers pool will
		// automaticaly free all command buffers allocated from it
		auto* commandBuffers = new VkCommandBuffer[frameBuffersCount];
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

		int i = 0;
		for (auto& frame : frames) {
			frame.index = i;
			frame.commandBuffer = commandBuffers[i];

			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			result = vkCreateFence(context->device->getDevice(), &fenceInfo, nullptr, &frame.fence);
			GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create a fences")

			i++;
		}

		// busy fences
		busyFences.resize(frameBuffersCount, VK_NULL_HANDLE);
	}

	void VulkanRendererAPI::initGlobalUbo() {
		globalUbo = std::make_unique<VulkanUniform>(0, sizeof(GpuSceneData), VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT);
		globalUboData = GpuSceneData{};
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

	void VulkanRendererAPI::startFrame(const GpuSceneData& _uboData) {
		globalUboData = _uboData;
		globalUbo->write(&globalUboData, sizeof(globalUboData), currentInFlightFrame);

		beginRecording();
	}

	void VulkanRendererAPI::beginRecording() {
		VkResult result;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;		// Optional
		beginInfo.pInheritanceInfo = nullptr;		 // Optional

		result = vkBeginCommandBuffer(frames[nextFrame].commandBuffer, &beginInfo);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't begin a command buffer");
		/* GEG_CORE_TRACE(" FRAME : Began command buffer"); */

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
		clearValues[1].depthStencil = {1.0f, 0};

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = context->swapChain->getRenderPass();
		renderPassInfo.framebuffer = context->swapChain->getSwapChainFrameBuffers()[frames[nextFrame].index];
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = context->swapChain->getSwapChainExtent();
		renderPassInfo.clearValueCount = clearValues.size();
		renderPassInfo.pClearValues = clearValues.data();

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

	void VulkanRendererAPI::drawMesh(const MeshRenderData& meshData) {
		GEG_CORE_ASSERT(context, "You should call Renderer::beginScene() first");

		// create a pipeline for the mesh
		size_t hash = meshData.rendererC->shader.shaderHash;
		if (pipelineCache.find(hash) == pipelineCache.end()) {
			pipelineCache[hash] = std::make_unique<VulkanPipeline>(meshData.meshC->mesh.vbo, meshData.rendererC->shader.shader);
		}

		// object UBO
		if (objectUbo.find(meshData.id) == objectUbo.end())
			objectUbo[meshData.id] = std::make_unique<VulkanUniform>(1, sizeof(GpuModelData), VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT);

		objectUboData.color = meshData.rendererC->color;
		objectUboData.useTex = meshData.rendererC->useTex;
		objectUbo[meshData.id]->write(&objectUboData, sizeof(objectUboData), currentInFlightFrame);

		// setup the vars
		const VulkanPipeline& pipeline = *pipelineCache[hash];
		const auto commandBuffer = frames[nextFrame].commandBuffer;
		const auto vbo = std::dynamic_pointer_cast<VulkanVertexBuffer>(meshData.meshC->mesh.vbo);
		const auto ibo = std::dynamic_pointer_cast<VulkanIndexBuffer>(meshData.meshC->mesh.ibo);

		// record commands
		vkCmdBindPipeline(
				commandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipeline.getPipelineHandle());

		globalUbo->bindAtOffset(
				pipeline,
				commandBuffer,
				currentInFlightFrame);

		objectUbo[meshData.id]->bindAtOffset(
				pipeline,
				commandBuffer,
				currentInFlightFrame);

		if (meshData.rendererC->useTex) {
			const auto tex = std::dynamic_pointer_cast<VulkanTexture>(meshData.rendererC->tex);
			tex->bindAtOffset(pipeline, commandBuffer, 2);
		} else {
			dummyTexture.bindAtOffset(pipeline, commandBuffer, 2);
		}

		vkCmdPushConstants(
				commandBuffer,
				pipeline.getLayout(),
				VK_SHADER_STAGE_ALL_GRAPHICS,
				0,
				ShaderDataTypeSize(ShaderDataType::Mat4),
				&meshData.transformC->getTransform());

		vkCmdPushConstants(
				commandBuffer,
				pipeline.getLayout(),
				VK_SHADER_STAGE_ALL_GRAPHICS,
				ShaderDataTypeSize(ShaderDataType::Mat4),
				ShaderDataTypeSize(ShaderDataType::Mat4),
				&meshData.transformC->getNormMat());

		VkBuffer vertexBuffers[] = {vbo->getBufferHandle()};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(
				commandBuffer,
				ibo->getBufferHandle(),
				0,
				VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(
				commandBuffer,
				ibo->getCount(),
				1,
				0,
				0,
				0);
	}

	void VulkanRendererAPI::clearPipelineCache() {
		pipelineCache.clear();
	};

	void VulkanRendererAPI::endFrame() {
		oncePerFrame();
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

		// Check if another frame is using the image
		if (busyFences[imageIndex] != VK_NULL_HANDLE) {
			// wait for that frame to finish
			vkWaitForFences(
					context->device->getDevice(),
					1,
					&busyFences[imageIndex],
					VK_TRUE,
					UINT64_MAX);
		}

		// Add the mark the image with the fences of the current frame
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

		// submit the queue with the current frame's fence
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
		presentInfo.pResults = nullptr;		 // Optional

		vkQueuePresentKHR(context->device->getPresentQueue(), &presentInfo);
		vkQueueWaitIdle(context->device->getPresentQueue());

		// setup for the next frame
		nextFrame = (imageIndex + 1) % frames.size();
		currentInFlightFrame = (currentInFlightFrame + 1) % VulkanGraphicsContext::MAX_FRAMES_IN_FLIGHT;
	}

}		 // namespace Geg
