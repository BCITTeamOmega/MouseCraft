#pragma once

#include "Core/Scene.h"

class ClientScene : public Scene {
public:
    void InitScene() override;
    void Update(const float dt) override {}
    void CleanUp() override;
};

