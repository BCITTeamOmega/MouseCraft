#pragma once

#define SDL_MAIN_HANDLED

#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <set>
#include <memory>
#include <chrono>
#include <mutex>
#include <queue>
#include <deque>
#include <SDL2/SDL.h>
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "Scene.h"
#include "../Util/CpuProfiler.h"
#include "StatusAction.h"
#include "../Graphics/Window.h"

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

class OmegaEngine
{
// singleton pattern (threadsafe: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern)
public:
	static OmegaEngine& Instance()
	{
		static OmegaEngine _instance;
		return _instance;
	}
	OmegaEngine(OmegaEngine const&) = delete;
	void operator=(OmegaEngine const&) = delete;
private:
	OmegaEngine() : transitionHolder(nullptr) {};
	~OmegaEngine();

// variables 
private:
	// sdl 
	Window* _window = NULL;
	
	// engine 
	const std::chrono::nanoseconds _frameTime = std::chrono::milliseconds((long)(10));
	bool _initialized = false;
	bool _isPause = false;
	bool _isRunning = false;
	bool _sceneChangeRequested = false;
	Scene* _activeScene;
	Scene* _nextScene;
	CpuProfiler _profiler;
	RootEntity transitionHolder;	// used to hold entities while transitioning scene.
	std::deque<StatusActionParam*> _deferredActions;
	std::mutex _deferredActionMtx;
	std::vector<System*> _systems;
	int _frameCount;

// functions 
public:
	// Initializes the core engine.
	void initialize();

	// TODO: Properly implement.
	// Changes the active scene with another one.
	template<typename SceneType>
	void ChangeScene()
	{
		static_assert(std::is_base_of<Scene, SceneType>::value, "That's not a scene...");
		_nextScene = new SceneType();	// TODO: no benefit to this, need to move this to the transition.
		_sceneChangeRequested = true;
	}

	// Changes the active scene with a loaded scene. Not recommended, use changeScene<Type>.
	// This forces the scene to change instantly.
	void ChangeScene(Scene* scene);

	// Add a system to receive updates. 
	void AddSystem(System* system);

	// Convenience function to add entity to active scene root. Managed. 
	void AddEntity(Entity* entity);

	// Persist an entity to the next scene. Persisted entities will be in stasis
	// until the scene is loaded. Note this ticket is valid for one transfer only.
	// WARNING: This function is unmanaged, be careful when multithreading.
	void TransferEntity(Entity* entity);

	// Starts engine loop. Blocking.
	void Loop();

	// Stops engine loop. This will exit the program.
	void Stop();

	// TODO: Implement properly.
	// Pauses / unpauses the engine
	void Pause(bool pause);

	// WARNING: Should only be called internally by the engine.
	// Defers actions that can cause catastrophic failure. 
	void DeferAction(StatusActionParam* action);

	// Get the total elapsed frames.
	int GetFrame() const;

	// Gets the current active scene.
	Scene* GetActiveScene() const;

	// Gets the active scene root entity.
	Entity* GetRoot() const;

	Window* getWindow() const;

private:
	// Current implementation of game loop
	void sequential_loop();

	void transitionScenes();
};

