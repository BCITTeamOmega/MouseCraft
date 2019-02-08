#pragma once

#include "Shader.h"
#include <GLEW/glew.h>
#include <sdl/SDL_opengl.h>
#include <sdl/SDL.h>

#define GLEW_STATIC

static class ShaderBuilder {
	static Shader compile(string vertSrc, string fragSrc);
};