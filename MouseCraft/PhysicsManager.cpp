#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	b2Vec2 gravity(0, 0);

	floorWorld = new b2World(gravity);
	upperWorld = new b2World(gravity);

	cListener = new CContactListener();
	cListener->setup();
	floorWorld->SetContactListener(cListener);
	upperWorld->SetContactListener(cListener);
}

PhysicsManager::~PhysicsManager()
{
	delete(cListener);
	delete(floorWorld);
	delete(upperWorld);
}

void PhysicsManager::Update(const float delta)
{
	const float ts = 1.0f / 60.0f;
	float t = 0;

	while (t + ts <= delta)
	{
		floorWorld->Step(ts, 10, 10);
		upperWorld->Step(ts, 10, 10);
		checkHeights();
		checkCollisions();
		t += ts;
	}

	if (t < delta)
	{
		floorWorld->Step(delta - t, 10, 10);
		upperWorld->Step(delta - t, 10, 10);
		checkHeights();
		checkCollisions();
	}
}

//Takes in a set of outer wall endpoints, makes a body out of them, and adds it to both worlds
void PhysicsManager::setOuterWalls(std::vector<std::pair<Vector2D, Vector2D>> walls)
{
	//Make the walls
	b2BodyDef wallsDef;
	wallsDef.type = b2_staticBody;
	wallsDef.position.Set(0, 0);

	b2Body* wallsBodyFloor = floorWorld->CreateBody(&wallsDef);
	b2Body* wallsBodyUpper = upperWorld->CreateBody(&wallsDef);

	//define fixture
	b2EdgeShape edge;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &edge;
	fixtureDef.filter.categoryBits = (short)0;
	fixtureDef.filter.maskBits = (short)1;

	for (int i = 0; i < walls.size(); i++)
	{
		edge.Set(b2Vec2(walls[i].first.x, walls[i].first.y), b2Vec2(walls[i].second.x, walls[i].second.x));
		wallsBodyFloor->CreateFixture(&fixtureDef);
		wallsBodyUpper->CreateFixture(&fixtureDef);
	}
}

void PhysicsManager::updateHeights()
{

}

void PhysicsManager::checkCollisions()
{

}