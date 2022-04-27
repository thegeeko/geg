#pragma once

#include "assets/loaders/mesh-loader.hpp"
#include "assets/loaders/shader-loader.hpp"
#include "assets/mesh-asset.hpp"
#include "assets/shader-asset.hpp"
#include "geg-pch.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"

namespace Geg {
	struct NameComponent {
		std::string name;

		NameComponent() = default;
		NameComponent(const std::string& _name):
				name(_name) {}
	};

	struct TransformComponent {
		glm::vec3 translation{0.f};
		glm::vec3 scale{1.f};
		glm::vec3 rotation{0.f};

	private:
		glm::mat4 transform{1.f};
		glm::mat4 norm{1.f};

	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		TransformComponent(const glm::vec3& _translation):
				translation(_translation) {}

		glm::mat4& getTransform() {
			// applying rotation
			glm::mat4 modelMatrix = glm::translate(glm::mat4{1}, translation);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), {1.f, 0.f, 0.f});		// x
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), {0.f, 1.f, 0.f});		// y
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), {0.f, 0.f, 1.f});		// z
			modelMatrix = glm::scale(modelMatrix, scale);

			transform = modelMatrix;
			return transform;
		}

		glm::mat4& getNormMat() {
			// applying rotation
			glm::mat4 rotationMat{1.0};
			rotationMat = glm::rotate(rotationMat, glm::radians(rotation.x), {1.f, 0.f, 0.f});		// x
			rotationMat = glm::rotate(rotationMat, glm::radians(rotation.y), {0.f, 1.f, 0.f});		// y
			rotationMat = glm::rotate(rotationMat, glm::radians(rotation.z), {0.f, 0.f, 1.f});		// z

			glm::mat4 scaleMat{1.0};
			rotationMat = glm::scale(rotationMat, scale);

			norm = rotationMat * glm::inverse(scaleMat);
			return norm;
		}

		operator glm::mat4() { return getTransform(); }
		operator const glm::mat4() { return getTransform(); }
	};

	struct MeshComponent {
		MeshAsset mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;

		MeshComponent(MeshAsset _mesh):
				mesh(std::move(_mesh)) {}

		MeshComponent(const std::string& path) {
			mesh = MeshLoader::loadModel(path);
		}
	};

	struct MeshRendererComponent {
		ShaderAsset shader;
		glm::vec4 color; // w is sharpness

		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;

		MeshRendererComponent(ShaderAsset _shader):
				shader(std::move(_shader)){};

		MeshRendererComponent(const std::string& path) {
			shader = ShaderLoader::load(path);
		}
	};

	struct GlobalLightComponent {
		glm::vec4 dir;
		glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
		glm::vec4 ambient = glm::vec4(1.f, 1.f, 1.f, 0.1f);

		GlobalLightComponent() = default;
		GlobalLightComponent(const GlobalLightComponent&) = default;
		GlobalLightComponent(glm::vec4 _dir) {
			dir = _dir;
		};
	};

	struct PointLightComponent {
		glm::vec4 color;		// w is intensity

		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent(glm::vec4 _color) {
			color = _color;
		};
	};
}		 // namespace Geg
