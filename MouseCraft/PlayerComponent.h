#pragma once
#include "Core/Component.h"
#include "Core/UpdatableComponent.h"
#include "HealthComponent.h"
#include "Core/Entity.h"
#include "Event/ISubscriber.h"
#include "Physics/PhysicsComponent.h"
#include "Event/Handler.h"

#include "Loading/PrefabLoader.h"
#include "json.hpp"
using json = nlohmann::json;

enum Team
{
	MOUSE,
	CAT
};

class PlayerComponent : public UpdatableComponent
{
public:
	PlayerComponent();
	virtual void OnInitialized();
	Team GetTeam() { return _teamID; }
	void SetTeam(Team id) { _teamID = id; }
	unsigned int GetID() { return _playerID; }
	void SetID(unsigned int id) { _playerID = id; }
	void SetSpeed(float speed) { _speed = speed; }
	float GetSpeed() { return _speed; }
	void SetDisabled(bool b) { _isDisabled = b; }
	void StopMoving();
	void ResumeMoving();

private:
	Handler<PlayerComponent> handleStop;
	Handler<PlayerComponent> handleResume;
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

	/* TEMPLATE
	{
		"type": "Player",
		"speed": 50.0,
		"team": "mouse",	// or cat 
		"id": 0				// OPTIONAL
	}
	DEPENDENT
	- HealthComponent
	- PhysicsComponent
	*/
	static Component* Create(json json);
	static PrefabRegistrar reg;
};