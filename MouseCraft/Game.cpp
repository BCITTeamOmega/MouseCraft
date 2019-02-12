#include "Game.h"
#include "MainScene.h"
#include "Loading/ModelLoader.h"
#include "Graphics/RenderData.h"
#include <thread>

Game::Game() {
    _currScene = new MainScene();
    _currScene->InitScene();
	renderer = new Renderer();
	renderer->initialize();
	Model* m = ModelLoader::loadModel("res/models/test/CubeModel.obj");
	rd = new RenderData(m, -2.0, 0, -2.5, Color(0.5, 1.0, 0.25));
	rd2 = new RenderData(m, 3.5, 0, -3.0, Color(1.0, 0.25, 0.5));
}

Game::~Game() {
    _currScene->CleanUp();
	renderer->cleanup();
    delete _currScene;
	delete rd;
}

void Game::Update(const float delta) {
	renderer->queueRender(*rd);
	renderer->queueRender(*rd2);
	if (FadeOut) {
		if (Black->color.a < 1) {
			Black->color.a += 0.1;
		} else {
			Black->color.a = 1;
			FadeOut = false;
		}
	} else if (FadeIn) {
		if (Black->color.a > 0) {
			Black->color.a -= 0.1;
		} else {
			Black->color.a = 0;
			FadeIn = false;
		}
	} else if (_currScene != nullptr) {
		_currScene->Update(delta);
    }
	renderer->render();
}

void Game::Notify(EventName eventName, Param *params) {
	switch (eventName) {
	default:
		break;
	}
}

void fadeOut(Game* game) {
	game->Black = UIManager::GetComponentById("BlackOverlay");
	game->FadeOut = true;
	while (game->FadeOut) {
		Sleep(1);
	}
}

void fadeIn(Game* game) {
	game->Black = UIManager::GetComponentById("BlackOverlay");
	game->FadeIn = true;
	while (game->FadeIn) {
		Sleep(1);
	}
}

void Game::transition(Scene *nextScene) {
	// Control fade to black animation on separate thread
	std::thread t(&fadeOut, this);
	t.join();

	// Once black screen, clean up the current scene then initialize the next scene
	Scene *scene = _currScene;
	_currScene = nullptr;

	if (scene != nullptr) {
		scene->CleanUp();
		delete scene;
	}

	nextScene->InitScene();
	_currScene = nextScene;

	// After next scene is initialized, fade back in
	std::thread t2(&fadeIn, this);
	t2.join();
}

