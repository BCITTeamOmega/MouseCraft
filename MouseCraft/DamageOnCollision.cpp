#include "DamageOnCollision.h"

#include "HealthComponent.h"
#include "Sound\SoundComponent.h"

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
            if (other->pType == PhysObjectType::CAT_DOWN || other->pType == PhysObjectType::CAT_UP) {
                //play cat hit sound
                    other->GetEntity()->GetComponent<SoundComponent>()->ChangeSound(SoundsList::CatScream); //set sound to squeak for mouse
                    auto targetPos = other->GetEntity()->transform.getLocalPosition(); //get mouse current position
                    other->GetEntity()->GetComponent<SoundComponent>()->PlaySound(targetPos.x, targetPos.y, targetPos.z); //play sound
                }
			else
				std::cout << "WARNING: DamageOnCollision couldn't find health to damage." << std::endl;
		}

		if (destroyOnCollision)
			GetEntity()->Destroy();
	}
}