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
	//EventManager::Notify(PLAY_SONG, &param);
}

void MainTest()
{
	PrefabLoader::DumpLoaders();

	OmegaEngine::Instance().initialize();

	OmegaEngine::Instance().ChangeScene(new MenuScene());

	InputSystem* inputSystem = new InputSystem();

	//Can this go at the top?
	RenderSystem* renderSystem = new RenderSystem();
	renderSystem->setWindow(OmegaEngine::Instance().getWindow());

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