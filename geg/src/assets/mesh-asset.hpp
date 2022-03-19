#pragma once

#include "geg-pch.hpp"
#include "renderer/buffers.hpp"

namespace Geg {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;

		static BufferLayout getLayout() {
			BufferLayout layout{};
			layout.add(ShaderDataType::Float3, false); // position
			layout.add(ShaderDataType::Float3, false); // normals
			layout.add(ShaderDataType::Float2, false); // texture coordinates
			return layout;
		}
	};

	class MeshAsset {
	public:
		MeshAsset() = default;
		MeshAsset(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

		// should change this from shared pointer ..
		Ref<VertexBuffer> vbo;
		Ref<IndexBuffer> ibo;

	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		// setup the vbo and ibo
		void setup();
	};
} // namespace Geg
