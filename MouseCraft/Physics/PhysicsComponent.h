#pragma once
#include "../Core/Vector2D.h"
#include "../Core/Component.h"
#include <Box2D/Box2D.h>
#include "PhysObjectType.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(float x, float y, float z, float r, PhysObjectType::PhysObjectType t);

	Vector2D position, velocity;
	float zPos, rotation;
	bool isJumping, isFalling, isUp;
	b2Body* body;
	PhysObjectType::PhysObjectType type;
private:
	
};