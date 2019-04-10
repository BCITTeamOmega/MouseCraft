#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::pmInstance;

PhysicsManager::PhysicsManager()
{
	b2Vec2 gravity(0, 0);

	world = new b2World(gravity);

	cListener = new CContactListener();
	cListener->setup();
	world->SetContactListener(cListener);

	profiler.InitializeTimers(4);
	profiler.LogOutput("Physics.log");	// optional
}

PhysicsManager::~PhysicsManager()
{
	delete(cListener);
	delete(world);
	delete(grid);
}

PhysicsManager* PhysicsManager::instance()
{
	if (pmInstance == nullptr)
		pmInstance = new PhysicsManager();
	return pmInstance;
}

void PhysicsManager::destroy()
{
	if (pmInstance != nullptr)
		delete(pmInstance);
}

void PhysicsManager::Update(float dt)
{
	profiler.StartTimer(0);

	/* 
	Resolve body status. This allows use to disable entities or components. 
	Note: OmegaEngine guarantees that entity life/status will not change during system updates. 	
	*/
	auto physicComponents = ComponentManager<PhysicsComponent>::Instance().All();
	for (auto& pc : physicComponents)
	{
		// performance should be ok referring to the latest revision of b2body.cpp 
		// (there's fast return if this doesn't change active status)
		pc->body->SetActive(pc->GetActive());
	}

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
		PhysicsComponent* pcomp;

		if (b->GetType() == b2_staticBody)
		{
			pcomp = static_cast<PhysicsComponent*>(b->GetUserData());

			b = b->GetNext();
			continue;
		}

		pcomp = static_cast<PhysicsComponent*>(b->GetUserData());

		if (pcomp != NULL)
		{
			//copy all relevant data from b to pcomp
			pcomp->GetEntity()->transform.setLocalPosition(glm::vec3(b->GetPosition().x, pcomp->zPos, b->GetPosition().y));
			pcomp->velocity = Vector2D(b->GetLinearVelocity().x, b->GetLinearVelocity().y);
		}

		//get the next body
		b = b->GetNext();
	}


	profiler.StopTimer(0);
	profiler.FrameFinish();
}

//Make sure scale divides into w and h or your w and h will be less than you want
void PhysicsManager::setupGrid(int w, int h, int scale)
{
	grid = new WorldGrid(w, h, scale);
}

