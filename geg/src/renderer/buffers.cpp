#include "buffers.hpp"

#include "backends/opengl/buffers.hpp"
#include "renderer.hpp"

namespace Geg {

	VertexBuffer *VertexBuffer::create(float *vertices, unsigned int size) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::OpenGL:
				return new GLVertexBuffer(vertices, size);
			case GraphicsAPI::Vulkan:
				GEG_CORE_ERROR("Vulkan not supported atm");
			default:
				GEG_CORE_ERROR("you must use renderer api");
		}
	}

	// @FIXME should make it size
	IndexBuffer *IndexBuffer::create(unsigned int *indices, unsigned int size) {
		switch (Renderer::getAPI()) {
			case GraphicsAPI::OpenGL:
				return new GLIndexBuffer(indices, size);
			case GraphicsAPI::Vulkan:
				GEG_CORE_ERROR("Vulkan not supported atm");
			default:
				GEG_CORE_ERROR("you must use renderer api");
		}
	}

	void BufferLayout::add(ShaderDataType dataType, bool normlized) {
		elements.emplace_back(dataType, normlized);
		stride += ShaderDataTypeSize(dataType);
	}
}		 // namespace Geg
