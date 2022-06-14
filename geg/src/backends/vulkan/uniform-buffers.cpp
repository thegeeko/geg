#include "uniform-buffers.hpp"

#include <vulkan/vulkan_core.h>

#include "backends/vulkan/descriptors.hpp"
#include "backends/vulkan/graphics-context.hpp"
#include "core/logger.hpp"

namespace geg {
	int VulkanUniform::uboCount = 0;

	VulkanUniform::VulkanUniform(int _setIndex, size_t size, int _instancesCount):
			setIndex(_setIndex), instancesCount(_instancesCount), instanceSize(size) {
		context = dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext());

		fullSize = instancesCount * padBufferSize(instanceSize);

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
		desBuffInfo.range = padBufferSize(instanceSize);

		DescriptorBuilder::begin(context->descriptorLayoutCache, context->descriptorsAlloc)
				.bindBuffer(0, &desBuffInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_ALL_GRAPHICS)
				.build(descriptorSet, descriptorSetLayout);

		uboCount++;
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
	 * @param instance starts from 0.
	 */
	void VulkanUniform::write(const void* data, size_t size, int instance) {
		char* mappingAddr;
		vmaMapMemory(context->allocator, allocationHandle, (void**)&mappingAddr);

		mappingAddr += instance * padBufferSize(instanceSize);

		memcpy(mappingAddr, data, size);
		vmaUnmapMemory(context->allocator, allocationHandle);
	}

	void VulkanUniform::bindAtOffset(const VulkanPipeline& pipline, VkCommandBuffer cmd, uint32_t instance) const {
		uint32_t offset = instance * padBufferSize(instanceSize);
		vkCmdBindDescriptorSets(
				cmd,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipline.getLayout(),
				setIndex,
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
	size_t VulkanUniform::padBufferSize(size_t bufferSize) const {
		// Calculate required alignment based on minimum device offset alignment
		auto gpuProperties = context->device->getGpuProperties();
		size_t minUboAlignment = gpuProperties.limits.minUniformBufferOffsetAlignment;
		size_t alignedSize = bufferSize;

		if (minUboAlignment > 0) {
			alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
		}
		return alignedSize;
	}
}		 // namespace geg