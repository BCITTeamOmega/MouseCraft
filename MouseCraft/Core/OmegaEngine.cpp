#include "OmegaEngine.h"
#include <chrono>
#include <SDL2/SDL.h>
#include "../gl/glad.h"
#include "TaskScheduler.h"
#include "../EventManager/EventManager.h"

OmegaEngine::~OmegaEngine()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void OmegaEngine::initialize()
{
	// measure performance 
	_profiler.InitializeTimers(6);
	_profiler.LogOutput("Engine.log");	// optional
	// _profiler.PrintOutput(true);		// optional
	// _profiler.FormatMilliseconds(true);	// optional

	_profiler.StartTimer(5);

	// main is defined elsewhere
	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cerr << "ERROR: SDL could not initialize. SDL_Error:  " << SDL_GetError() << std::endl;
		return;
	}

	// prepare opengl version (4.2) for SDL 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	// using core as opposed to compatibility or ES 

	// create window
	_window = SDL_CreateWindow("Mouse Craft", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (_window == NULL)
	{
		std::cerr << "ERROR: SDL window could not be created. SDL_Error:  " << SDL_GetError() << std::endl;
		return;
	}

	// get window surface (not necessary)
	_screenSurface = SDL_GetWindowSurface(_window);

	// initialize sdl opengl context 
	_context = SDL_GL_CreateContext(_window);
	if (_context == NULL)
	{
		std::cerr << "ERROR: SDL failed to create openGL context. SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	// initialize opengl 
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cerr << "ERROR: GLAD failed to initialize opengl function pointers." << std::endl;
		return;
	}
	std::cout << "Vendor:\t" << glGetString(GL_VENDOR) << std::endl
		<< "Renderer:\t" << glGetString(GL_RENDERER) << std::endl
		<< "Version:\t" << glGetString(GL_VERSION) << std::endl;
	//cout << "SOUND:"<<SDL_GetCurrentAudioDriver()<<endl;

	// configure opengl 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//initialize SDL sound mixer context
	//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
	//	printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	//	return;
	//}

	_profiler.StopTimer(5);
	std::cout << "Engine initialization finished: " << _profiler.GetDuration(4) << "ns" << std::endl;
}

void OmegaEngine::changeScene(Scene* scene)
{
	std::cerr << "ERROR: Engine::changeScene() is not implemented yet" << std::endl;

	// TODO: implement

	// disable previous scene 
	if (_activeScene)
		_activeScene->CleanUp();

	// enable new scene 
	_activeScene = scene;
	scene->InitScene();
}

void OmegaEngine::addSystem(System * system)
{
	std::cerr << "ERROR: Engine::addSystem() is not implemented yet" << std::endl;
}

void OmegaEngine::addEntity(Entity* entity)
{
	std::cerr << "ERROR: Engine::addEntity() is not implemented yet" << std::endl;
}

void OmegaEngine::transferEntity(Entity * entity)
{
	transitionHolder.addChild(entity, true);
}

void OmegaEngine::loop()
{
	_isRunning = true;
	sequential_loop();
}

void OmegaEngine::stop()
{
	_isRunning = false;
}

void OmegaEngine::pause(bool p)
{
	std::cerr << "ERROR: Engine::pause() is not implemented yet" << std::endl;
	_isPause = p;
}

void OmegaEngine::deferAction(StatusAction * action)
{
	std::unique_lock<std::mutex> lock(_deferredActionMtx);
	_deferredActions.push(action);
}

int OmegaEngine::getFrame() const
{
	return _frameCount;
}

void OmegaEngine::sequential_loop()
{
	auto amt = std::chrono::milliseconds(16);
	std::chrono::duration<double> dt = amt;

	auto timestamp = std::chrono::high_resolution_clock::now();

	while (_isRunning)
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto delta = now - timestamp;
		auto deltaMs = delta.count() / (float)1000000;
		timestamp = now;

		_profiler.StartTimer(0);

		// PHASE 0: Scene Change Requested
		_profiler.StartTimer(1);
		if (_sceneChangeRequested)
		{
			// cleanup
			_activeScene->CleanUp();
			std::queue<StatusAction*>().swap(_deferredActions);	// https://stackoverflow.com/questions/709146/how-do-i-clear-the-stdqueue-efficiently
			_sceneChangeRequested = false;
			// load 
			_activeScene = _nextScene;
			_activeScene->InitScene();
			continue;
		}
		_profiler.StopTimer(1);

		// PHASE 1: Status Change Resolution
		_profiler.StartTimer(2);
		while (!_deferredActions.empty())
		{
			// WARNING: MEMORY LEAK - USE UNIQUE_POINTER 
			_deferredActions.front()->Execute();
			_deferredActions.pop();
		}
		_profiler.StopTimer(2);

		// PHASE 2: Component Update
		_profiler.StartTimer(3);
		auto deltaParam = new TypeParam<float>(deltaMs);	// Consider: Using unique-pointer for self-destruct
		EventManager::Notify(EventName::PLAY_SONG, deltaParam);	// serial
		delete(deltaParam);
		_profiler.StopTimer(3);

		// PHASE 3: System Update
		// During this phase the entity state is frozen. 
		// Entity parent, child, enable, or delete is deferred until next frame.
		_profiler.StartTimer(4);
		for (auto& s : _systems)
		{
			s->update(deltaMs);
		}
		_profiler.StopTimer(4);

		_profiler.StopTimer(0);
		_profiler.FrameFinish();

		// PHASE 4: Buffer swap and Input Poll (SDL specific)
		SDL_GL_SwapWindow(_window);
		++_frameCount;
	}
}

