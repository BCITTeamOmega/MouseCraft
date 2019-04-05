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
	type = t;
}

PhysicsComponent::~PhysicsComponent()
{
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

void PhysicsComponent::jump()
{
	zVelocity = JUMP_VELOCITY;
	isJumping = true;
}

void PhysicsComponent::fall()
{
	isFalling = true;
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
