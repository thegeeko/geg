#pragma once

#include "glm/matrix.hpp"

namespace Geg {
	class Camera {
	public:
		Camera(float fov, float aspectRatio, float near, float far);
		const glm::mat4& getProjView() const { return projView; }
		
		glm::mat4 view{1.f};
		glm::mat4 proj{1.f};
		
		void update();

	private:
		glm::mat4 projView{};
	};
}		 // namespace Geg
