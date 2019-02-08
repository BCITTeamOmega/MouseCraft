#pragma once
#include "RenderData.h"
#include <GLEW/glew.h>
#include <sdl/SDL.h>
#include <vector>

class Renderer {
public:
	void initialize();
	void queueRender(const RenderData& m);
	void render();
	void cleanup();
private:
	SDL_Window* window;
	SDL_GLContext context;
	vector<RenderData> renderingList;
	vector<RenderData> accumulatingList;
};