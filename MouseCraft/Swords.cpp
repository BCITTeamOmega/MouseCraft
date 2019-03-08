#include "Swords.h"

Swords::Swords()
{
}


Swords::~Swords()
{
}

void Swords::use() {
	Contraption::use();
	std::cout << "SWORDS is being used" << std::endl;

	PhysicsComponent* pc = GetEntity()->GetComponent<PhysicsComponent>();

	std::vector<PhysObjectType::PhysObjectType> stuff;

	//this must change depending on if the contraption is up or down
	stuff.push_back(PhysObjectType::CAT_DOWN);
	
	pc->areaCheck(stuff, GetEntity()->transform.setLocalPosition(glm::vec3(2, 1, 0)), p2, true);
}

void Swords::show() {
	Contraption::show();
}