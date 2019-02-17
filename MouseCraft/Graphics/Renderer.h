#pragma once
#include "RenderData.h"
#include "Shader.h"
#include "../GL/glad.h"
#include <sdl/SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "Color.h"

class Renderer {
public:
	void initialize();
	void queueRender(const RenderData& m);
	void render();
	void cleanup();
private:
	void setShader(Shader& s);
	void clearShader();
	int trySDL(int val, std::string step);
	int tryGLEW(int val, std::string step);
	void swapLists();
	glm::mat4 makeMatrix(RenderData r);
	glm::vec3 convertColor(Color c);

	SDL_Window* _window;
	SDL_GLContext _context;
	std::vector<RenderData>* _renderingList;
	std::vector<RenderData>* _accumulatingList;
	std::map<std::string, Shader> _shaders;
	Shader* _currShader;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	int _width;
	int _height;
};