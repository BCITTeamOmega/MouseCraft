#include "Game.h"
#include "MainScene.h"
#include "NetworkManager.h"
#include <thread>
#include <chrono>
#include <iostream>

Game::Game() {
    int port;
    std::cout << "Select Port to initialize on: " << std::endl;
    std::cin >> port;
    if (NetworkManager::Initialize(port)) {
        std::cout << "Network Manager successfully intialized." << std::endl;

        bool valid = false;
        do {
            std::cin.clear();

            std::cout << "Client or Server? (C/S)" << std::endl;
            char choice;
            std::cin >> choice;

            if (choice == 'C' || choice == 'c') {
                NetworkManager::JoinServer();
                valid = true;
            } else if (choice == 'S' || choice == 's') {
                NetworkManager::CreateRoom();
                valid = true;
            } else {
                std::cout << "Invalid Option" << std::endl;
            }
        } while (!valid);
    }
    _currScene = new MainScene();
    _currScene->InitScene();
}

Game::~Game() {
    _currScene->CleanUp();
    delete _currScene;
    NetworkManager::CleanUp();
}

void Game::Update(const float delta) {
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
        NetworkManager::Update(delta);
    }
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
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void fadeIn(Game* game) {
	game->Black = UIManager::GetComponentById("BlackOverlay");
	game->FadeIn = true;
	while (game->FadeIn) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

