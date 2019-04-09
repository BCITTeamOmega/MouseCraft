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
	_rotator = GetEntity()->GetComponent<Rotator>();
}

void Pickup::Grab()
{
	_physics->SetEnabled(false);
	_rotator->SetEnabled(false);
	GetEntity()->transform.setLocalRotation(glm::vec3(0.0f));
}

void Pickup::Drop()
{
	auto pos = GetEntity()->transform.getLocalPosition();
	_physics->moveBody(new Vector2D(pos.x, pos.z), 0);
	_physics->SetEnabled(true);
	_rotator->SetEnabled(true);
	GetEntity()->transform.setLocalRotation(glm::vec3(0.42f, 0.0f, 0.0f));
}

Component* Pickup::Create(json json)
{
	auto c = ComponentManager<Pickup>::Instance().Create<Pickup>();	
	c->type = json["type"].get<PICKUPS>();
	return c;
}

PrefabRegistrar Pickup::reg("Pickup", Pickup::Create);
