#pragma once
#include "Core/Component.h"
#include "Core/UpdatableComponent.h"
#include "HealthComponent.h"
#include "Core/Entity.h"
#include "Event/ISubscriber.h"

enum Team
{
	MOUSE,
	CAT
};

class PlayerComponent : public UpdatableComponent, public ISubscriber
{
public:
	virtual void OnInitialized();
	Team GetTeam() { return _teamID; }
	void SetTeam(Team id) { _teamID = id; }
	unsigned int GetID() { return _playerID; }
	void SetSpeed(float speed) { _speed = speed; }
	float GetSpeed() { return _speed; }

private:
	HealthComponent* _health;
	unsigned int _playerID;
	static unsigned int _curID;
	Team _teamID;

	Entity* _entity;
	float _moveX;
	float _moveY;
	float _aimX;
	float _aimY;
	float _speed;

	// Inherited via ISubscriber
	virtual void Notify(EventName eventName, Param * params) override;

	// Inherited via UpdatableComponent
	virtual void Update(float deltaTime) override;
};