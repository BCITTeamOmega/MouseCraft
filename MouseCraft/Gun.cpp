#include "Gun.h"

Gun::Gun()
{
}


Gun::~Gun()
{
}

void Gun::use() {
	Contraption::use();

	std::cout << "GUN is being used" << std::endl;

	// need to raycast 
	this->Drop();
	this->GetEntity()->Destroy();
}

void Gun::show() {
	Contraption::show();
}