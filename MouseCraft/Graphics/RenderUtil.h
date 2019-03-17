#pragma once
#include <iostream>
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

	static void checkGLError(std::string step) {
		// Adapted from https://www.khronos.org/opengl/wiki/OpenGL_Error
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::string errName = "{UNKNOWN}";
			switch (err) {
			case GL_INVALID_ENUM:
				errName = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				errName = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				errName = "GL_INVALID_OPERATION";
				break;
			case GL_STACK_OVERFLOW:
				errName = "GL_STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				errName = "GL_STACK_UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:
				errName = "GL_OUT_OF_MEMORY";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errName = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_CONTEXT_LOST:
				errName = "GL_CONTEXT_LOST";
				break;
			case GL_TABLE_TOO_LARGE:
				errName = "GL_TABLE_TOO_LARGE";
				break;
			}
			std::cerr << "OpenGL error during " << step << ": " << errName << std::endl;
		}
	}
};