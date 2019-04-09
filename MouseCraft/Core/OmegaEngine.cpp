#include "OmegaEngine.h"
#include <chrono>
#include <SDL2/SDL.h>
#include "../gl/glad.h"
#include "TaskScheduler.h"
#include "../Event/EventManager.h"
#include "../Graphics/Window.h" 

OmegaEngine::~OmegaEngine()
{
	SDL_DestroyWindow(_window->getSDLWindow());
	SDL_Quit();
}

void OmegaEngine::initialize()
{
	// measure performance 
	_profiler.InitializeTimers(7);
	_profiler.LogOutput("Engine.log");	// optional
	// _profiler.PrintOutput(true);		// optional
	// _profiler.FormatMilliseconds(true);	// optional

	_profiler.StartTimer(5);

	// main is defined elsewhere
	_window = new Window("MouseCraft", SCREEN_WIDTH, SCREEN_HEIGHT);

	_profiler.StopTimer(5);
	std::cout << "Engine initialization finished: " << _profiler.GetDuration(4) << "ns" << std::endl;
}

void OmegaEngine::ChangeScene(Scene* scene)
{
	//std::cerr << "WARNING: Engine::changeScene(scene) is not recommended, use changeScene<Scene>()" << std::endl;
	_nextScene = scene;
	transitionScenes();
}

void OmegaEngine::AddSystem(System * system)
{
	_systems.push_back(system);
}

void OmegaEngine::AddEntity(Entity* entity)
{
	assert(GetActiveScene() != nullptr && "ERROR: There was no active scene!");
	_activeScene->root.AddChild(entity);
}

void OmegaEngine::TransferEntity(Entity * entity)
{
	transitionHolder.AddChild(entity, true);
}

void OmegaEngine::Loop()
{
	_isRunning = true;
	sequential_loop();
}

void OmegaEngine::Stop()
{
	_isRunning = false;
}

void OmegaEngine::Pause(bool p)
{
	std::cerr << "ERROR: Engine::pause() is not implemented yet" << std::endl;
	_isPause = p;
}

void OmegaEngine::DeferAction(StatusActionParam * action)
{
	std::unique_lock<std::mutex> lock(_deferredActionMtx);
	if (action->action == StatusActionType::Delete)
	{
		// WARNING: You can still double delete an entity by deleting the parent and child (passes validation).
		const bool is_in = _destructionValidation.find(action->target->GetID()) != _destructionValidation.end();
		if (is_in)
		{
			std::cout << "WARNING: You are attempting to delete an entity twice, check your code!" << std::endl;
			// do nothing
		}
		else
		{
			_destructionValidation.insert(action->target->GetID());
			_deferredActionsBack.push_back(action);
		}
	}
	else
	{
		_deferredActionsBack.push_front(action);
	}
}

int OmegaEngine::GetFrame() const
{
	return _frameCount;
}

Scene* OmegaEngine::GetActiveScene() const
{
	return _activeScene;
}

Entity* OmegaEngine::GetRoot() const
{
	if (_activeScene == nullptr) return nullptr;
	return &_activeScene->root;
}

void OmegaEngine::sequential_loop()
{
	auto timestamp = std::chrono::high_resolution_clock::now();

	while (_isRunning)
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> delta = now - timestamp;
		auto deltaSeconds = delta.count();
		timestamp = now;

		_profiler.StartTimer(0);

		// PHASE 0: Scene Change Requested
		_profiler.StartTimer(1);
		if (_sceneChangeRequested)
		{
			transitionScenes();
			continue;
		}
		_profiler.StopTimer(1);

		// PHASE 1: Status Change Resolution
		_profiler.StartTimer(2);
		// swap buffers 
		std::swap(_deferredActions, _deferredActionsBack);
		while (!_deferredActions.empty())
		{
			// WARNING: MEMORY LEAK - USE UNIQUE_POINTER 
			auto action = _deferredActions.front();
			
			switch (action->action)
			{
			case Move:
				action->target->SetParent(action->destination, true);
				break;
			case Delete:
				action->target->Destroy(true);
				break;
			case Enable:
				action->target->SetEnabled(true, true);
				break;
			case Disable:
				action->target->SetEnabled(false, true);
				break;
			default:
				std::cerr << "ERROR: UNKNOWN S.ACTION" << std::endl;
				break;
			}

			_deferredActions.pop_front();
		}
		_profiler.StopTimer(2);

		// PHASE 1.5: Transformation precompute 
		_profiler.StartTimer(3);
		precomputeTransforms(&_activeScene->root);
		_profiler.StopTimer(3);

		// PHASE 2: Component Update
		_profiler.StartTimer(4);
		auto deltaParam = new TypeParam<float>(deltaSeconds);	// Consider: Using unique-pointer for self-destruct
		EventManager::Notify(EventName::COMPONENT_UPDATE, deltaParam);	// serial
		delete(deltaParam);
		_profiler.StopTimer(4);

		// PHASE 3: System Update
		// During this phase the entity state is frozen. 
		// Entity parent, child, enable, or delete is deferred until next frame.
		_profiler.StartTimer(5);
		for (auto& s : _systems)
		{
			s->Update(deltaSeconds);
		}
		_profiler.StopTimer(5);

		_profiler.StopTimer(0);
		_profiler.FrameFinish();

		// PHASE 4: Buffer swap and Input Poll (SDL specific)
		SDL_GL_SwapWindow(_window->getSDLWindow());
		++_frameCount;
	}
}

void OmegaEngine::transitionScenes()
{
	// cleanup
	if (_activeScene)
	{
		_activeScene->CleanUp();
		_activeScene->root.Destroy(true);
		delete(_activeScene);
	}
	std::deque<StatusActionParam*>().swap(_deferredActions);	// https://stackoverflow.com/questions/709146/how-do-i-clear-the-stdqueue-efficiently
	std::deque<StatusActionParam*>().swap(_deferredActionsBack);
	_sceneChangeRequested = false;
	// load 
	_activeScene = _nextScene;
	_activeScene->InitScene();
	_activeScene->root.SetEnabled(true, true);
	// transfer entities 
	for (auto& e : transitionHolder.GetChildren())
		_activeScene->root.AddChild(e);
}

Window* OmegaEngine::getWindow() const
{
	return _window;
}
void OmegaEngine::precomputeTransforms(Entity* entity, glm::mat4 parentTransformation)
{
	// can use a enabled check here b/c of the scenegraph
	if (!entity->GetEnabled())
		return;

	// calculate local transformation 
	if (!entity->GetStatic())
		entity->transform.computeLocalTransformation();

	// calculate world transformation 
	entity->transform.computeWorldTransformation(parentTransformation);
	auto worldTransform = entity->transform.getWorldTransformation();

	// propogate to all children 
	auto children = entity->GetChildren();
	for (auto c : children)
		precomputeTransforms(c, worldTransform);
}
