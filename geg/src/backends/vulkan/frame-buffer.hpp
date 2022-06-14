#pragma once

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

namespace geg {
	class VulkanFrameBufferImage {
	public:
		VulkanFrameBufferImage(VulkanDevice* _device, VmaAllocator alloc, int _width, int _height);
		~VulkanFrameBufferImage();

		const VkImageView& getImageView() const { return imageView; }
		const VkSampler& getSampler() const { return sampler; }
		VkDescriptorImageInfo getDescriptor() const { return descriptor; }

		const VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
		const int width = 0;
		const int height = 0;

	private:
		VulkanDevice* device;
		VmaAllocator vmaAllocator;

		// rendering target
		VkImage image;
		VkSampler sampler;
		VkImageView imageView;
		VmaAllocation imageAllocation;

		VkDescriptorImageInfo descriptor;
	};
}		 // namespace geg
