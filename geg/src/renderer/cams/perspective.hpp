#pragma once

#include "glm/fwd.hpp"

namespace Geg {
	class Camera {
	public:
		Camera(float fov, float aspectRatio, float near, float far);

		const glm::mat4& getProj() const { return proj; }
		const glm::mat4& getView() const { return view; }
		const glm::mat4& getProjView() const { return projView; }

		void updateView(glm::vec3);
		void updateProjection(float fov, float aspectRatio, float near, float far);

	private:
		glm::mat4 view{1.f};
		glm::mat4 proj{1.f};
		glm::mat4 projView{};
	};
}		 // namespace Geg
