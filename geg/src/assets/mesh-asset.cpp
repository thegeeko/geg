#include "mesh-asset.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace Geg {
	MeshAsset::MeshAsset(std::vector<Vertex>& _vertices, std::vector<uint32_t>& _indices):
			vertices(_vertices), indices(_indices) {
		setup();
	}

	void MeshAsset::setup() {
		vbo = Ref<VertexBuffer>(VertexBuffer::create((float*)vertices.data(), vertices.size() * sizeof(Vertex)));
		ibo = Ref<IndexBuffer>(IndexBuffer::create(indices.data(), indices.size() * sizeof indices[0]));
		vbo->setLayout(Vertex::getLayout());
	}
}		 // namespace Geg
