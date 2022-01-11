#include "descriptors.hpp"

namespace Geg {

	// helper function to create pools
	VkDescriptorPool createPool(VkDevice device, const DescriptorAllocator::PoolSizes& poolSizes, int count, VkDescriptorPoolCreateFlags flags) {
		std::vector<VkDescriptorPoolSize> sizes;
		sizes.reserve(poolSizes.sizes.size());
		for (auto sz : poolSizes.sizes) {
			sizes.push_back({sz.first, uint32_t(sz.second * count)});
		}
		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = flags;
		poolInfo.maxSets = count;
		poolInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
		poolInfo.pPoolSizes = sizes.data();

		VkDescriptorPool descriptorPool;
		vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);

		return descriptorPool;
	}

	DescriptorAllocator::DescriptorAllocator(VulkanDevice* _device) {
		device = _device;
	}

	DescriptorAllocator::~DescriptorAllocator() {
		cleanUp();
	}

	void DescriptorAllocator::cleanUp() {
		// delete every pool held
		for (auto p : freePools) {
			vkDestroyDescriptorPool(device->getDevice(), p, nullptr);
		}
		for (auto p : usedPools) {
			vkDestroyDescriptorPool(device->getDevice(), p, nullptr);
		}
	}

	VkDescriptorPool DescriptorAllocator::grabPool() {
		// there are reusable pools availible
		if (freePools.size() > 0) {
			// grab pool from the back of the vector and remove it from there.
			VkDescriptorPool pool = freePools.back();
			freePools.pop_back();
			return pool;
		} else {
			// no pools availible, so create a new one
			return createPool(device->getDevice(), descriptorSizes, 1000, 0);
		}
	}

	bool DescriptorAllocator::allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout) {
		// initialize the currentPool handle if it's null
		if (currentPool == VK_NULL_HANDLE) {
			currentPool = grabPool();
			usedPools.push_back(currentPool);
		}

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.pSetLayouts = &layout;
		allocInfo.descriptorPool = currentPool;
		allocInfo.descriptorSetCount = 1;

		// try to allocate the descriptor set
		VkResult result = vkAllocateDescriptorSets(device->getDevice(), &allocInfo, set);
		bool needReallocate = false;

		switch (result) {
			case VK_SUCCESS:
				// all good, return
				return true;
			case VK_ERROR_FRAGMENTED_POOL:
			case VK_ERROR_OUT_OF_POOL_MEMORY:
				// reallocate pool
				needReallocate = true;
				break;
			default:
				// unrecoverable error
				return false;
		}

		if (needReallocate) {
			// allocate a new pool and retry
			currentPool = grabPool();
			usedPools.push_back(currentPool);

			result = vkAllocateDescriptorSets(device->getDevice(), &allocInfo, set);

			// if it still fails then we have big issues
			if (result == VK_SUCCESS) {
				return true;
			}
		}

		return false;
	}

	void DescriptorAllocator::resetPools() {
		// reset all used pools and add them to the free pools
		for (auto p : usedPools) {
			vkResetDescriptorPool(device->getDevice(), p, 0);
			freePools.push_back(p);
		}

		// clear the used pools, since we've put them all in the free pools
		usedPools.clear();

		// reset the current pool handle back to null
		currentPool = VK_NULL_HANDLE;
	}

	DescriptorLayoutCache::DescriptorLayoutCache(VulkanDevice* _device) {
		device = _device;
	}

	DescriptorLayoutCache::~DescriptorLayoutCache() {
		cleanUp();
	}

	void DescriptorLayoutCache::cleanUp() {
		// delete every descriptor layout held
		for (auto pair : layoutCache) {
			vkDestroyDescriptorSetLayout(device->getDevice(), pair.second, nullptr);
		}
	}

	VkDescriptorSetLayout DescriptorLayoutCache::createDescriptorLayout(VkDescriptorSetLayoutCreateInfo* info) {
		DescriptorLayoutInfo layoutinfo;
		layoutinfo.bindings.reserve(info->bindingCount);
		bool isSorted = true;
		int lastBinding = -1;

		// copy from the direct info struct into our own one
		for (int i = 0; i < info->bindingCount; i++) {
			layoutinfo.bindings.push_back(info->pBindings[i]);

			// check that the bindings are in strict increasing order
			if (info->pBindings[i].binding > lastBinding) {
				lastBinding = info->pBindings[i].binding;
			} else {
				isSorted = false;
			}
		}
		// sort the bindings if they aren't in order
		if (!isSorted) {
			std::sort(layoutinfo.bindings.begin(), layoutinfo.bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b) {
				return a.binding < b.binding;
			});
		}

		// try to grab from cache
		auto it = layoutCache.find(layoutinfo);
		if (it != layoutCache.end()) {
			return (*it).second;
		} else {
			// create a new one (not found)
			VkDescriptorSetLayout layout;
			vkCreateDescriptorSetLayout(device->getDevice(), info, nullptr, &layout);

			// add to cache
			layoutCache[layoutinfo] = layout;
			return layout;
		}
	}

	bool DescriptorLayoutCache::DescriptorLayoutInfo::operator==(const DescriptorLayoutInfo& other) const {
		if (other.bindings.size() != bindings.size()) {
			return false;
		} else {
			// compare each of the bindings is the same. Bindings are sorted so they will match
			for (int i = 0; i < bindings.size(); i++) {
				if (other.bindings[i].binding != bindings[i].binding) {
					return false;
				}
				if (other.bindings[i].descriptorType != bindings[i].descriptorType) {
					return false;
				}
				if (other.bindings[i].descriptorCount != bindings[i].descriptorCount) {
					return false;
				}
				if (other.bindings[i].stageFlags != bindings[i].stageFlags) {
					return false;
				}
			}
			return true;
		}
	}

	size_t DescriptorLayoutCache::DescriptorLayoutInfo::hash() const {
		using std::hash;
		using std::size_t;

		size_t result = hash<size_t>()(bindings.size());

		for (const VkDescriptorSetLayoutBinding& b : bindings) {
			// pack the binding data into a single int64. Not fully correct but it's ok
			size_t binding_hash = b.binding | b.descriptorType << 8 | b.descriptorCount << 16 | b.stageFlags << 24;

			// shuffle the packed binding data and xor it with the main hash
			result ^= hash<size_t>()(binding_hash);
		}

		return result;
	}

	DescriptorBuilder DescriptorBuilder::begin(DescriptorLayoutCache* layoutCache, DescriptorAllocator* allocator) {
		DescriptorBuilder builder;

		builder.cache = layoutCache;
		builder.alloc = allocator;
		return builder;
	}

	DescriptorBuilder& DescriptorBuilder::bindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags) {
		// create the descriptor binding for the layout
		VkDescriptorSetLayoutBinding newBinding{};

		newBinding.descriptorCount = 1;
		newBinding.descriptorType = type;
		newBinding.pImmutableSamplers = nullptr;
		newBinding.stageFlags = stageFlags;
		newBinding.binding = binding;

		bindings.push_back(newBinding);

		// create the descriptor write
		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;

		newWrite.descriptorCount = 1;
		newWrite.descriptorType = type;
		newWrite.pBufferInfo = bufferInfo;
		newWrite.dstBinding = binding;

		writes.push_back(newWrite);
		return *this;
	}

	bool DescriptorBuilder::build(VkDescriptorSet& set, VkDescriptorSetLayout& layout) {
		// build layout first
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;

		layoutInfo.pBindings = bindings.data();
		layoutInfo.bindingCount = bindings.size();

		layout = cache->createDescriptorLayout(&layoutInfo);

		// allocate descriptor
		bool success = alloc->allocate(&set, layout);
		if (!success) { return false; };

		// write descriptor
		for (VkWriteDescriptorSet& w : writes) {
			w.dstSet = set;
		}

		vkUpdateDescriptorSets(alloc->device->getDevice(), writes.size(), writes.data(), 0, nullptr);

		return true;
	}

}		 // namespace Geg
