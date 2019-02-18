#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(float x, float y, float vX, float vY)
{
	position = Vector2D(x, y);
	velocity = Vector2D(vX, vY);
}