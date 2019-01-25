#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <set>
#include <memory>
#include <chrono>
#include <mutex>
#include <SDL2/SDL.h>
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "../Scene.h"
#include "../Util/CpuProfiler.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

struct EntityAction
{
	int target;
	Entity* entity;
	EntityAction(int parent, Entity* e)
	{
		target = parent;
		entity = e;
	}
};

class OmegaEngine
{
// singleton pattern (threadsafe: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern)
public:
	static OmegaEngine& instance()
	{
		static OmegaEngine _instance;
		return _instance;
	}
	OmegaEngine(OmegaEngine const&) = delete;
	void operator=(OmegaEngine const&) = delete;
private:
	OmegaEngine();
	~OmegaEngine();

// variables 
public:
	// Scene* activeScene = nullptr;
	// AssetLoader loader;

private:
	// sdl 
	SDL_Window* _window = NULL;
	SDL_Surface* _screenSurface = NULL;
	SDL_GLContext _context = NULL;
	
	// engine 
	const std::chrono::nanoseconds _frameTime = std::chrono::milliseconds((long)(10));
	bool _initialized = false;
	bool _isPause = false;
	bool _isRunning = false;
	Scene* _activeScene;
	CpuProfiler _profiler;

// functions 
public:
	// Initializes the core engine.
	void initialize();

	// Changes the active scene. Does not dispose of the previous scene, but does disable it.
	void changeScene(Scene* scene);

	// Add a system to receive updates. 
	void addSystem(System* system);

	// Convenience function to add entity to active scene root. Managed. 
	void addEntity(Entity* entity);

	// Starts engine loop. Blocking.
	void loop();

	// Stops engine loop.
	void stop();

	// Pauses / unpauses the engine
	void pause(bool pause);

private:
	// Current implementation of game loop
	void sequential_loop();
};

