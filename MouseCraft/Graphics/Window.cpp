#include "Window.h"
#include "RenderUtil.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

// TODO: Change width and height when window is resized

Window::Window(string title, int width, int height) : _width(width), _height(height) {
	static const int x = SDL_WINDOWPOS_UNDEFINED;
	static const int y = SDL_WINDOWPOS_UNDEFINED;

	static const char* cTitle = title.c_str();

	// OpenGL version 3.2
	static const int glMajorVer = 3;
	static const int glMinorVer = 3;

	static const int glDepthBufferSize = 24;

	SDL_SetMainReady();
	RenderUtil::sdlErrorOnNotSuccess(SDL_Init(SDL_INIT_VIDEO), "Initialization", 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajorVer);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinorVer);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, glDepthBufferSize);
	SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
	SDL_GL_SetSwapInterval(1);

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
	SDL_GL_SetSwapInterval(1);	// turn v-sync on
}

SDL_Window* Window::getSDLWindow() {
	return _sdlWindow;
}

SDL_GLContext Window::getContext() {
	return _context;
}

int Window::getWidth() {
	return _width;
}

int Window::getHeight() {
	return _height;
}