#include "Contraption.h"

#include "Mouse.h"

Contraption::Contraption()
{
}


Contraption::~Contraption()
{
}

void Contraption::OnInitialized()
{
	//Listens for collisions with the physics component
	PhysicsComponent* pComp = GetEntity()->GetComponent<PhysicsComponent>();

	if (pComp != nullptr)
	{
		pComp->onCollide.Attach(this);
		pComp->onHit.Attach(this);
	}

	mouse = GetEntity()->GetParent()->GetComponent<Mouse>();
}

void Contraption::Update(float dt)
{
}

bool Contraption::use(Mouse* m) {
	return true;
}

void Contraption::show() {

}

void Contraption::Grab() {
	
}

void Contraption::Drop() {
	
}