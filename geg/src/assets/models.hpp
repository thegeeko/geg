#pragma once

#include "geg-pch.hpp"
#include "renderer/buffers.hpp"

namespace Geg {
	class Model {
	public:
    Model() = default;
		Model(std::vector<float>* _vertices, std::vector<uint32_t>* _indices, BufferLayout _layout);

		Ref<VertexBuffer> vertexBuffer;
		Scope<std::vector<float>> vertices;

		Ref<IndexBuffer> indexBuffer;
		Scope<std::vector<uint32_t>> indicies;

		BufferLayout layout;
	};
}		 // namespace Geg