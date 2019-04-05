#pragma once
#include "Core/Scene.h"
#include "Core/Entity.h"

class MenuScene : public Scene {
public:
	void InitScene() override;

	void Update(const float delta) override;

	void CleanUp() override;
private:
    Entity *_menu;
};

