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
	auto pos = GetEntity()->transform.getLocalPosition();
	_physics->moveBody(new Vector2D(pos.x, pos.z), 0);
	_physics->SetEnabled(true);
}
