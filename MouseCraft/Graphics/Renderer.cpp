#include "Renderer.h"
#include <sdl/SDL.h>

void Renderer::initialize() {
	SDL_Window *window;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
}