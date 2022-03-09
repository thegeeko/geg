#include "mesh.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace Geg {
	Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices):
		vertices(_vertices), indices(_indices) {
		setup();
	}

	void Mesh::setRotation(glm::vec3 newRotation) {
		modelMatrix = glm::mat4{1.f};
		rotation = newRotation;

		// applying rotation
		modelMatrix = glm::rotate(modelMatrix, newRotation.x, {1.f, 0.f, 0.f}); // x
		modelMatrix = glm::rotate(modelMatrix, newRotation.y, {0.f, 1.f, 0.f}); // y
		modelMatrix = glm::rotate(modelMatrix, newRotation.z, {0.f, 0.f, 1.f}); // z

		// applying transform
		modelMatrix = glm::translate(modelMatrix, position);

		// applying scale
		modelMatrix = glm::scale(modelMatrix, scale);
	}

	void Mesh::setScale(glm::vec3 newScale) {
		modelMatrix = glm::mat4{1.f};
		scale = newScale;

		// applying rotation
		modelMatrix = glm::rotate(modelMatrix, rotation.x, {1.f, 0.f, 0.f}); // x
		modelMatrix = glm::rotate(modelMatrix, rotation.y, {0.f, 1.f, 0.f}); // y
		modelMatrix = glm::rotate(modelMatrix, rotation.z, {0.f, 0.f, 1.f}); // z

		// applying transform
		modelMatrix = glm::translate(modelMatrix, position);

		// applying scale
		modelMatrix = glm::scale(modelMatrix, newScale);
	}

	void Mesh::setPosition(glm::vec3 newPosition) {
		modelMatrix = glm::mat4{1.f};
		position = newPosition;

		// applying rotation
		modelMatrix = glm::rotate(modelMatrix, rotation.x, {1.f, 0.f, 0.f}); // x
		modelMatrix = glm::rotate(modelMatrix, rotation.y, {0.f, 1.f, 0.f}); // y
		modelMatrix = glm::rotate(modelMatrix, rotation.z, {0.f, 0.f, 1.f}); // z

		// applying transform
		modelMatrix = glm::translate(modelMatrix, newPosition);

		// applying scale
		modelMatrix = glm::scale(modelMatrix, scale);
	}

	void Mesh::setup() {
		vbo = Ref<VertexBuffer>(VertexBuffer::create((float*)vertices.data(), vertices.size() * sizeof Vertex));
		ibo = Ref<IndexBuffer>(IndexBuffer::create(indices.data(), indices.size() * sizeof indices[0]));
		vbo->setLayout(Vertex::getLayout());
	}
} // namespace Geg
