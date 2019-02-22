#pragma once
#include "Core/Component.h"
#include "HealthComponent.h"
#include "Core/Entity.h"
#include "Event/ISubscriber.h"

class PlayerComponent : public Component, public ISubscriber
{
public:
	virtual void OnInitialized();

private:
	HealthComponent* _health;
	unsigned int _playerID;
	unsigned int _teamID;

	// Inherited via ISubscriber
	virtual void Notify(EventName eventName, Param * params) override;
};