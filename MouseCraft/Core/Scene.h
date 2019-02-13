#pragma once

#include "RootEntity.h"

/**
Abstract class used to represent the framework for a game scene
*/
class Scene {
public:  
	RootEntity root = RootEntity(this);
    virtual void InitScene() = 0;
    virtual void Update(const float delta) = 0;
    virtual void CleanUp() = 0;
};

