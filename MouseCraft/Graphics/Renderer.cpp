#include "Renderer.h"
#include "Shader.h"
#include "../Loading/TextLoader.h"
#include <string>
#include <iostream>

using std::string;
using std::swap;
using std::cerr;
using std::endl;

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

	static const string shaderPath = "res/shaders/";

	trySDL(SDL_Init(SDL_INIT_VIDEO), "Initialization");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajorVer);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinorVer);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, glDepthBufferSize);

	_window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		trySDL(1, "Window Creation");
	}
	_context = SDL_GL_CreateContext(_window);
	if (_context == nullptr) {
		trySDL(1, "Context Creation");
	}
	_renderingList = new vector<RenderData>();
	_accumulatingList = new vector<RenderData>();

	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	tryGLEW(glewInit(), "Initialization");

	string vshSrc = TextLoader::load(shaderPath + "gbuffer.vsh");
	string fshSrc = TextLoader::load(shaderPath + "gbuffer.fsh");
	_shaders["gbuffer"] = Shader(vshSrc, fshSrc);
	_shaders["gbuffer"].compile();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	//glDepthRange(0.01f, 100.0f);
}

void Renderer::queueRender(const RenderData& m) {
	_accumulatingList->push_back(m);
}

void Renderer::render() {
	swapLists();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	setShader(_shaders["gbuffer"]);
	GLint pos = glGetAttribLocation(_shaders["gbuffer"].getProgram(), "position");
	glEnableVertexAttribArray(pos);

	GLuint indices[] = { 0, 1, 2, 1, 2, 3 };

	for (RenderData render : *_renderingList) {
		Geometry* g = render.getModel()->getGeometry();
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			g->getVertexData().size() * sizeof(GLfloat),
			static_cast<void*>(&(g->getVertexData()[0])),
			GL_STATIC_DRAW
		);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			g->getIndices().size() * sizeof(GLuint),
			static_cast<void*>(&(g->getIndices()[0])),
			GL_STATIC_DRAW
		);

		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawElements(GL_TRIANGLES, g->getIndices().size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);

	SDL_GL_SwapWindow(_window);

	SDL_Event e;
	SDL_PollEvent(&e);
}

void Renderer::swapLists() {
	swap(_renderingList, _accumulatingList);
	_accumulatingList->clear();
}

void Renderer::cleanup() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
	delete _renderingList;
	delete _accumulatingList;
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Renderer::setShader(Shader& shader) {
	glUseProgram(shader.getProgram());
}

int Renderer::trySDL(int val, string step) {
	if (val != 0) {
		cerr << "SDL Error during " << step << ": " << SDL_GetError() << endl;
	}
	return val;
}

int Renderer::tryGLEW(int val, string step) {
	if (val != GLEW_OK) {
		cerr << "GLEW Error during " << step << ": " << glewGetErrorString(val) << endl;
	}
	return val;
}