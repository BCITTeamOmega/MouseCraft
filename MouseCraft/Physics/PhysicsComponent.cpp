#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(PhysObjectType::PhysObjectType t, float z, float r, float vX, float vY)
{
	velocity = Vector2D(vX, vY);
	zPos = z;
	rotation = r;
	isJumping = false;
	type = t;
}

PhysicsComponent::~PhysicsComponent()
{
	body->GetWorld()->DestroyBody(body);
}
