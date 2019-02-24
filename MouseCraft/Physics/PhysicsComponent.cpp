#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(float x, float y, float z, float r)
{
	position = Vector2D(x, y);
	velocity = Vector2D(0, 0);
	zPos = z;
	zVel = 0;
	rotation = r;
	isJumping = false;
}