#include "Renderer.h"

void Renderer::initialize() {
	static int width = 640;
	static int height = 480;
	static int startx = SDL_WINDOWPOS_UNDEFINED;
	static int starty = SDL_WINDOWPOS_UNDEFINED;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Mousecraft", 
		startx, 
		starty, 
		width,
		height,
		SDL_WINDOW_OPENGL);
	//Get window surface
	surface = SDL_GetWindowSurface(window);
}

void Renderer::queueRender(const RenderData& m) {
}

void Renderer::render() {
	SDL_UpdateWindowSurface(window);
}

void Renderer::cleanup() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}