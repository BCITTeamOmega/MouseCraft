#include "CContactListener.h"

void CContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fa = contact->GetFixtureA();
	b2Fixture* fb = contact->GetFixtureB();

	if (fa == NULL || fb == NULL)
		return;

	std::cout << "collision" << std::endl;

	PhysicsComponent* pCompA = static_cast<PhysicsComponent*>(fa->GetBody()->GetUserData());
	PhysicsComponent* pCompB = static_cast<PhysicsComponent*>(fb->GetBody()->GetUserData());

	if (pCompA == nullptr || pCompB == nullptr)
		return;

	if (pCompA->type == PLATFORM)
	{
		if (pCompB->type == CAT_UP || pCompB->type == MOUSE_UP || pCompB->type == OBSTACLE_UP)
		{
			//PREVENT COLLISION RESOLUTION
		}
		else if (pCompB->type != CONTRAPTION_DOWN) //if you're anything other than these 4 just do the collision
		{
			return;
		}

		if (collided + 1 > arraySize)
			enlargeArrays();

		colliders1[collided] = fb->GetBody();
		colliders2[collided] = fa->GetBody();

		collided++;
	}
	else if (pCompB->type == PLATFORM)
	{
		if (pCompA->type == CAT_UP || pCompA->type == MOUSE_UP || pCompA->type == OBSTACLE_UP)
		{
			//PREVENT COLLISION RESOLUTION
		}
		else if (pCompA->type != CONTRAPTION_DOWN) //if you're anything other than these 4 just do the collision
		{
			return;
		}

		if (collided + 1 > arraySize)
			enlargeArrays();

		colliders1[collided] = fa->GetBody();
		colliders2[collided] = fb->GetBody();

		collided++;
	}
	else if (pCompA->type == WALL)
	{
		//Unless you are a contraption nothing special happens on collision with walls
		if (pCompB->type != CONTRAPTION_UP && pCompB->type != CONTRAPTION_DOWN)
			return;

		if (collided + 1 > arraySize)
			enlargeArrays();

		colliders1[collided] = fb->GetBody();
		colliders2[collided] = fa->GetBody();

		collided++;
	}
	else if (pCompB->type == WALL)
	{
		//Unless you are a contraption nothing special happens on collision with walls
		if (pCompA->type != CONTRAPTION_UP && pCompA->type != CONTRAPTION_DOWN)
			return;

		if (collided + 1 > arraySize)
			enlargeArrays();

		colliders1[collided] = fa->GetBody();
		colliders2[collided] = fb->GetBody();

		collided++;
	}
	else
	{
		//Basically everything else has special handling for collisions
		if (collided + 1 > arraySize)
			enlargeArrays();

		colliders1[collided] = fa->GetBody();
		colliders2[collided] = fb->GetBody();

		collided++;
	}
}

void CContactListener::EndContact(b2Contact* contact)
{
	b2Fixture* fa = contact->GetFixtureA();
	b2Fixture* fb = contact->GetFixtureB();

	if (fa == NULL || fb == NULL)
		return;

	std::cout << "collision ended" << std::endl;

	PhysicsComponent* pCompA = static_cast<PhysicsComponent*>(fa->GetBody()->GetUserData());
	PhysicsComponent* pCompB = static_cast<PhysicsComponent*>(fb->GetBody()->GetUserData());

	//if A is a platform
	if (pCompA->type == PLATFORM && (pCompB->type == CAT_UP || pCompB->type == MOUSE_UP || pCompB->type == OBSTACLE_UP))
	{
		if (collided + 1 > arraySize)
			enlargeArrays();

		//just ensures the platforms are always in colliders2 for easier checking in PhysicsManager
		colliders1[collided] = fb->GetBody();
		colliders2[collided] = fa->GetBody();

		collided++;
	}//if B is a platform
	else if (pCompB->type == PLATFORM && (pCompA->type == CAT_UP || pCompA->type == MOUSE_UP || pCompA->type == OBSTACLE_UP))
	{
		if (collided + 1 > arraySize)
			enlargeArrays();

		//just ensures the platforms are always in colliders2 for easier checking in PhysicsManager
		colliders1[collided] = fa->GetBody();
		colliders2[collided] = fb->GetBody();

		collided++;
	}
}

void CContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {};
void CContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};

void CContactListener::setup()
{
	arraySize = 4;
	collided = 0;
	colliders1 = (b2Body**)malloc(4 * sizeof(b2Body*));
	colliders2 = (b2Body**)malloc(4 * sizeof(b2Body*));
}

void CContactListener::enlargeArrays()
{
	arraySize = arraySize * 2;

	//allocate memory for colliders1
	b2Body** temp = (b2Body**)malloc(arraySize * sizeof(b2Body*));
	memcpy(temp, colliders1, arraySize / 2 * sizeof(b2Body*));
	delete[] colliders1;
	colliders1 = temp;

	//allocate memory for colliders2
	temp = (b2Body**)malloc(arraySize * sizeof(b2Body*));
	memcpy(temp, colliders2, arraySize / 2 * sizeof(b2Body*));
	delete[] colliders2;
	colliders2 = temp;
}

void CContactListener::resetCollided()
{
	collided = 0;
}

int CContactListener::hasCollided()
{
	return collided;
}

b2Body** CContactListener::getColliders1()
{
	return colliders1;
}

b2Body** CContactListener::getColliders2()
{
	return colliders2;
}