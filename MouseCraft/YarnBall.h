#pragma once

#include "Core/UpdatableComponent.h"
#include "Obstacle.h"

class YarnBall : public Obstacle
{
public:
	YarnBall();
	~YarnBall();
	void OnInitialized() override;
	void Update(float deltaTime) override;
	void HitByCat(Vector2D dir) override;
	void DestroyedByMouse() override;
private:
	const float SPEED = 10.0f;
	PhysicsComponent* _physics;
	void OnMouseCollide(PhysicsComponent* other);
	Handler<YarnBall, PhysicsComponent*> HandleMouseCollide;
};

