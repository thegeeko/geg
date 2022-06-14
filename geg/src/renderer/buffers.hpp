#pragma once

#include "shader.hpp"

namespace geg {

	struct BufferElement {
		ShaderDataType type;
		int size;
		uint32_t offset = 0;
		bool normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType _type, bool _normalized = false):
				type(_type), size(ShaderDataTypeSize(type)), normalized(_normalized) {}
		[[nodiscard]] uint8_t getComponentCount() const {
			switch (type) {
				case ShaderDataType::Float: return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;
				case ShaderDataType::Mat3: return 3;		// 3* float3
				case ShaderDataType::Mat4: return 4;		// 4* float4
				case ShaderDataType::Int: return 1;
				case ShaderDataType::Int2: return 2;
				case ShaderDataType::Int3: return 3;
				case ShaderDataType::Int4: return 4;
				case ShaderDataType::Bool: return 1;
				default: {
					GEG_CORE_ERROR("can't get component count of unknown type");
					return 0;
				}
			}
		}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;
		void add(ShaderDataType dataType, bool normalized);
		[[nodiscard]] int getStride() const { return stride; }
		[[nodiscard]] const std::vector<BufferElement> &getElements() const { return elements; }

		std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		std::vector<BufferElement>::iterator end() { return elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

	private:
		int stride = 0;
		std::vector<BufferElement> elements;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;
		virtual void setData(void *data, uint32_t size) = 0;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setLayout(BufferLayout) = 0;
		virtual BufferLayout getLayout() const = 0;
		static VertexBuffer *create(float *vertices, uint32_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;
		virtual void setData(uint32_t *data, uint32_t size) = 0;
		virtual void bind() const = 0;
		virtual unsigned int getCount() const = 0;
		virtual void unbind() const = 0;
		static IndexBuffer *create(uint32_t *indices, uint32_t size);
	};
}		 // namespace geg
