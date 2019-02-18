#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	b2Vec2 gravity(0, 0);

	world = new b2World(gravity);

	cListener = new CContactListener();
	cListener->setup();
	world->SetContactListener(cListener);
}

PhysicsManager::~PhysicsManager()
{
	delete(cListener);
	delete(world);
}

void PhysicsManager::Update(float dt)
{
	const float ts = 1.0f / 60.0f;
	float t = 0;

	//std::vector<PhysicsComponent*> objects = ComponentManager<PhysicsComponent>::Instance().All();

	//Step every 60th of a second
	while (t + ts <= dt)
	{
		//Advance each physics world
		world->Step(ts, 10, 10);

		//Update the heights of characters based on gravity and jumping
		updateHeights(dt);

		//Check for collisions in each physics world
		checkCollisions();
		t += ts;
	}

	//Run a smaller step for the remainder of the delta time
	if (t < dt)
	{
		world->Step(dt - t, 10, 10);
		updateHeights(dt);
		checkCollisions();
	}
}

void PhysicsManager::createPlayer(float x, float y, float w, float h, bool floor)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	bodyDef.angle = 0;

	b2Body* playerBody;

	playerBody = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(w, h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;
	playerBody->CreateFixture(&fixtureDef);

	player = playerBody;
}

//Kinematics can be used for pushables, parts, contraptions, etc.
void PhysicsManager::createKinematic(float x, float y, float w, float h, bool floor)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(x, y);

	b2Body* kinematicBody;

	kinematicBody = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(w, h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;
	kinematicBody->CreateFixture(&fixtureDef);

	//NOTE: there is a bullet setting for projectiles that move exceptionally fast
}

void PhysicsManager::createPlatform(float x, float y, float w, float h)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	bodyDef.angle = 0;

	b2Body* playerBody;

	playerBody = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(w, h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;
	playerBody->CreateFixture(&fixtureDef);
}

//Takes in a set of outer wall endpoints, makes a body out of them, and adds it to both worlds
void PhysicsManager::setOuterWalls(std::vector<std::pair<Vector2D, Vector2D>> walls)
{
	//Make the walls
	b2BodyDef wallsDef;
	wallsDef.type = b2_staticBody;
	wallsDef.position.Set(0, 0);

	b2Body* wallsBody = world->CreateBody(&wallsDef);

	//define fixture
	b2EdgeShape edge;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &edge;
	fixtureDef.filter.categoryBits = (short)0;
	fixtureDef.filter.maskBits = (short)1;

	for (int i = 0; i < walls.size(); i++)
	{
		edge.Set(b2Vec2(walls[i].first.x, walls[i].first.y), b2Vec2(walls[i].second.x, walls[i].second.x));
		wallsBody->CreateFixture(&fixtureDef);
	}
}

//Move each physics object up or down based on gravity and jumping
//Then, if any cross the height threshold, delete them from one world and add them to the other
void PhysicsManager::updateHeights(float delta)
{
	//For each physics object, check if they are jumping
	//If so, move them up by jump velocity * delta
	//Otherwise check if they are on the floor
	//If so, check if they are on a tall object/surface
	//If not, move them by gravity * delta
}

void PhysicsManager::checkCollisions()
{
	//If there are any unhandled collisions
	if (cListener->hasCollided() == 0)
		return;

	/*b2Body** dynamics = cListener->getDynamic();
	b2Body** kinematics = cListener->getKinematic();

	for (int c = 0; c < cListener->hasCollided(); c++)
	{
		b2Body* d = dynamics[c];
		b2Body* k = kinematics[c];
			
		//Figure out how to check for collisions on each one properly
		//Index dynamics like so, cListender->hasCollided() should be number of collisions
		//How do you know the number of dynamics and kinematics?
		//Is it a pair for each collision?
	}

	cListener->resetCollided();*/
}