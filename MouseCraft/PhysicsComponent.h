#pragma once
#include "Vector2D.h"
#include "Core/Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(float x, float y, float vX = 0, float vY = 0);

	Vector2D position, velocity;
private:
	
};