#include "YarnBall.h"

#include "HealthComponent.h"

YarnBall::YarnBall() :
	HandleMouseCollide(this, &YarnBall::OnMouseCollide)
{
}

YarnBall::~YarnBall()
{
}

void YarnBall::OnInitialized()
{
	Obstacle::OnInitialized();
	_physics = GetEntity()->GetComponent<PhysicsComponent>();
	_physics->onCollide.Attach(HandleMouseCollide);
}

void YarnBall::Update(float deltaTime)
{
	_physics->updateFalling();
}

void YarnBall::HitByCat(Vector2D dir)
{
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

	//Changes the object from a generic obstacle to a ball in the physics manager's eyes
	b2Filter filter;
	pComp->pType = pComp->isUp ? PhysObjectType::BALL_UP : PhysObjectType::BALL_DOWN;
	filter.categoryBits = pComp->isUp ? BALL_UP_CATEGORY : BALL_DOWN_CATEGORY;
	filter.maskBits = pComp->isUp ? BALL_UP_MASK : BALL_DOWN_MASK;
	pComp->body->GetFixtureList()->SetFilterData(filter);

	//Make it dynamic so it collides properly
	pComp->makeDynamic();
	pComp->velocity = dir * SPEED;
}

void YarnBall::DestroyedByMouse()
{
	GetEntity()->Destroy();
}

void YarnBall::OnMouseCollide(PhysicsComponent* other)
{
	if (other->pType == PhysObjectType::MOUSE_DOWN || other->pType == PhysObjectType::MOUSE_UP)
	{
		other->GetEntity()->GetComponent<HealthComponent>()->Damage(1);
		GetEntity()->Destroy();
	}
}

Component * YarnBall::Create(json json)
{
	auto c = ComponentManager<UpdatableComponent>::Instance().Create<YarnBall>();
	return c;
}

PrefabRegistrar YarnBall::reg("YarnBall", &YarnBall::Create);
