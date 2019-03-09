#pragma once
#include "../Core/Vector2D.h"
#include "../Core/Component.h"
#include <Box2D/Box2D.h>
#include "PhysObjectType.h"
#include "../Event/Subject.h"
#include "PhysicsManager.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(PhysObjectType::PhysObjectType t, float z, float r, float vX = 0, float vY = 0);
	~PhysicsComponent();
	bool areaCheck(std::vector<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2, bool triggerHit);
	Vector2D* rayCheck(std::vector<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2, bool triggerHit);

	Vector2D velocity;
	float zPos, rotation;
	bool isJumping, isFalling, isUp;
	b2Body* body;
	PhysObjectType::PhysObjectType type;
	Subject<PhysicsComponent*> onCollide; //for collision between bodies
	Subject<PhysicsComponent*> onHit; //for hitbox checking
private:
	
};