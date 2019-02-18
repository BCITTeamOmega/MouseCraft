#pragma once

#include <Box2D/Box2D.h>
#include <iostream>

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
	b2Body** getDynamic();
	b2Body** getKinematic();
private:
	int collided;
	int arraySize;
	b2Body **dynamic, **kinematic;
	void enlargeArrays();
};