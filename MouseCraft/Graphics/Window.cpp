#include "Window.h"
#include "RenderUtil.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

Window::Window(string title, int width, int height) {
	static const int x = SDL_WINDOWPOS_UNDEFINED;
	static const int y = SDL_WINDOWPOS_UNDEFINED;

	static const char* cTitle = title.c_str();

	// OpenGL version 3.2
	static const int glMajorVer = 3;
	static const int glMinorVer = 2;

	static const int glDepthBufferSize = 24;

	static const string shaderPath = "res/shaders/";

	SDL_SetMainReady();
	RenderUtil::sdlErrorOnNotSuccess(SDL_Init(SDL_INIT_VIDEO), "Initialization", 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajorVer);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinorVer);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, glDepthBufferSize);

	_sdlWindow = SDL_CreateWindow(cTitle, x, y, width, height, SDL_WINDOW_OPENGL);
	RenderUtil::sdlErrorOnNotSuccess(_sdlWindow == nullptr, "Window Creation", false);

	_context = SDL_GL_CreateContext(_sdlWindow);
	RenderUtil::sdlErrorOnNotSuccess(_context == nullptr, "Context Creation", false);

	RenderUtil::gladErrorOnFail(gladLoadGLLoader(SDL_GL_GetProcAddress), "Initialization", 0);

	cout << "Vendor:\t" << glGetString(GL_VENDOR) << endl
		<< "Renderer:\t" << glGetString(GL_RENDERER) << endl
		<< "Version:\t" << glGetString(GL_VERSION) << endl;
		
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

SDL_Window* Window::getSDLWindow() {
	return _sdlWindow;
}

SDL_GLContext Window::getContext() {
	return _context;
}