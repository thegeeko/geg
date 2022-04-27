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
		void bindAtOffset(const VulkanPipeline& pipline, VkCommandBuffer cmd, uint32_t instances);

		VkDescriptorSet descriptorSet;

	private:
		VulkanGraphicsContext* context;

		int instancesCount = 0;
		int setIndex = -1;
		size_t fullSize;
		size_t originalSize;
		VkBuffer bufferHandle;
		VmaAllocation allocationHandle;

		VkDescriptorSetLayout descriptorSetLayout;

		size_t padBufferSize(size_t bufferSize);

		static int uboCount;
	};
}		 // namespace Geg