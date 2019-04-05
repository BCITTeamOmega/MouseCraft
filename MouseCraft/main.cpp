#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#include <Windows.h>
#include <iostream>
#include "Core/OmegaEngine.h"
#include "Graphics/RenderSystem.h"
#include "Input/InputSystem.h"
#include "Loading/PrefabLoader.h"
#include "Network/NetworkSystem.h"
#include "Physics/PhysicsManager.h"
#include "Sound/SoundManager.h"
#include "ContraptionSystem.h"
#include "MenuScene.h"

SoundManager* noise;

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void SetupSound()
{
	//adding sound system
	noise = new SoundManager();
	//start initial music track, standard form for music selection
	//create Track Params for event
	TrackParams * initial = new TrackParams();
	//select song
	initial->track = MainBGM;
	//specify song location. Usually fine to leave with default values of 0
	initial->x = 0;
	initial->y = 0;
	initial->z = 0;
	//create a type Param from the track params and pass it into the event notifier
	TypeParam<TrackParams*> param(initial);
	EventManager::Notify(PLAY_SONG, &param);
}

void MainTest()
{
	PrefabLoader::DumpLoaders();

	OmegaEngine::Instance().initialize();

	OmegaEngine::Instance().ChangeScene(new MenuScene());

	InputSystem* inputSystem = new InputSystem();

	//Create PhysicsManager and tell it how big the world is
	PhysicsManager::instance()->setupGrid(100, 75, 5);

	
	std::string* menuGameStartTex = new std::string("res/textures/menu1.png");
	std::string* menuQuitTex = new std::string("res/textures/menu2.png");

	// Set up menu entities
	Entity* menuEntity = EntityManager::Instance().Create();
	Entity* startGameEntity = EntityManager::Instance().Create();
	Entity* quitEntity = EntityManager::Instance().Create();
	menuEntity->transform.setLocalPosition(glm::vec3(0.5, 0.6, 0));

	UIRenderable* startGameUIRend = ComponentManager<UIRenderable>::Instance().Create<UIRenderable>();
	startGameUIRend->setSize(0.6f, 0.2f);
	startGameUIRend->setTexture(menuGameStartTex);
	startGameUIRend->setColor(Color(1.0, 1.0, 1.0));
	startGameEntity->AddComponent(startGameUIRend);

	UIRenderable* quitUIRend = ComponentManager<UIRenderable>::Instance().Create<UIRenderable>();
	quitUIRend->setSize(0.6f, 0.2f);
	quitUIRend->setTexture(menuQuitTex);
	quitUIRend->setColor(Color(1.0, 1.0, 1.0));
	quitEntity->AddComponent(quitUIRend);

	//Create the menu component
	MenuController* mc = ComponentManager<MenuController>::Instance().Create<MenuController>();
	menuEntity->AddComponent(mc);
	mc->addMenuItem(startGameEntity);
	mc->addMenuItem(quitEntity);
	// NOTE: The menu controller's callback is set up at the end

	//Can this go at the top?
	RenderSystem* renderSystem = new RenderSystem();
	renderSystem->setWindow(OmegaEngine::Instance().getWindow());

			auto* bookEntity = ObstacleFactory::Instance().Create(OBSTACLES::BOOK, glm::vec3(5, 0, 35), true);
			auto* boxEntity = ObstacleFactory::Instance().Create(OBSTACLES::BOX, glm::vec3(50, 0, 50), false);
			auto* vaseEntity = ObstacleFactory::Instance().Create(OBSTACLES::VASE, glm::vec3(30, 0, 50), false);
			auto* lampEntity = ObstacleFactory::Instance().Create(OBSTACLES::LAMP, glm::vec3(35, 0, 25), true);
			auto* ballEntity = ObstacleFactory::Instance().Create(OBSTACLES::YARNBALL, glm::vec3(35, 0, 30), true);
			auto* lampEntity2 = ObstacleFactory::Instance().Create(OBSTACLES::LAMP, glm::vec3(70, 0, 50), false);
	//Add the systems
	OmegaEngine::Instance().AddSystem(PhysicsManager::instance());
	OmegaEngine::Instance().AddSystem(renderSystem);
	OmegaEngine::Instance().AddSystem(inputSystem);
	OmegaEngine::Instance().AddSystem(new ContraptionSystem());
    OmegaEngine::Instance().AddSystem(NetworkSystem::Instance());

	//Start the game
	OmegaEngine::Instance().Loop();
}

int main(int argc, char* argv[]) 
{
	SetupSound();

	MainTest();
}