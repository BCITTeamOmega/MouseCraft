#include "Contraption.h"



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
}

void Contraption::use() {

}

void Contraption::show() {

}

void Contraption::Grab() {
	
}

void Contraption::Drop() {
	
}