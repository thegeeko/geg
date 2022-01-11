#pragma once

#include <vulkan/vulkan.hpp>

#include "backends/vulkan/graphics-context.hpp"
#include "renderer/buffers.hpp"
#include "vk_mem_alloc.h"

namespace Geg {
	class VulkanVertexBuffer: public VertexBuffer {
	public:
		VulkanVertexBuffer(float* vertices, unsigned int size);
		~VulkanVertexBuffer() override;

		void bind() const override{};
		void unbind() const override{};
		BufferLayout getLayout() const override { return layout; };
		void setLayout(BufferLayout) override;
		void setData(void* data, unsigned int size) override{};
		const uint32_t getVerticesCount() const { return size / sizeof(float); };
		const VkBuffer& getBufferHandle() const { return bufferHandle; };
		const VmaAllocation& getAllocationHandle() const { return allocationHandle; };
		const std::vector<VkVertexInputAttributeDescription>& getAttribDesc() const {
			return attributeDescriptions;
		}
		const VkVertexInputBindingDescription& getBindingDesc() const {
			return bindingDescription;
		};

	private:
		VulkanGraphicsContext* context;
		unsigned int size;
		float* vertices;

		VkVertexInputBindingDescription bindingDescription{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
		BufferLayout layout;
		VkBuffer bufferHandle;
		VmaAllocation allocationHandle;

		void createInputBindingDescription();
		void createInputAttributeDescription();
		void createBufferAndAllocateMem();
	};
}		 // namespace Geg
