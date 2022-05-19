#include "texture.hpp"

#include "stb_image.h"

namespace Geg {
	VulkanTexture::VulkanTexture() {
		GraphicsContext* _context = App::get().getWindow().getGraphicsContext();
		context = dynamic_cast<VulkanGraphicsContext*>(_context);

		std::array<uint8_t, 4> pixels = {1, 1, 1, 1};
		width = 1;
		height = 1;
		size = width * height * 4;

		createImage(pixels.data(), width, height);
		VulkanGraphicsContext::uploadDataToImage(pixels.data(), size, imageHandle, width, height);

		createImageViews();
		createTextureSampler();

		VkDescriptorImageInfo desImageInfo{};
		desImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		desImageInfo.imageView = imageView;
		desImageInfo.sampler = textureSampler;

		DescriptorBuilder::begin(context->descriptorLayoutCache, context->descriptorsAlloc)
				.bindImage(0, &desImageInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL_GRAPHICS)
				.build(descriptorSet, descriptorSetLayout);

		GEG_CORE_WARN("created texture");
	}

	VulkanTexture::VulkanTexture(const std::string& path) {
		GraphicsContext* _context = App::get().getWindow().getGraphicsContext();
		context = dynamic_cast<VulkanGraphicsContext*>(_context);

		int texChannels;
		const uint8_t* pixels = stbi_load(path.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
		GEG_CORE_ASSERT(pixels, "Can't load texture from path {}", path);

		size = width * height * 4;

		createImage(pixels, width, height);
		VulkanGraphicsContext::uploadDataToImage(pixels, size, imageHandle, width, height);
		stbi_image_free((void*)pixels);

		createImageViews();
		createTextureSampler();

		VkDescriptorImageInfo desImageInfo{};
		desImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		desImageInfo.imageView = imageView;
		desImageInfo.sampler = textureSampler;

		DescriptorBuilder::begin(context->descriptorLayoutCache, context->descriptorsAlloc)
				.bindImage(0, &desImageInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_ALL_GRAPHICS)
				.build(descriptorSet, descriptorSetLayout);

		GEG_CORE_WARN("created texture");
	}

	VulkanTexture::~VulkanTexture() {
		vkDestroySampler(context->device->getDevice(), textureSampler, nullptr);
		vkDestroyImageView(context->device->getDevice(), imageView, nullptr);
		vmaDestroyImage(context->allocator, imageHandle, allocationHandle);

		GEG_CORE_WARN("deleted texture");
	}

	void* VulkanTexture::getHandler() {
		return &descriptorSet;
	}

	void VulkanTexture::bindAtOffset(const VulkanPipeline& pipeline, VkCommandBuffer cmd, uint32_t setNumber) const {
		vkCmdBindDescriptorSets(
				cmd,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipeline.getLayout(),
				setNumber,
				1,
				&descriptorSet,
				0,
				nullptr);
	}

	void VulkanTexture::createImage(const uint8_t* imageData, uint32_t width, uint32_t height) {
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		vmaCreateImage(context->allocator, &imageInfo, &allocInfo, &imageHandle, &allocationHandle, nullptr);
	}

	void VulkanTexture::createImageViews() {
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = imageHandle;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(context->device->getDevice(), &viewInfo, nullptr, &imageView);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't create image views for a texture");
	}

	void VulkanTexture::createTextureSampler() {
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 0;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		VkResult result = vkCreateSampler(context->device->getDevice(), &samplerInfo, nullptr, &textureSampler);
		GEG_CORE_ASSERT(result == VK_SUCCESS, "can't create texture sampler");
	}
}		 // namespace Geg