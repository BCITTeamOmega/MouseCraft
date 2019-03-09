#include "Bomb.h"

Bomb::Bomb()
{
}


Bomb::~Bomb()
{
}

void Bomb::use() {
	Contraption::use();

	// need to dissapear after exploding
	fieldEntity->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->transform.setLocalPosition(GetEntity()->transform.getWorldPosition());
	fieldEntity->SetEnabled(true);
 
	std::cout << "BOMB is being used" << std::endl;
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

void Bomb::show() {
	Contraption::use();
}
