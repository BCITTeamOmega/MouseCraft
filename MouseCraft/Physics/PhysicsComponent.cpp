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

bool PhysicsComponent::areaCheck(std::vector<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2, bool triggerHit)
{
	return PhysicsManager::instance()->areaCheck(this, toCheck, p1, p2, triggerHit);
}

Vector2D* PhysicsComponent::rayCheck(std::vector<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2, bool triggerHit)
{
	return PhysicsManager::instance()->rayCheck(this, toCheck, p1, p2, triggerHit);
}