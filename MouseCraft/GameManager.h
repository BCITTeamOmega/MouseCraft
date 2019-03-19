#pragma once

#include <vector>
#include "Core/Component.h"
#include "Core/UpdatableComponent.h"
#include "Event/Handler.h"
#include "Event/EventManager.h"
#include "Mice.h"
#include "Cat.h"

// Winner of the game
enum GameOverParams
{
	NONE,
	CAT_WON,
	MOUSE_WON
};

class GameManager : public UpdatableComponent
{
public:
	GameManager();
	~GameManager();
	virtual void Update(float deltaTime) override;
	void AddMouse(Mice* m);
	void SetCat(Cat* c);

private:
	void OnMiceDeath();
	void OnMiceRevive();
	void OnCatDeath();
	void GameOver(GameOverParams winner);

private:
	std::vector<Mice*> mice;
	Cat* cat;

	int aliveMice = 0;

	Handler<GameManager> HandleMiceDeath;
	Handler<GameManager> HandleMiceRevive;
	Handler<GameManager> HandleCatDeath;
};
