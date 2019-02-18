#pragma once
#include <string>
#include <sdl/SDL.h>

class RenderUtil {
public:
	template <typename O>
	static O sdlErrorOnNotSuccess(O val, std::string step, O success) {
		if (val != success) {
			std::cerr << "SDL Error during " << step << ": " << SDL_GetError() << std::endl;
		}
		return val;
	}

	template <typename O>
	static O gladErrorOnFail(O val, std::string step, O fail) {
		if (val == fail) {
			std::cerr << "GLAD Error during " << step << ": Failed to initialize opengl" << std::endl;
		}
		return val;
	}
};