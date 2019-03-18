#include "DamageOnCollision.h"

#include "HealthComponent.h"

DamageOnCollision::DamageOnCollision(std::set<PhysObjectType::PhysObjectType> checkFor) :
	_handleOnCollision(this, &DamageOnCollision::OnCollision),
	_checkFor(checkFor)
{
}

DamageOnCollision::~DamageOnCollision()
{
}

void DamageOnCollision::OnInitialized()
{
	GetEntity()->GetComponent<PhysicsComponent>()->onCollide.Attach(_handleOnCollision);
}

void DamageOnCollision::OnCollision(PhysicsComponent * other)
{
	if (_checkFor.find(other->type) != _checkFor.end())
	{
		auto health = other->GetEntity()->GetComponent<HealthComponent>();
		if (health)
			health->Damage(1);
		else
			std::cout << "WARNING: DamageOnCollision couldn't find health to damage." << std::endl;

		if (destroyOnCollision)
			GetEntity()->Destroy();
	}
}