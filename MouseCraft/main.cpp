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
#include "UI/UIManager.h"

SoundManager* noise;

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void SetupSound()
{
	//adding sound system
	noise = new SoundManager();
	//start initial music track, standard form for music selection
    selectSong(MenuBGM);
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
	OmegaEngine::Instance().AddSystem(new UIManager());

	//Start the game
	OmegaEngine::Instance().Loop();
}

int main(int argc, char* argv[])
{
	SetupSound();

	MainTest();
}