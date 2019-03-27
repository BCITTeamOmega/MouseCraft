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
	GetEntity()->GetComponent<PhysicsComponent>()->velocity = dir * SPEED;
}

void YarnBall::DestroyedByMouse()
{
	GetEntity()->Destroy();
}

void YarnBall::OnMouseCollide(PhysicsComponent* other)
{
	if (other->type == PhysObjectType::MOUSE_DOWN || other->type == PhysObjectType::MOUSE_UP)
	{
		other->GetEntity()->GetComponent<HealthComponent>()->Damage(1);
	}
}