PhysicsComponent* PhysicsManager::createObject(float x, float y, float w, float h, float r, PhysObjectType::PhysObjectType t)
{
	PhysicsComponent* physicsComp = ComponentManager<PhysicsComponent>::Instance().Create<PhysicsComponent>(t, 0, r, w, h);

	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	bodyDef.angle = r;
	bodyDef.active = false;	// wait for component to be active (valid state)

	b2Body* body;

	b2PolygonShape shape;
	shape.SetAsBox(w/2, h/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;
	fixtureDef.userData = physicsComp;

	switch (t)
	{
	case PhysObjectType::PROJECTILE_UP:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = PROJECTILE_UP_CATEGORY;
		fixtureDef.filter.maskBits = PROJECTILE_UP_MASK;
		physicsComp->isUp = true;
		physicsComp->zPos = Z_UPPER;
		break;
	case PhysObjectType::PROJECTILE_DOWN:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = PROJECTILE_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = PROJECTILE_DOWN_MASK;
		physicsComp->isUp = false;
		physicsComp->zPos = Z_LOWER;
		break;
	case PhysObjectType::CAT_UP:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = CAT_UP_CATEGORY;
		fixtureDef.filter.maskBits = CAT_UP_MASK;
		physicsComp->isUp = true;
		physicsComp->zPos = Z_UPPER;
		break;
	case PhysObjectType::CAT_DOWN:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = CAT_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = CAT_DOWN_MASK;
		physicsComp->isUp = false;
		physicsComp->zPos = Z_LOWER;
		break;
	case PhysObjectType::MOUSE_UP:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = MOUSE_UP_CATEGORY;
		fixtureDef.filter.maskBits = MOUSE_UP_MASK;
		physicsComp->isUp = true;
		physicsComp->zPos = Z_UPPER;
		break;
	case PhysObjectType::MOUSE_DOWN:
		bodyDef.type = b2_dynamicBody;
		fixtureDef.filter.categoryBits = MOUSE_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = MOUSE_DOWN_MASK;
		physicsComp->isUp = false;
		physicsComp->zPos = Z_LOWER;
		break;
	case PhysObjectType::OBSTACLE_UP:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = OBSTACLE_UP_CATEGORY;
		fixtureDef.filter.maskBits = OBSTACLE_UP_MASK;
		physicsComp->isUp = true;
		physicsComp->zPos = Z_UPPER;
		break;
	case PhysObjectType::OBSTACLE_DOWN:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = OBSTACLE_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = OBSTACLE_DOWN_MASK;
		physicsComp->isUp = false;
		physicsComp->zPos = Z_LOWER;
		break;
	case PhysObjectType::WALL:
		bodyDef.type = b2_staticBody;
		fixtureDef.filter.categoryBits = WALL_CATEGORY;
		fixtureDef.filter.maskBits = WALL_MASK;
		physicsComp->zPos = Z_UPPER;
		break;
	default:
		return nullptr; //if they input something that needs a grid
	}

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	physicsComp->body = body;

	body->SetUserData(physicsComp);

	return physicsComp;
}

//For this function the position should be the top left corner
PhysicsComponent* PhysicsManager::createGridObject(float x, float y, int w, int h, PhysObjectType::PhysObjectType t)
{
	PhysicsComponent* physicsComp = ComponentManager<PhysicsComponent>::Instance().Create<PhysicsComponent>(t, 0, 0, w, h);

	b2BodyDef bodyDef;
	bodyDef.active = true;	//wait for component to be active (valid state)
	Vector2D *p1 = new Vector2D(0, 0), *p2 = new Vector2D(0, 0);

	switch (t)
	{
	case PhysObjectType::OBSTACLE_UP:
	case PhysObjectType::OBSTACLE_DOWN:
	case PhysObjectType::PLATFORM:
		p1 = new Vector2D(x - ((float)w / 2), y + ((float)h / 2));
		p2 = new Vector2D(x + ((float)w / 2), y - ((float)h / 2));

		grid->positionArea(*p1, *p2);

		bodyDef.position.Set(p1->x + ((float)w / 2), p1->y - ((float)h / 2));
		break;
	case PhysObjectType::CONTRAPTION_UP:
	case PhysObjectType::CONTRAPTION_DOWN:
	case PhysObjectType::PART:
		p1 = new Vector2D(x, y);

		grid->positionObject(*p1);

		bodyDef.position.Set(p1->x + grid->scale / 2.0f, p1->y - grid->scale / 2.0f);
		break;
	default:
		return nullptr;
	}

	switch (t)
	{
	case PhysObjectType::OBSTACLE_UP:
		physicsComp->zPos = Z_UPPER;
		break;
	case PhysObjectType::OBSTACLE_DOWN:
		physicsComp->zPos = Z_LOWER;
		break;
	case PhysObjectType::PLATFORM:
		physicsComp->zPos = Z_UPPER / 2;
		break;
	case PhysObjectType::CONTRAPTION_UP:
		physicsComp->zPos = Z_UPPER;
		break;
	case PhysObjectType::CONTRAPTION_DOWN:
		physicsComp->zPos = Z_LOWER;
		break;
	case PhysObjectType::PART:
		physicsComp->zPos = Z_LOWER;
		break;
	}

	b2Body* body;

	b2PolygonShape shape;
	shape.SetAsBox(w/2, h/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;
	fixtureDef.userData = physicsComp;

	switch (t)
	{
	case PhysObjectType::OBSTACLE_UP:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = OBSTACLE_UP_CATEGORY;
		fixtureDef.filter.maskBits = OBSTACLE_UP_MASK;
		physicsComp->isUp = true;
		physicsComp->zPos = Z_UPPER;
		break;
	case PhysObjectType::OBSTACLE_DOWN:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = OBSTACLE_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = OBSTACLE_DOWN_MASK;
		physicsComp->isUp = false;
		physicsComp->zPos = Z_LOWER;
		break;
	case PhysObjectType::PLATFORM:
		bodyDef.type = b2_staticBody;
		fixtureDef.filter.categoryBits = PLATFORM_CATEGORY;
		fixtureDef.filter.maskBits = PLATFORM_MASK;
		break;
	case PhysObjectType::CONTRAPTION_UP:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = CONTRAPTION_UP_CATEGORY;
		fixtureDef.filter.maskBits = CONTRAPTION_UP_MASK;
		physicsComp->isUp = true;
		physicsComp->zPos = Z_UPPER;
		break;
	case PhysObjectType::CONTRAPTION_DOWN:
		bodyDef.type = b2_kinematicBody;
		fixtureDef.filter.categoryBits = CONTRAPTION_DOWN_CATEGORY;
		fixtureDef.filter.maskBits = CONTRAPTION_DOWN_MASK;
		physicsComp->isUp = false;
		physicsComp->zPos = Z_LOWER;
		break;
	case PhysObjectType::PART:
		bodyDef.type = b2_staticBody;
		fixtureDef.filter.categoryBits = PART_CATEGORY;
		fixtureDef.filter.maskBits = PART_MASK;
		physicsComp->isUp = false;
		physicsComp->zPos = Z_LOWER;
		break;
	default:
		return nullptr; //if the input something that does use the grid
	}

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	physicsComp->body = body;

	body->SetUserData(physicsComp);

	switch (t)
	{
	case PhysObjectType::OBSTACLE_UP:
	case PhysObjectType::OBSTACLE_DOWN:
	case PhysObjectType::PLATFORM:
		grid->createArea(*p1, *p2, physicsComp, t);
		break;
	case PhysObjectType::CONTRAPTION_UP:
	case PhysObjectType::CONTRAPTION_DOWN:
	case PhysObjectType::PART:
		grid->createObject(*p1, physicsComp);
		break;
	}

	return physicsComp;
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

		if (comp == nullptr || (!comp->isJumping && !comp->isFalling))
		{
			b = b->GetNext();
			continue;
		}

		//If the body is falling or jumping, then we must handle z physics
		//Update z position based on gravity, velocity, and time since last frame
		comp->zPos = comp->zPos + (comp->zVelocity * step) + (GRAVITY * step * step / 2);

		//Update velocity based on gravity and time since last frame
		comp->zVelocity = comp->zVelocity + (GRAVITY * step);

		//The object in the upper half
		if (comp->isUp)
		{
			//Has it reached the platform?
			if (comp->zPos > Z_UPPER)
			{
				comp->isJumping = false;
				comp->isFalling = false;
				comp->zPos = Z_UPPER;
			} //Has it reached the height threshold?
			else if (comp->zPos < Z_THRESHOLD)
			{
				comp->isUp = false;

				//Update the filters and categories to be the lower level versions
				b2Filter filter;

				switch (comp->pType)
				{
				case PhysObjectType::MOUSE_UP:
					comp->pType = PhysObjectType::MOUSE_DOWN;
					filter.categoryBits = MOUSE_DOWN_CATEGORY;
					filter.maskBits = MOUSE_DOWN_MASK;
					break;
				case PhysObjectType::CAT_UP:
					comp->pType = PhysObjectType::CAT_DOWN;
					filter.categoryBits = CAT_DOWN_CATEGORY;
					filter.maskBits = CAT_DOWN_MASK;
					break;
				case PhysObjectType::OBSTACLE_UP:
					comp->pType = PhysObjectType::OBSTACLE_DOWN;
					filter.categoryBits = OBSTACLE_DOWN_CATEGORY;
					filter.maskBits = OBSTACLE_DOWN_MASK;
					break;
				case PhysObjectType::BALL_UP:
					comp->pType = PhysObjectType::BALL_DOWN;
					filter.categoryBits = BALL_DOWN_CATEGORY;
					filter.maskBits = BALL_DOWN_MASK;
					break;
				default:
					break; //you goofed
				}

				b->GetFixtureList()->SetFilterData(filter);
			}
		} //The object is in the lower half
		else
		{
			//Has it reached the height threshold?
			if (comp->zPos > Z_THRESHOLD)
			{
				comp->isUp = true;

				//Update the filters and categories to be the upper level versions
				b2Filter filter;

				switch (comp->pType)
				{
				case PhysObjectType::MOUSE_DOWN:
					comp->pType = PhysObjectType::MOUSE_UP;
					filter.categoryBits = MOUSE_UP_CATEGORY;
					filter.maskBits = MOUSE_UP_MASK;
					break;
				case PhysObjectType::CAT_DOWN:
					comp->pType = PhysObjectType::CAT_UP;
					filter.categoryBits = CAT_UP_CATEGORY;
					filter.maskBits = CAT_UP_MASK;
					break;
				case PhysObjectType::OBSTACLE_DOWN:
					comp->pType = PhysObjectType::OBSTACLE_UP;
					filter.categoryBits = OBSTACLE_UP_CATEGORY;
					filter.maskBits = OBSTACLE_UP_MASK;
					break;
				case PhysObjectType::BALL_DOWN:
					comp->pType = PhysObjectType::BALL_UP;
					filter.categoryBits = BALL_UP_CATEGORY;
					filter.maskBits = BALL_UP_MASK;
					break;
				default:
					break; //you goofed
				}

				b->GetFixtureList()->SetFilterData(filter);
			} //Has it reached the ground?
			else if (comp->zPos < Z_LOWER)
			{
				comp->isFalling = false;
				comp->isJumping = false;
				comp->zPos = Z_LOWER;
			}
		}

		b = b->GetNext();
	}
}

void PhysicsManager::checkCollisions()
{
	//If there are any unhandled collisions
	if (cListener->hasCollided() == 0)
		return;

	b2Body** collider1 = cListener->getColliders1();
	b2Body** collider2 = cListener->getColliders2();

	for (int c = 0; c < cListener->hasCollided(); c++)
	{
		b2Body* c1 = collider1[c];
		b2Body* c2 = collider2[c];

		PhysicsComponent* pComp1 = static_cast<PhysicsComponent*>(c1->GetUserData());
		PhysicsComponent* pComp2 = static_cast<PhysicsComponent*>(c2->GetUserData());
			
		pComp1->onCollide.Notify(pComp2);
		pComp2->onCollide.Notify(pComp1);
	}

	cListener->resetCollided();
}

//returns a list of the objects hit
std::vector<PhysicsComponent*> PhysicsManager::areaCheck(PhysicsComponent * checkedBy, Vector2D * p1, Vector2D * p2)
{
	std::vector<PhysicsComponent*> foundObjects;

	AreaQueryCallback callback;

	b2AABB boundingBox;
	boundingBox.lowerBound = b2Vec2(p1->x, p1->y);
	boundingBox.upperBound = b2Vec2(p2->x, p2->y);

	world->QueryAABB(&callback, boundingBox);

	for (int i = 0; i < callback.foundBodies.size(); i++)
	{
		PhysicsComponent* pComp = static_cast<PhysicsComponent*>(callback.foundBodies[i]->GetUserData());

		foundObjects.push_back(pComp);
	}

	return foundObjects;
}

//returns a list of the objects hit
std::vector<PhysicsComponent*> PhysicsManager::areaCheck(PhysicsComponent* checkedBy, std::set<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2)
{
	std::vector<PhysicsComponent*> foundObjects;

	AreaQueryCallback callback;

	b2AABB boundingBox;
	boundingBox.lowerBound = b2Vec2(p1->x, p1->y);
	boundingBox.upperBound = b2Vec2(p2->x, p2->y);

	world->QueryAABB(&callback, boundingBox);

	for (int i = 0; i < callback.foundBodies.size(); i++)
	{
		PhysicsComponent* pComp = static_cast<PhysicsComponent*>(callback.foundBodies[i]->GetUserData());

		if (toCheck.find(pComp->pType) != toCheck.end())
			foundObjects.push_back(pComp);
	}

	return foundObjects;
}

//returns the first object hit
PhysicsComponent * PhysicsManager::rayCheck(PhysicsComponent * checkedBy, Vector2D * p1, Vector2D * p2, Vector2D& hit)
{
	float32 frac = 1; //used to determine the closest object
	PhysicsComponent* bestMatch = nullptr;
	RayQueryCallback callback;

	b2Vec2 point1 = b2Vec2(p1->x, p1->y);
	b2Vec2 point2 = b2Vec2(p2->x, p2->y);

	world->RayCast(&callback, point1, point2);

	//Find the closest object hit by the ray
	for (int i = 0; i < callback.hitBodies.size(); i++)
	{
		//if the object is further than the best match so far, move on
		if (callback.fractions[i] > frac)
			continue;

		PhysicsComponent* pComp = static_cast<PhysicsComponent*>(callback.hitBodies[i]->GetUserData());
		frac = callback.fractions[i];
		bestMatch = pComp;
	}

	return bestMatch;
}

//returns the first object hit
PhysicsComponent* PhysicsManager::rayCheck(PhysicsComponent* checkedBy, std::set<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2, Vector2D& hit)
{
	float32 frac = 1; //used to determine the closest object
	PhysicsComponent* bestMatch = nullptr;
	RayQueryCallback callback;

	b2Vec2 point1 = b2Vec2(p1->x, p1->y);
	b2Vec2 point2 = b2Vec2(p2->x, p2->y);

	world->RayCast(&callback, point1, point2);

	//Find the closest object hit by the ray
	for (int i = 0; i < callback.hitBodies.size(); i++)
	{
		//if the object is further than the best match so far, move on
		if (callback.fractions[i] > frac)
			continue;

		PhysicsComponent* pComp = static_cast<PhysicsComponent*>(callback.hitBodies[i]->GetUserData());

		if (toCheck.find(pComp->pType) != toCheck.end())
		{
			frac = callback.fractions[i];
			bestMatch = pComp;
		}
	}

	if (bestMatch == nullptr)
		return nullptr;

	b2Vec2 ray = point2 - point1;
	ray *= frac;
	ray = point1 + ray;

	hit = Vector2D(ray.x, ray.y);
	return bestMatch;
}

WorldGrid* PhysicsManager::getGrid()
{
	return grid;
}