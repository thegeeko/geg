#pragma once

#include <vulkan/vulkan.h>

#include "backends/vulkan/vulkan-device.hpp"

namespace Geg {
	class DescriptorAllocator {
	public:
		DescriptorAllocator(VulkanDevice* _device);
		~DescriptorAllocator();

		VulkanDevice* device;

		struct PoolSizes {
			std::vector<std::pair<VkDescriptorType, float>> sizes =
					{
							{VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f},
							{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f},
							{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f},
							{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f},
							{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f},
							{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f},
							{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f},
							{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f},
							{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f},
							{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f},
							{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f}};
		};

		void resetPools();
		bool allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout);
		void cleanUp();

	private:
		VkDescriptorPool currentPool{VK_NULL_HANDLE};
		PoolSizes descriptorSizes;
		std::vector<VkDescriptorPool> usedPools;
		std::vector<VkDescriptorPool> freePools;

		VkDescriptorPool grabPool();
	};

	VkDescriptorPool createPool(VkDevice device, const DescriptorAllocator::PoolSizes& poolSizes, int count, VkDescriptorPoolCreateFlags flags);

	// ---------

	class DescriptorLayoutCache {
	public:
		DescriptorLayoutCache(VulkanDevice* _device);
		~DescriptorLayoutCache();

		void cleanUp();
		VkDescriptorSetLayout createDescriptorLayout(VkDescriptorSetLayoutCreateInfo* info);

		struct DescriptorLayoutInfo {
			// good idea to turn this into a inlined array
			std::vector<VkDescriptorSetLayoutBinding> bindings;

			bool operator==(const DescriptorLayoutInfo& other) const;

			size_t hash() const;
		};

	private:
		struct DescriptorLayoutHash {
			std::size_t operator()(const DescriptorLayoutInfo& k) const {
				return k.hash();
			}
		};

		std::unordered_map<DescriptorLayoutInfo, VkDescriptorSetLayout, DescriptorLayoutHash> layoutCache;
		VulkanDevice* device;
	};

	// -------------

	class DescriptorBuilder {
	public:
		static DescriptorBuilder begin(DescriptorLayoutCache* layoutCache, DescriptorAllocator* allocator);

		DescriptorBuilder& bindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);
		DescriptorBuilder& bindImage(uint32_t binding, VkDescriptorImageInfo* imageInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);

		bool build(VkDescriptorSet& set, VkDescriptorSetLayout& layout);
		bool build(VkDescriptorSet& set);

	private:
		std::vector<VkWriteDescriptorSet> writes;
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		DescriptorLayoutCache* cache;
		DescriptorAllocator* alloc;
	};

}		 // namespace Geg
