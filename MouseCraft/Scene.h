#pragma once

/**
Abstract class used to represent the framework for a game scene
*/
class Scene {
public:  
    virtual void InitScene() = 0;
    virtual void Update(const float delta) = 0;
    virtual void CleanUp() = 0;
};

