#pragma once

#include "Core/Scene.h"

class HostScene : public Scene {
public:
    void InitScene() override;
    void Update(const float delta) override {}
    void CleanUp() override;
};

