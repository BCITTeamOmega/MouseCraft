#include "GameManager.h"

#include <iostream>

GameManager::GameManager() :
	HandleMiceDeath(this, &GameManager::OnMiceDeath),
	HandleMiceRevive(this, &GameManager::OnMiceRevive),
	HandleCatDeath(this, &GameManager::OnCatDeath)
{
}


GameManager::~GameManager()
{
}

void GameManager::Update(float deltaTime)
{
}

void GameManager::AddMouse(Mice * m)
{
	mice.push_back(m);
	++aliveMice;
	m->GetEntity()->GetComponent<HealthComponent>()->OnDeath.Attach(HandleMiceDeath);
	m->GetEntity()->GetComponent<HealthComponent>()->OnRevive.Attach(HandleMiceRevive);
}

void GameManager::SetCat(Cat * c)
{
	cat = c;
	c->GetEntity()->GetComponent<HealthComponent>()->OnDeath.Attach(HandleCatDeath);
}

void GameManager::OnMiceDeath()
{
	--aliveMice;
	if (aliveMice == 0)
		GameOver(GameOverParams::CAT_WON);
}

void GameManager::OnMiceRevive()
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

	// disable all players 
	for (auto m : mice) m->SetEnabled(false);
	cat->SetEnabled(false);
		
	// notify any interested parties 
	EventManager::Notify(EventName::GAMEOVER, new TypeParam<GameOverParams>(winner));
}
