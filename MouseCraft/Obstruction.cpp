#include "Obstruction.h"

#include "HealthComponent.h"

Obstruction::Obstruction() : 
	HandleMouseCollide(this, &Obstruction::OnMouseCollide)
{
}

Obstruction::~Obstruction()
{
}

void Obstruction::OnInitialized()
{
	Obstacle::OnInitialized();
	_physics = GetEntity()->GetComponent<PhysicsComponent>();
	_physics->onCollide.Attach(HandleMouseCollide);
}

void Obstruction::Update(float deltaTime)
{
}

void Obstruction::HitByCat(Vector2D dir) 
{
	std::cout << "Cat hit an obstruction (book/box)." << std::endl;
	auto pos = GetEntity()->t().wPos();
	auto newPos = Vector2D(pos.x, pos.z) + (dir * HIT_TRAVEL_DIST);
	_physics->moveBody(&newPos, 0);
	_physics->updateFalling();
}

void Obstruction::DestroyedByMouse()
{
	GetEntity()->Destroy();
}

void Obstruction::OnMouseCollide(PhysicsComponent * other)
{
	if (!_physics->isFalling)
		return;

	if (other->pType == PhysObjectType::MOUSE_DOWN || other->pType == PhysObjectType::MOUSE_UP)
	{
		auto health = other->GetEntity()->GetComponent<HealthComponent>();
		health->Damage(100);	// C R U S H E D.
	}
}
