#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include "PhysicsComponent.h"

class CContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	void setup();
	int hasCollided();
	void resetCollided();
	b2Body** getColliders1();
	b2Body** getColliders2();
private:
	int collided;
	int arraySize;
	b2Body **colliders1, **colliders2;
	void enlargeArrays();
};