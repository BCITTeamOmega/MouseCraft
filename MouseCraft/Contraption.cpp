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