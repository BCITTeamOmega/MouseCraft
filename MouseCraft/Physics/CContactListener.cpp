#include "CContactListener.h"

void CContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fa = contact->GetFixtureA();
	b2Fixture* fb = contact->GetFixtureB();

	if (fa == NULL || fb == NULL)
		return;

	PhysicsComponent* pCompA = static_cast<PhysicsComponent*>(fa->GetBody()->GetUserData());
	PhysicsComponent* pCompB = static_cast<PhysicsComponent*>(fb->GetBody()->GetUserData());

	if (pCompA == nullptr || pCompB == nullptr)
		return;

	if (pCompA->pType == PhysObjectType::PLATFORM && (pCompB->pType == PhysObjectType::CAT_UP || pCompB->pType == PhysObjectType::MOUSE_UP
		|| pCompB->pType == PhysObjectType::OBSTACLE_UP || pCompB->pType == PhysObjectType::CONTRAPTION_DOWN))
	{
		if (collided + 1 > arraySize)
			enlargeArrays();

		colliders1[collided] = fb->GetBody();
		colliders2[collided] = fa->GetBody();

		collided++;
	}
	else if (pCompB->pType == PhysObjectType::PLATFORM && (pCompA->pType == PhysObjectType::CAT_UP || pCompA->pType == PhysObjectType::MOUSE_UP
		|| pCompA->pType == PhysObjectType::OBSTACLE_UP || pCompA->pType == PhysObjectType::CONTRAPTION_DOWN))
	{
		if (collided + 1 > arraySize)
			enlargeArrays();

		colliders1[collided] = fa->GetBody();
		colliders2[collided] = fb->GetBody();

		collided++;
	}
	else if (pCompA->pType == PhysObjectType::WALL)
	{
		//Unless you are a contraption nothing special happens on collision with walls
		if (pCompB->pType != PhysObjectType::CONTRAPTION_UP && pCompB->pType != PhysObjectType::CONTRAPTION_DOWN)
			return;

		if (collided + 1 > arraySize)
			enlargeArrays();

		colliders1[collided] = fb->GetBody();
		colliders2[collided] = fa->GetBody();

		collided++;
	}
	else if (pCompB->pType == PhysObjectType::WALL)
	{
		//Unless you are a contraption nothing special happens on collision with walls
		if (pCompA->pType != PhysObjectType::CONTRAPTION_UP && pCompA->pType != PhysObjectType::CONTRAPTION_DOWN)
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

	std::cout << "Collision" << std::endl;

	PhysicsComponent* pCompA = static_cast<PhysicsComponent*>(fa->GetBody()->GetUserData());
	PhysicsComponent* pCompB = static_cast<PhysicsComponent*>(fb->GetBody()->GetUserData());

	//if A is a platform
	if (pCompA->pType == PhysObjectType::PLATFORM && (pCompB->pType == PhysObjectType::CAT_UP
		|| pCompB->pType == PhysObjectType::MOUSE_UP || pCompB->pType == PhysObjectType::OBSTACLE_UP))
	{
		if (collided + 1 > arraySize)
			enlargeArrays();

		//just ensures the platforms are always in colliders2 for easier checking in PhysicsManager
		colliders1[collided] = fb->GetBody();
		colliders2[collided] = fa->GetBody();

		collided++;
	}//if B is a platform
	else if (pCompB->pType == PhysObjectType::PLATFORM && (pCompA->pType == PhysObjectType::CAT_UP
		|| pCompA->pType == PhysObjectType::MOUSE_UP || pCompA->pType == PhysObjectType::OBSTACLE_UP))
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