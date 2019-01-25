#pragma once

#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <set>
#include <memory>
#include <chrono>
#include <mutex>
#include <queue>
#include <SDL2/SDL.h>
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "../Scene.h"
#include "../Util/CpuProfiler.h"
#include "StatusAction.h"

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

//struct FloatParam
//{
//	FloatParam(float val) : value(val) {}
//	float value;
//};

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
	OmegaEngine() {};
	~OmegaEngine();

// variables 
public:
	Scene* activeScene = nullptr;
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
	bool _sceneChangeRequested = false;
	Scene* _activeScene;
	Scene* _nextScene;
	CpuProfiler _profiler;
	Entity transitionHolder;	// used to hold entities while transitioning scene.
	std::queue<StatusAction*> _deferredActions;
	std::mutex _deferredActionMtx;
	std::vector<System*> _systems;
	int _frameCount;

// functions 
public:
	// Initializes the core engine.
	void initialize();

	template<typename SceneType>
	void changeScene()
	{
		
		static_assert(std::is_base_of<Scene, SceneType>::value, "That's not a scene...");
		_nextScene = new SceneType();	// might want to move this to the transition.
		_sceneChangeRequested = true;
	}

	// Changes the active scene with a loaded scene. Not recommended, use changeScene(Type).
	void changeScene(Scene* scene);

	// Add a system to receive updates. 
	void addSystem(System* system);

	// Convenience function to add entity to active scene root. Managed. 
	void addEntity(Entity* entity);

	// Persist an entity to the next scene. Persisted entities will be in stasis
	// until the scene is loaded. Note this ticket is valid for one transfer only.
	// This function is unmanaged, be careful when multithreading.
	void transferEntity(Entity* entity);

	// Starts engine loop. Blocking.
	void loop();

	// Stops engine loop.
	void stop();

	// Pauses / unpauses the engine
	void pause(bool pause);

	// Defers actions that can cause catastrophic failure. 
	void deferAction(StatusAction* action);

	int getFrame() const;

private:
	// Current implementation of game loop
	void sequential_loop();
};

