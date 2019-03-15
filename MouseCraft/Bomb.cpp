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

	if (pc->type = PhysObjectType::CONTRAPTION_DOWN) {
		stuff.push_back(PhysObjectType::CAT_DOWN);
		stuff.push_back(PhysObjectType::OBSTACLE_DOWN);
	}

	if (pc->type = PhysObjectType::CONTRAPTION_UP) {
		stuff.push_back(PhysObjectType::CAT_UP);
		stuff.push_back(PhysObjectType::OBSTACLE_UP);
	}

	// need to deal damage to health 
	if (pc->areaCheck(stuff, new Vector2D(fieldEntity->transform.getWorldPosition().x - 1, fieldEntity->transform.getWorldPosition().y - 1), new Vector2D(fieldEntity->transform.getWorldPosition().x + 1, fieldEntity->transform.getWorldPosition().y + 1), true)) {
		this->Drop();
		this->GetEntity()->Destroy();
	}
}

void Bomb::show() {
	Contraption::use();
}
