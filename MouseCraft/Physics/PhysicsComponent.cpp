#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(PhysObjectType::PhysObjectType t, float z, float r, float w, float h)
{
	velocity = Vector2D(0, 0);
	zVelocity = 0;
	width = w;
	height = h;
	zPos = z;
	rotation = r;
	isJumping = false;
	pType = t;
}

PhysicsComponent::~PhysicsComponent()
{
	if (body->GetType() != b2BodyType::b2_dynamicBody)
	{
		switch (pType)
		{
		case PhysObjectType::OBSTACLE_UP:
		case PhysObjectType::OBSTACLE_DOWN:
		case PhysObjectType::CONTRAPTION_UP:
		case PhysObjectType::CONTRAPTION_DOWN:
		case PhysObjectType::PLATFORM:
		case PhysObjectType::PART:
			removeFromGrid();
			break;
		}
	}

	body->GetWorld()->DestroyBody(body);
}

void PhysicsComponent::initPosition()
{
	//Make sure the initial position of the entity is the same as the position of the body
	GetEntity()->transform.setLocalPosition(glm::vec3(body->GetPosition().x, zPos, body->GetPosition().y));
}

void PhysicsComponent::moveBody(Vector2D* pos, float angle)
{
	body->SetTransform(b2Vec2(pos->x, pos->y), angle);
}

std::vector<PhysicsComponent*> PhysicsComponent::areaCheck(std::set<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2)
{
	return PhysicsManager::instance()->areaCheck(this, toCheck, p1, p2);
}

PhysicsComponent* PhysicsComponent::rayCheck(std::set<PhysObjectType::PhysObjectType> toCheck, Vector2D* p1, Vector2D* p2, Vector2D& hit)
{
	return PhysicsManager::instance()->rayCheck(this, toCheck, p1, p2, hit);
}

void PhysicsComponent::jump(float upVel, float forwardVel)
{
	zVelocity = upVel;
	isJumping = true;

	stopMoving.Notify();
	velocity = GetEntity()->transform.getWorldForward2D() * forwardVel;
}

void PhysicsComponent::fall()
{
	zVelocity = -5;
	isFalling = true;
	
	stopMoving.Notify();

	if (velocity.x != 0 || velocity.y != 0)
		velocity = velocity * (1.0f/velocity.length()) * FALL_FORWARD_VELOCITY;
}

void PhysicsComponent::landed()
{
	isJumping = false;
	isFalling = false;

	resumeMoving.Notify();
}

bool PhysicsComponent::updateFalling()
{
	if (!isJumping && isUp && !isFalling)
	{
		std::set<PhysObjectType::PhysObjectType> types = std::set<PhysObjectType::PhysObjectType>{
			PhysObjectType::PLATFORM
		};

		auto compPos = body->GetPosition();
		Vector2D* p1 = new Vector2D(compPos.x - (width / 2), compPos.y - (height / 2));
		Vector2D* p2 = new Vector2D(compPos.x + (width / 2), compPos.y + (height / 2));

		std::vector<PhysicsComponent*> found = areaCheck(types, p1, p2);

		//if you aren't on a platform then fall
		if (found.size() == 0)
		{
			fall();
			return true;
		}
	}
	return false;
}

void PhysicsComponent::makeDynamic()
{
	body->SetType(b2BodyType::b2_dynamicBody);
	body->SetAwake(true);
}

void PhysicsComponent::removeCollisions()
{
	b2Filter filter;
	filter.categoryBits = COLLISIONLESS_CATEGORY;
	filter.maskBits = COLLISIONLESS_MASK;

	body->GetFixtureList()->SetFilterData(filter);
}

void PhysicsComponent::removeFromGrid()
{
	PhysicsManager::instance()->getGrid()->removeObject(body->GetPosition().x, body->GetPosition().y);
}

Component* PhysicsComponent::Create(json json)
{
	/*std::string physType = json["physType"].get<std::string>();
	PhysObjectType::PhysObjectType properType = PhysObjectType::NOTHING;

	if (physType == "OBSTACLE_UP")
		properType = PhysObjectType::OBSTACLE_UP;
	else if (physType == "OBSTACLE_DOWN")
		properType = PhysObjectType::OBSTACLE_DOWN;
	else if (physType == "CONTRAPTION_UP")
		properType = PhysObjectType::CONTRAPTION_UP;
	else if (physType == "CONTRAPTION_DOWN")
		properType = PhysObjectType::CONTRAPTION_DOWN;
	else if (physType == "PLATFORM")
		properType = PhysObjectType::PLATFORM;
	else if (physType == "PART")
		properType = PhysObjectType::PART;
	else if (physType == "PROJECTILE_UP")
		properType = PhysObjectType::PROJECTILE_UP;
	else if (physType == "PROJECTILE_DOWN")
		properType = PhysObjectType::PROJECTILE_DOWN;
	else if (physType == "CAT_UP")
		properType = PhysObjectType::CAT_UP;
	else if (physType == "CAT_DOWN")
		properType = PhysObjectType::CAT_DOWN;
	else if (physType == "MOUSE_UP")
		properType = PhysObjectType::MOUSE_UP;
	else if (physType == "MOUSE_DOWN")
		properType = PhysObjectType::MOUSE_DOWN;*/
	
	auto c = PhysicsManager::instance()->createObject(0, 0, 1, 1, 0, PhysObjectType::MOUSE_DOWN);
	return c;
}

// !!! which key you want to load 
PrefabRegistrar PhysicsComponent::reg("TestComponent1", &PhysicsComponent::Create);