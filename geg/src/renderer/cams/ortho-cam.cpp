#include "ortho-cam.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/matrix.hpp"

namespace Geg {
	OrthoCam::OrthoCam(float top, float bottom, float left, float right) {
		projM = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		viewM = glm::mat4(1.f);
		projViewM = projM * viewM;
	}

	void OrthoCam::updateM() {
		glm::mat4 _transform = glm::translate(glm::mat4(1.f), position);
		glm::mat4 _rotation = glm::rotate(glm::mat4(1.f), rotation, {0, 0, 1});

		viewM = glm::inverse(_transform * _rotation);
		projViewM = projM * viewM;

	}
}		 // namespace Geg
