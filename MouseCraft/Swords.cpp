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

	// this must change depending on if the contraption is up or down
	stuff.push_back(PhysObjectType::CAT_DOWN);
	stuff.push_back(PhysObjectType::OBSTACLE_DOWN);
	stuff.push_back(PhysObjectType::CAT_UP);
	stuff.push_back(PhysObjectType::OBSTACLE_UP);

	// need to get p1 and p2
	// need to set mice item to null
	// need to deal damage to health
	if (pc->areaCheck(stuff, p1, p2, true)) {
		this->Drop();
		this->GetEntity()->Destroy();
	}
}

void Swords::show() {
	Contraption::show();
}