#pragma once

#include "backends/vulkan/pipeline.hpp"
#include "renderer/texture.hpp"

namespace Geg {
	class VulkanTexture: public Texture {
	public:
		VulkanTexture();		// creates dummy texture
		VulkanTexture(const std::string& path);
		~VulkanTexture() override;

		void* getHandler() override;
		void bindAtOffset(const VulkanPipeline& pipeline, VkCommandBuffer cmd, uint32_t setNumber) const;

	private:
		VulkanGraphicsContext* context;

		int width = 0;
		int height = 0;
		size_t size = 0;

		VkImage imageHandle;
		VmaAllocation allocationHandle;

		VkImageView imageView;
		VkSampler textureSampler;

		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorSet descriptorSet;

		void createImage(const uint8_t* imageData, uint32_t width, uint32_t height);
		void createImageViews();
		void createTextureSampler();
	};
}		 // namespace Geg