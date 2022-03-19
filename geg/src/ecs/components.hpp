#pragma once

#include <glm/ext/matrix_transform.hpp>

#include "geg-pch.hpp"
#include "assets/mesh-asset.hpp"
#include "assets/shader-asset.hpp"
#include "assets/loaders/mesh-loader.hpp"
#include "assets/loaders/shader-loader.hpp"

namespace Geg {
	struct NameComponent {
		std::string name;

		NameComponent() = default;

		NameComponent(const std::string& _name) :
			name(_name) {}
	};

	struct TransformComponent {
		glm::vec3 translation{0.f};
		glm::vec3 scale{1.f};
		glm::vec3 rotation{0.f};

	private:
		glm::mat4 transform{1.f};

	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		TransformComponent(const glm::vec3& _translation) :
			translation(_translation) {}

		glm::mat4& getTransform() {
			// applying rotation
			glm::mat4 modelMatrix = glm::translate(glm::mat4{1}, translation);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), {1.f, 0.f, 0.f}); // x
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), {0.f, 1.f, 0.f}); // y
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), {0.f, 0.f, 1.f}); // z
			modelMatrix = glm::scale(modelMatrix, scale);

			transform = modelMatrix;
			return transform;
		}

		operator glm::mat4() { return getTransform(); }
		operator const glm::mat4() { return getTransform(); }
	};

	struct MeshComponent {
		MeshAsset mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default ;

		MeshComponent(MeshAsset _mesh) :
			mesh(std::move(_mesh)) {}

		MeshComponent(const std::string& path) {
			mesh = MeshLoader::loadModel(path);
		}
	};

	struct MeshRendererComponent {
		ShaderAsset shader;

		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default ;

		MeshRendererComponent(ShaderAsset _shader) :
			shader(std::move(_shader)) {};

		MeshRendererComponent(const std::string& path) {
			shader = ShaderLoader::load(path);
		}
	};
} // namespace Geg
