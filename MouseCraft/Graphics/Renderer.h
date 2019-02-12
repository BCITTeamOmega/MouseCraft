#pragma once
#include "RenderData.h"
#include "Shader.h"
#include <GLEW/glew.h>
#include <sdl/SDL.h>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;

class Renderer {
public:
	void initialize();
	void queueRender(const RenderData& m);
	void render();
	void cleanup();
private:
	void setShader(Shader& s);
	int trySDL(int val, string step);
	int tryGLEW(int val, string step);
	void swapLists();
	SDL_Window* _window;
	SDL_GLContext _context;
	vector<RenderData>* _renderingList;
	vector<RenderData>* _accumulatingList;
	map<string, Shader> _shaders;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};