#include "Renderer.h"
#include <string>

using std::string;

void Renderer::initialize() {
	static const int width = 640;
	static const int height = 480;
	static const int x = SDL_WINDOWPOS_UNDEFINED;
	static const int y = SDL_WINDOWPOS_UNDEFINED;

	static const char* title = "Mousecraft";

	// OpenGL version 3.2
	static const int glMajorVer = 3;
	static const int glMinorVer = 2;

	static const int glDepthBufferSize = 24;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajorVer);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinorVer);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, glDepthBufferSize);

	window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);	

	SDL_GL_SetSwapInterval(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::queueRender(const RenderData& m) {
}

void Renderer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapWindow(window);
}

void Renderer::cleanup() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}