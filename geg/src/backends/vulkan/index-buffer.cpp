#include "index-buffer.hpp"

namespace Geg {
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* _indecies, uint32_t _size):
			context(dynamic_cast<VulkanGraphicsContext*>(App::get().getWindow().getGraphicsContext())),
			size(_size),
			indecies(_indecies) {
		createBufferAndAllocateMem();
		context->uploadDataToBuffer(indecies, static_cast<size_t>(size), bufferHandle);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer() {
		vmaDestroyBuffer(context->allocator, bufferHandle, allocationHandle);
	}

	void VulkanIndexBuffer::createBufferAndAllocateMem() {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = static_cast<size_t>(size);
		bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
											 VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkResult result = vmaCreateBuffer(
				context->allocator,
				&bufferInfo,
				&allocInfo,
				&bufferHandle,
				&allocationHandle,
				nullptr);

		GEG_CORE_ASSERT(result == VK_SUCCESS, "Can't create index buffer");
		GEG_CORE_INFO("Created index buffer");
	}

}		 // namespace Geg
