#pragma once

#include "renderer/vertex-array.hpp"
#include "renderer/buffers.hpp"
#include <vector>

namespace Geg {

	class GLVertexArray : public VertexArray {
	public:
		GLVertexArray();
		~GLVertexArray() override;

		void bind() const override;
		void unbind() const override;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

		[[nodiscard]] const std::shared_ptr<VertexBuffer> &getVertexBuffer() const override;
		[[nodiscard]] const std::shared_ptr<IndexBuffer> &getIndexBuffer() const override;

	private:
		unsigned int r_Id = 0;
		unsigned int v_Index = 0; // vertex buffer index
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};
}

