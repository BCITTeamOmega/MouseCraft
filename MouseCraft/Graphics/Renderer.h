#pragma once
#include "Model.h"
#include "RenderData.h"
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
	SDL_Surface* surface;
	vector<RenderData> renderingList;
	vector<RenderData> accumulatingList;
};