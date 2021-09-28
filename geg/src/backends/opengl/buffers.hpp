#ifndef GEG_GL_BUFFERS_HPP
#define GEG_GL_BUFFERS_HPP

#include "renderer/buffers.hpp"
#include "glad/glad.h"

namespace Geg {
	class GLVertexBuffer : public VertexBuffer {
	public:
		GLVertexBuffer(float* vertices, unsigned int size);
		~GLVertexBuffer() override;

		void setData(void* data, unsigned int size) override;
		void bind() const override;
		void unbind() const override;
		void setLayout(BufferLayout _layout) override;
		[[nodiscard]] BufferLayout getLayout() const override;

	private:
		unsigned int r_Id = 0;
		BufferLayout layout;
	};

	class GLIndexBuffer : public IndexBuffer {
	public:
		GLIndexBuffer(unsigned int* indices, unsigned int _count);
		~GLIndexBuffer() override;

		void setData(unsigned int* data, unsigned int size) override;
		void bind() const override;
		[[nodiscard]] unsigned int getCount() const override;
		void unbind() const override;

	private:
		unsigned int r_Id;
		unsigned int count;
	};
}

#endif //GEG_GL_BUFFERS_HPP
