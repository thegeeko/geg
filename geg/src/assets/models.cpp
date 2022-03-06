#include "models.hpp"

namespace Geg {
	Model::Model(
			std::vector<float>* _vertices,
			std::vector<uint32_t>* _indices,
			BufferLayout _layout) {
		layout = _layout;
		vertices = Scope<std::vector<float>>(_vertices);
		indicies = Scope<std::vector<uint32_t>>(_indices);

		vertexBuffer = Ref<VertexBuffer>(Geg::VertexBuffer::create(vertices->data(), vertices->size()));
		vertexBuffer->setLayout(layout);
		indexBuffer = Ref<IndexBuffer>(Geg::IndexBuffer::create(indicies->data(), indicies->size()));
	}
}		 // namespace Geg