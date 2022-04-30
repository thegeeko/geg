#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <cstddef>
#include <vector>

#include "backends/vulkan/graphics-context.hpp"
#include "backends/vulkan/pipeline.hpp"

namespace Geg {
	class VulkanUniform {
	public:
		VulkanUniform(int _setIndex, size_t size, int _instancesCount);
		~VulkanUniform();

		void write(const void* data, size_t size, int instance = 1);
		void bindAtOffset(const VulkanPipeline& pipline, VkCommandBuffer cmd, uint32_t instances) const;

	private:
		VulkanGraphicsContext* context;

		int setIndex = -1;
		size_t fullSize = 0;
		int instancesCount = 0;
		size_t instanceSize = 0;
		VkBuffer bufferHandle = VK_NULL_HANDLE;
		VmaAllocation allocationHandle = VK_NULL_HANDLE;

		VkDescriptorSet descriptorSet;
		VkDescriptorSetLayout descriptorSetLayout;

		size_t padBufferSize(size_t bufferSize) const;

		static int uboCount;
	};
}		 // namespace Geg