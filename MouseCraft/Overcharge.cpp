#include "Overcharge.h"

Overcharge::Overcharge()
{
}


Overcharge::~Overcharge()
{
}

bool Overcharge::use() {
	Contraption::use();	

	std::cout << "OVERCHARGE is being used" << std::endl;
	this->GetEntity()->GetParent()->GetComponent<HealthComponent>()->AddShield();
	this->Drop();
	this->GetEntity()->Destroy();

	return true;
}

void Overcharge::show() {
	Contraption::show();
}