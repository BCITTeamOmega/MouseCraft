#pragma once
#include "../Core/Vector2D.h"
#include "../Core/Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(float x, float y, float z, float vX = 0, float vY = 0, float zY = 0);

	Vector2D position, velocity;
	float zPos, zVel;
private:
	
};