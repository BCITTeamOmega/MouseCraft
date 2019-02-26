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

	//update body velocities
	b2Body* b = world->GetBodyList(); //points to the first body

	while (b != NULL)
	{
		PhysicsComponent* pcomp = static_cast<PhysicsComponent*>(b->GetUserData());

		if (pcomp != NULL)
		{
			//call set velocity on the body to set it to velocity in physics component
			b->SetLinearVelocity(b2Vec2(pcomp->velocity.x, pcomp->velocity.y));
		}

		//get the next body
		b = b->GetNext();
	}
	
	//Step every 60th of a second
	while (t + ts <= dt)
	{
		//Advance each physics world
		world->Step(ts, 10, 10);

		//Update the heights of characters based on gravity and jumping
		updateHeights(ts);

		//Check for collisions in each physics world
		checkCollisions();
		t += ts;
	}

	//Run a smaller step for the remainder of the delta time
	if (t < dt)
	{
		world->Step(dt - t, 10, 10);
		updateHeights(dt - t);
		checkCollisions();
	}
	
	//update all the components to match the bodies
	b = world->GetBodyList(); //points to the first body

	while (b != NULL)
	{
		PhysicsComponent* pcomp = static_cast<PhysicsComponent*>(b->GetUserData());

		if (pcomp != NULL)
		{
			//copy all relevant data from b to pcomp
			pcomp->position = Vector2D(b->GetPosition().x, b->GetPosition().y);
			pcomp->velocity = Vector2D(b->GetLinearVelocity().x, b->GetLinearVelocity().y);
		}

		//get the next body
		b = b->GetNext();
	}
}

PhysicsComponent* PhysicsManager::createObject(float x, float y, float w, float h, float r, PhysObjectType t)
{
	PhysicsComponent* physicsComp = new PhysicsComponent(x, y, 0, r, t);

	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	bodyDef.angle = r;

	b2Body* body;

	b2PolygonShape shape;
	shape.SetAsBox(w, h);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;
	fixtureDef.userData = physicsComp;

	switch (t)
	{
	case PART:
		bodyDef.type = b2_staticBody;
		fixtureDef.filter.categoryBits = PART_CATEGORY;
		fixtureDef.filter.maskBits = PART_MASK;
		break;
	case CONTRAPTION_UP:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = CONTRAPTION_UP_CATEGORY;
		fixtureDef.filter.maskBits = CONTRAPTION_UP_MASK;
		physicsComp->isUp = true;
		break;
	case CONTRAPTION_DOWN:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = CONTRAPTION_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = CONTRAPTION_DOWN_MASK;
		physicsComp->isUp = false;
		break;
	case CAT_UP:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = CAT_UP_CATEGORY;
		fixtureDef.filter.maskBits = CAT_UP_MASK;
		physicsComp->isUp = true;
		break;
	case CAT_DOWN:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = CAT_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = CAT_DOWN_MASK;
		physicsComp->isUp = false;
		break;
	case MOUSE_UP:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = MOUSE_UP_CATEGORY;
		fixtureDef.filter.maskBits = MOUSE_UP_MASK;
		physicsComp->isUp = true;
		break;
	case MOUSE_DOWN:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = MOUSE_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = MOUSE_DOWN_MASK;
		physicsComp->isUp = false;
		break;
	case OBSTACLE_UP:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = OBSTACLE_UP_CATEGORY;
		fixtureDef.filter.maskBits = OBSTACLE_UP_MASK;
		physicsComp->isUp = true;
		break;
	case OBSTACLE_DOWN:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = OBSTACLE_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = OBSTACLE_DOWN_MASK;
		physicsComp->isUp = false;
		break;
	case PLATFORM:
		bodyDef.type = b2_staticBody;
		fixtureDef.filter.categoryBits = PLATFORM_CATEGORY;
		fixtureDef.filter.maskBits = PLATFORM_MASK;
		break;
	case WALL:
		bodyDef.type = b2_staticBody;
		fixtureDef.filter.categoryBits = WALL_CATEGORY;
		fixtureDef.filter.maskBits = WALL_MASK;
	}

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	switch (t)
	{
		case CAT_UP:
		case CAT_DOWN:
		case MOUSE_UP:
		case MOUSE_DOWN:
			players.push_back(body);
			break;
		default:
			break;
	}

	physicsComp->body = body;

	body->SetUserData(physicsComp);

	return physicsComp;

	//NOTE: there is a bullet setting for projectiles that move exceptionally fast
}

//Move each physics object up or down based on gravity and jumping
//Then, if any cross the height threshold, change their filters
//Possibly optimize this later
void PhysicsManager::updateHeights(float step)
{
	b2Body* b = world->GetBodyList();
	PhysicsComponent* comp;

	while (b != NULL)
	{
		//If the body can't move don't bother
		if (b->GetType() == b2_staticBody)
		{
			b = b->GetNext();
			continue;
		}

		comp = static_cast<PhysicsComponent*>(b->GetUserData());

		if (comp == nullptr)
		{
			b = b->GetNext();
			continue;
		}

		if (comp->isJumping)
		{
			comp->zPos += step * JUMP_VELOCITY;

			if (comp->isUp)
			{
				//The object in the upper half
				//Has it reached the platform yet?
				if (comp->zPos >= Z_UPPER)
				{
					comp->isJumping = false;
					comp->zPos = Z_UPPER;
				}
				
			}
			else if (comp->zPos >= Z_THRESHOLD)
			{
				//The object is in the lower half
				//Has it reached the threshold yet?
				comp->isUp = true;
			}
		}
		else if (comp->isFalling)
		{
			comp->zPos -= step * FALL_VELOCITY;

			if (comp->isUp && comp->zPos <= Z_THRESHOLD)
			{
				//The object is in the upper half
				//Has it reached the threshold yet?
				comp->isUp == false;
			}
			else
			{
				//The object is in the lower half
				//Has it reached the ground yet?
				if (comp->zPos <= Z_LOWER)
				{
					comp->isFalling = false;
					comp->zPos = Z_LOWER;
				}
			}
		}

		b = b->GetNext();
	}

	//For each physics object, check if they are jumping
	//If so, move them up by jump velocity * delta
	//Otherwise check if they are on the floor
	//If so, check if they are on a tall object/surface
	//If not, move them by gravity * delta

	//WHEN A Z THRESHOLD IS MET TO CHANGE LEVELS, MODIFY THE FILTERS
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