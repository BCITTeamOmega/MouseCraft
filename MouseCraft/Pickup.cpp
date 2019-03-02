#include "Pickup.h"

#include "Core/Entity.h"

Pickup::Pickup()
{
}

Pickup::~Pickup()
{
}

void Pickup::OnInitialized()
{
	//Listens for collisions with the physics component
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

	if (pComp != nullptr)
	{
		pComp->onCollide.Attach(this);
		pComp->onHit.Attach(this);
	}
}

void Pickup::Grab()
{
	
}

void Pickup::Drop()
{
	
}
