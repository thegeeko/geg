#include "frame-buffer.hpp"
#include <vulkan/vulkan_core.h>

namespace geg {
	VulkanFrameBufferImage::VulkanFrameBufferImage(
		VulkanDevice* _device, VmaAllocator alloc, int _width, int _height)
		: device(_device), vmaAllocator(alloc), width(_width), height(_height) {

		VkImageCreateInfo imageInfo{};
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.format = imageFormat;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		// We will sample directly from the color attachment
		imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		VkResult result =
			vmaCreateImage(vmaAllocator, &imageInfo, &allocInfo, &image, &imageAllocation, nullptr);

		GEG_CORE_ASSERT(result == VK_SUCCESS, "error creating frame buffer image");

		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = imageFormat;
		imageViewInfo.subresourceRange = {};
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;
		imageViewInfo.image = image;
		vkCreateImageView(device->getDevice(), &imageViewInfo, nullptr, &imageView);

		// Create sampler to sample from the attachment in the fragment shader
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeV = samplerInfo.addressModeU;
		samplerInfo.addressModeW = samplerInfo.addressModeU;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 1.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		result = vkCreateSampler(device->getDevice(), &samplerInfo, nullptr, &sampler);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "Failed to create sampler for a framebuffer");

		VkDescriptorImageInfo desImageInfo{};
		desImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		desImageInfo.imageView = imageView;
		desImageInfo.sampler = sampler;
	}

	VulkanFrameBufferImage::~VulkanFrameBufferImage() {
		vkDestroySampler(device->getDevice(), sampler, nullptr);
		vkDestroyImageView(device->getDevice(), imageView, nullptr);
		vmaDestroyImage(vmaAllocator, image, imageAllocation);
	}

}		 // namespace geg
