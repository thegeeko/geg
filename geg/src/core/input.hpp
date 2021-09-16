#ifndef GEG_INPUT_HPP
#define GEG_INPUT_HPP

#include "app.hpp"

namespace Geg::Input {
	static bool isKeyPressed (int keyCode){
		return glfwGetKey(App::get().getWindow().getRawWindow(), keyCode);
	}

	static bool isButtonPressed (int button){
		return glfwGetMouseButton(App::get().getWindow().getRawWindow(), button);
	}

	static std::pair<float, float> getMousePos(){
		double x, y;
		glfwGetCursorPos(App::get().getWindow().getRawWindow(), &x, &y);
		return {(float)x, (float)y};
	}
}


#endif //GEG_INPUT_HPP
