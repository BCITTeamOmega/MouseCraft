#pragma once
#include "Obstacle.h"
#include "Physics/PhysicsComponent.h"
#include "Event/Handler.h"
#include "Event/EventManager.h"

/*
Layout of the Lamp

Entity 
- Lamp
- Physics
- Renderable (field)
	Entity 
	- Renderable (lamp model)
*/

class Lamp : public Obstacle
{
public:
	Lamp();
	~Lamp();

	virtual void OnInitialized() override;
	virtual void Update(float deltaTime) override;
	virtual void HitByCat(Vector2D dir) override;
	virtual void DestroyedByMouse() override;

	// reference to field visuals
	Entity* visualsEntity;

private:
	// deal one tick of damage every n seconds
	const float DAMAGE_RATE = 1.0f;

	// range of the field
	const float FIELD_RANGE = 16.0F;

	// if the field has been placed
	bool _isPlaced = false;

	// the floor which to check the cat for
	std::set<PhysObjectType::PhysObjectType> _checkFor;

	float _counter = 0;

	// Handler<Lamp, PhysicsComponent*> HandleOnCollision;
};