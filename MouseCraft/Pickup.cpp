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
	_collider = GetEntity()->GetComponent<DebugColliderComponent>();
}

void Pickup::Grab()
{
	_collider->SetEnabled(false);
}

void Pickup::Drop()
{
	_collider->SetEnabled(true);
}
