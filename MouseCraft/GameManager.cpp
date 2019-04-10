#include "GameManager.h"

#include <iostream>
#include "Sound\TrackParams.h"

GameManager::GameManager() :
	HandleMouseDeath(this, &GameManager::OnMouseDeath),
	HandleMouseRevive(this, &GameManager::OnMouseRevive),
	HandleCatDeath(this, &GameManager::OnCatDeath)
{
}


GameManager::~GameManager()
{
}

void GameManager::Update(float deltaTime)
{
}

void GameManager::AddMouse(Mouse * m)
{
	mice.push_back(m);
	++aliveMice;
	m->GetEntity()->GetComponent<HealthComponent>()->OnDeath.Attach(HandleMouseDeath);
	m->GetEntity()->GetComponent<HealthComponent>()->OnRevive.Attach(HandleMouseRevive);
}

void GameManager::SetCat(Cat * c)
{
	cat = c;
	c->GetEntity()->GetComponent<HealthComponent>()->OnDeath.Attach(HandleCatDeath);
}

void GameManager::OnMouseDeath()
{
	--aliveMice;
	if (aliveMice == 0)
		GameOver(GameOverParams::CAT_WON);
}

void GameManager::OnMouseRevive()
{
	++aliveMice;
}

void GameManager::OnCatDeath()
{
	GameOver(GameOverParams::MOUSE_WON);
}

void GameManager::GameOver(GameOverParams winner)
{
	std::cout << "GAMEOVER! THE WINNER IS: " << winner << std::endl;

    selectSong(GameoverBGM);
	// disable all players 
	for (auto m : mice)
	{
		m->GetEntity()->SetEnabled(false);
	}
	cat->GetEntity()->SetEnabled(false);

	// notify any interested parties 
	EventManager::Notify(EventName::GAMEOVER, new TypeParam<GameOverParams>(winner));
}
