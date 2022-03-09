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

	class Mesh {
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

		// should change this from shared pointer ..
		Ref<VertexBuffer> vbo;
		Ref<IndexBuffer> ibo;
		Ref<Shader> material;

		void setRotation(glm::vec3 newRotation);
		void setScale(glm::vec3 newScale);
		void setPosition(glm::vec3 newPosition);

		[[nodiscard]] glm::vec3 getPosition() const { return position; }
		[[nodiscard]] glm::vec3 getScale() const { return scale; }
		[[nodiscard]] glm::vec3 getRotation() const { return rotation; }
		[[nodiscard]] const glm::mat4& getModelMatrix() const { return modelMatrix; };

	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		// model transform
		glm::mat4 modelMatrix{1.f};
		glm::vec3 position{0.f};
		glm::vec3 scale{1.f};
		glm::vec3 rotation{0.f};

		// setup the vbo and ibo
		void setup();
	};
} // namespace Geg
