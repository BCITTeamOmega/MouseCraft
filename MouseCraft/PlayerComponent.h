#pragma once
#include "Core/Component.h"
#include "Core/UpdatableComponent.h"
#include "HealthComponent.h"
#include "Core/Entity.h"
#include "Event/ISubscriber.h"
#include "Physics/PhysicsComponent.h"

enum Team
{
	MOUSE,
	CAT
};

class PlayerComponent : public UpdatableComponent
{
public:
	virtual void OnInitialized();
	Team GetTeam() { return _teamID; }
	void SetTeam(Team id) { _teamID = id; }
	unsigned int GetID() { return _playerID; }
	void SetID(unsigned int id) { _playerID = id; }
	void SetSpeed(float speed) { _speed = speed; }
	float GetSpeed() { return _speed; }
	void SetDisabled(bool b) { _isDisabled = b; }


private:
	HealthComponent* _health;
	unsigned int _playerID;
	Team _teamID;
	bool _isDisabled = false;

	Entity* _entity;
	glm::vec2 _move;
	glm::vec2 _aim;
	float _speed = 50.0f;

	// physics component 
	PhysicsComponent* _physicsComponent;

	// Inherited via ISubscriber
	virtual void Notify(EventName eventName, Param * params) override;

	// Inherited via UpdatableComponent
	virtual void Update(float deltaTime) override;
};