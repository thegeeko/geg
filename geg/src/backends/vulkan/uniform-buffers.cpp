#include "uniform-buffers.hpp"

#include <vulkan/vulkan_core.h>

#include "backends/vulkan/descriptors.hpp"
#include "backends/vulkan/graphics-context.hpp"
#include "core/logger.hpp"

namespace Geg {
	VulkanUniform::VulkanUniform(int _instancesCount, size_t size) {
		context = dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext());

		instancesCount = _instancesCount;
		originalSize = size;
		fullSize = _instancesCount * padBufferSize(originalSize);

		VkBufferCreateInfo buffInfo{};
		buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffInfo.pNext = nullptr;
		buffInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		buffInfo.size = fullSize;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		VkResult result = vmaCreateBuffer(
				context->allocator,
				&buffInfo,
				&allocInfo,
				&bufferHandle,
				&allocationHandle,
				nullptr);

		GEG_CORE_ASSERT(result == VK_SUCCESS, "cant allocate uniform buffer");
		GEG_CORE_INFO("Global Ubo created");

		VkDescriptorBufferInfo desBuffInfo{};
		desBuffInfo.buffer = bufferHandle;
		desBuffInfo.offset = 0;
		desBuffInfo.range = padBufferSize(originalSize);

		DescriptorBuilder::begin(context->descriptorLayoutCache, context->descriptorsAlloc)
				.bindBuffer(0, &desBuffInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT)
				.build(descriptorSet, descriptorSetLayout);
	}

	VulkanUniform::~VulkanUniform() {
		vmaDestroyBuffer(context->allocator, bufferHandle, allocationHandle);
	}

	/**
	 * @brief
	 *  Copy the data from the @param date to the specfied instance
	 *  will copy to the first instance by default
	 * @param data pointer to the src
	 * @param size
	 * @param instance starts from 1.
	 */
	void VulkanUniform::write(const void* data, size_t size, int instance) {
		char* mappingAddr;
		vmaMapMemory(context->allocator, allocationHandle, (void**)&mappingAddr);

		mappingAddr += (instance - 1) * padBufferSize(originalSize);

		memcpy(mappingAddr, data, size);
		vmaUnmapMemory(context->allocator, allocationHandle);
	}

	void VulkanUniform::bindAtOffset(Ref<VulkanPipeline> pipline, VkCommandBuffer cmd, int instance) {
		uint32_t offset = instance * padBufferSize(originalSize);
		vkCmdBindDescriptorSets(
				cmd,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipline->getLayout(),
				0,
				1,
				&descriptorSet,
				1,
				&offset);
	}

	/**
	 * @brief
	 *  calculate the size of the buffer after the padding
	 * @param bufferSize original buffer size
	 * @return size_t the size after padding
	 */
	size_t VulkanUniform::padBufferSize(size_t bufferSize) {
		// Calculate required alignment based on minimum device offset alignment
		auto gpuProperties = context->device->getGpuProperties();
		size_t minUboAlignment = gpuProperties.limits.minUniformBufferOffsetAlignment;
		size_t alignedSize = bufferSize;

		if (minUboAlignment > 0) {
			alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
		}
		return alignedSize;
	}
}		 // namespace Geg