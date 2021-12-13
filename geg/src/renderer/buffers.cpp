#include "buffers.hpp"

#include "backends/vulkan/vertex-buffer.hpp"
#include "backends/vulkan/index-buffer.hpp"
#include "renderer.hpp"

namespace Geg {

	VertexBuffer *VertexBuffer::create(float *vertices, unsigned int size) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::Vulkan:
				return new VulkanVertexBuffer(vertices, size);
			default:
				GEG_CORE_ERROR("you must use renderer api");
		}
	}

	IndexBuffer *IndexBuffer::create(unsigned int *indices, unsigned int size) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::Vulkan:
				return new VulkanIndexBuffer(indices, size);
			default:
				GEG_CORE_ERROR("you must use renderer api");
		}
	}

	void BufferLayout::add(ShaderDataType dataType, bool normlized) {
		BufferElement newElement(dataType, normlized); 
		newElement.offset = stride; // element offset will be the end of the buffer before him
		elements.push_back(newElement);
		stride += ShaderDataTypeSize(dataType);
	}
}		 // namespace Geg
