#include "Renderer.h"
#include <string>

using std::string;

void Renderer::initialize() {
	static const int width = 640;
	static const int height = 480;
	static const int x = SDL_WINDOWPOS_UNDEFINED;
	static const int y = SDL_WINDOWPOS_UNDEFINED;

	static const char* title = "Mousecraft";

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_OPENGL);
	//Get window surface
	surface = SDL_GetWindowSurface(window);
}

void Renderer::queueRender(const RenderData& m) {
}

void Renderer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_UpdateWindowSurface(window);
}

void Renderer::cleanup() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}