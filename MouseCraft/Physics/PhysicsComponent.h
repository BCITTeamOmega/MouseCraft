#pragma once
#include "../Core/Vector2D.h"
#include "../Core/Component.h"
#include <Box2D/Box2D.h>
#include "PhysObjectType.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(PhysObjectType::PhysObjectType t, float z, float r, float vX = 0, float vY = 0);

	Vector2D velocity;
	float zPos, rotation;
	bool isJumping, isFalling, isUp;
	b2Body* body;
	PhysObjectType::PhysObjectType type;
private:
	
};