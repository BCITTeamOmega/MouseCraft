#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(float x, float y, float z, float vX, float vY, float vZ)
{
	position = Vector2D(x, y);
	velocity = Vector2D(vX, vY);
	zPos = z;
	zVel = vZ;
}