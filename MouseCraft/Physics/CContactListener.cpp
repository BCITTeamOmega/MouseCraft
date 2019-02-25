#include "CContactListener.h"

void CContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fa = contact->GetFixtureA();
	b2Fixture* fb = contact->GetFixtureB();

	std::cout << "collision" << std::endl;

	/*if (fa == NULL || fb == NULL)
		return;

	//if a is player or tongue
	if (fa->GetBody()->GetType() == b2_dynamicBody && fb->GetBody()->GetType() == b2_kinematicBody)
	{
		if (collided + 1 > arraySize)
			enlargeArrays();

		dynamic[collided] = fa->GetBody();
		kinematic[collided] = fb->GetBody();

		collided++;
	} //if b is player or tongue
	else if (fb->GetBody()->GetType() == b2_kinematicBody && fb->GetBody()->GetType() == b2_dynamicBody)
	{
		if (collided + 1 > arraySize)
			enlargeArrays();

		dynamic[collided] = fb->GetBody();
		kinematic[collided] = fa->GetBody();

		collided++;
	}*/
}

void CContactListener::EndContact(b2Contact* contact) {};
void CContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {};
void CContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};

void CContactListener::setup()
{
	arraySize = 4;
	collided = 0;
	dynamic = (b2Body**)malloc(4 * sizeof(b2Body*));
	kinematic = (b2Body**)malloc(4 * sizeof(b2Body*));
}

void CContactListener::enlargeArrays()
{
	arraySize = arraySize * 2;

	//allocate memory for dynamic
	b2Body** temp = (b2Body**)malloc(arraySize * sizeof(b2Body*));
	memcpy(temp, dynamic, arraySize / 2 * sizeof(b2Body*));
	dynamic = temp;

	temp = (b2Body**)malloc(arraySize * sizeof(b2Body*));
	memcpy(temp, kinematic, arraySize / 2 * sizeof(b2Body*));
	delete[] kinematic;
	kinematic = temp;
}

void CContactListener::resetCollided()
{
	collided = 0;
}

int CContactListener::hasCollided()
{
	return collided;
}

b2Body** CContactListener::getDynamic()
{
	return dynamic;
}

b2Body** CContactListener::getKinematic()
{
	return kinematic;
}