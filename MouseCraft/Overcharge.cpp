#include "Overcharge.h"

Overcharge::Overcharge()
{
}


Overcharge::~Overcharge()
{
}

void Overcharge::use() {
	Contraption::use();	

	std::cout << "OVERCHARGE is being used" << std::endl;
	this->GetEntity()->GetParent()->GetComponent<HealthComponent>()->AddShield();
	this->Drop();
	this->GetEntity()->Destroy();
}

void Overcharge::show() {
	Contraption::show();
}