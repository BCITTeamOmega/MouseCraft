#pragma once
#include "Scene.h"

class MainScene : public Scene {
	void InitScene();

	void Update(const float delta);

	void CleanUp();
};

