#pragma once

#include <vulkan/vulkan.hpp>

#include "backends/vulkan/graphics-context.hpp"
#include "renderer/buffers.hpp"
#include "vk_mem_alloc.h"

namespace Geg {
	class VulkanIndexBuffer: public IndexBuffer {
	public:
		VulkanIndexBuffer(uint32_t* _indecies, uint32_t _size);
		~VulkanIndexBuffer() override;

		void bind() const override{};
		void unbind() const override{};
		void setData(uint32_t* data, uint32_t size) override{};
		uint32_t getCount() const override { return size / sizeof(uint32_t); };

		const VkBuffer& getBufferHandle() const { return bufferHandle; };
		const VmaAllocation& getAllocationHandle() const { return allocationHandle; };

	private:
		VulkanGraphicsContext* context;
		uint32_t size;
		uint32_t* indecies;

		VkBuffer bufferHandle;
		VmaAllocation allocationHandle;

		void createInputBindingDescription();
		void createInputAttributeDescription();
		void createBufferAndAllocateMem();
	};
}		 // namespace Geg
