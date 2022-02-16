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
		VulkanUniform(int instances, size_t size);
		~VulkanUniform();

		void write(const void* data, size_t size, int instance = 1);
		void bindAtOffset(Ref<VulkanPipeline> pipline, VkCommandBuffer cmd, int instances);

	private:
		VulkanGraphicsContext* context;

		int instancesCount = 0;
		size_t fullSize;
		size_t originalSize;
		VkBuffer bufferHandle;
		VmaAllocation allocationHandle;

		VkDescriptorSet descriptorSet;
		VkDescriptorSetLayout descriptorSetLayout;

		size_t padBufferSize(size_t bufferSize);
	};
}		 // namespace Geg