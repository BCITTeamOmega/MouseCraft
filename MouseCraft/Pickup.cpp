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
	_physics = GetEntity()->GetComponent<PhysicsComponent>();
}

void Pickup::Grab()
{
	_physics->SetEnabled(false);
}

void Pickup::Drop()
{
	_physics->SetEnabled(true);
}
