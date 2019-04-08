#pragma once

#include "Core/UpdatableComponent.h"
#include "Physics/PhysicsManager.h"
#include "Event/Handler.h"

class Obstacle : public UpdatableComponent
{
public:
	Obstacle();
	~Obstacle();
	virtual void Update(float deltaTime) override {};
	virtual void OnInitialized() override;
	virtual void HitByCat(Vector2D dir) = 0;
	virtual void DestroyedByMouse() = 0;
private:
	Handler<Obstacle> HandleOnDeath;
};

