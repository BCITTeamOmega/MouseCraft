#include "Contraption.h"



Contraption::Contraption()
{
}


Contraption::~Contraption()
{
}

void Contraption::OnInitialized()
{
	_collider = GetEntity()->GetComponent<DebugColliderComponent>();

	//Listens for collisions with the physics component
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

	if (pComp != nullptr)
		pComp->onCollide.Attach(this);
}

void Contraption::use() {

}

void Contraption::show() {

}

void Contraption::Grab() {
	_collider->SetEnabled(false);
}

void Contraption::Drop() {
	_collider->SetEnabled(true);
}