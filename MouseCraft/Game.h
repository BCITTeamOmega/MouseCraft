#pragma once

#include "Scene.h"
#include "EventManager/EventManager.h"
#include "UIManager/UIManager.h"

/**
Game contains scenes for game and transitions between them
*/
class Game : public ISubscriber {
public:
	Game();
    ~Game();

	// Update function called each frame passing in delta time for that frame
    void Update(const float delta);

    void Notify(EventName eventName, Param *params); // overrides ISubscriber::Notify

	// Used for Scene Transition animation
    bool FadeOut = false;
    bool FadeIn = false;
    UIComponent *Black = nullptr;
private:
	// Transition the current scene to the given scene. Cleanup current scene and initialize and execute the new scene 
	void transition(Scene *nextScene);

    Scene *_currScene;
};