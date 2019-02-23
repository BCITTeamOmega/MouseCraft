#pragma once
#include "Core/Component.h"
#include "HealthComponent.h"
#include "Core/Entity.h"
#include "Event/ISubscriber.h"

enum Team
{
	MOUSE,
	CAT
};

class PlayerComponent : public Component, public ISubscriber
{
public:
	virtual void OnInitialized();
	Team GetTeam() { return _teamID; }
	void SetTeam(Team id) { _teamID = id; }
	unsigned int GetID() { return _playerID; }

private:
	HealthComponent* _health;
	unsigned int _playerID;
	static unsigned int _curID;
	Team _teamID;

	// Inherited via ISubscriber
	virtual void Notify(EventName eventName, Param * params) override;
};