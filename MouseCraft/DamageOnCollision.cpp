#include "DamageOnCollision.h"

#include "HealthComponent.h"

DamageOnCollision::DamageOnCollision() : 
	_handleOnCollision(this, &DamageOnCollision::OnCollision)
{
}

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

void DamageOnCollision::SetLayers(std::set<PhysObjectType::PhysObjectType> checkFor)
{
	_checkFor = checkFor;
}

void DamageOnCollision::OnCollision(PhysicsComponent * other)
{
	if (_checkFor.find(other->pType) != _checkFor.end())
	{
		if (damage > 0)
		{
			auto health = other->GetEntity()->GetComponent<HealthComponent>();
			if (health)
				health->Damage(1);
			else
				std::cout << "WARNING: DamageOnCollision couldn't find health to damage." << std::endl;
		}

		if (destroyOnCollision)
			GetEntity()->Destroy();
	}
}