#include "Swords.h"

#include "Mice.h"

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

	if (pc->type = PhysObjectType::CONTRAPTION_DOWN) {
		stuff.push_back(PhysObjectType::CAT_DOWN);
		stuff.push_back(PhysObjectType::OBSTACLE_DOWN);
	}

	if (pc->type = PhysObjectType::CONTRAPTION_UP) {
		stuff.push_back(PhysObjectType::CAT_UP);
		stuff.push_back(PhysObjectType::OBSTACLE_UP);
	}

	auto p1 = GetEntity()->transform;
	auto pos = p1.getWorldPosition();
	pos += p1.getWorldForward();
	auto bl = pos - glm::vec3(-1, 0, -1);
	auto tr = pos - glm::vec3(1, 0, 1);
	if (pc->areaCheck(stuff, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z), true)) {
		mice->dropItem();
		this->GetEntity()->Destroy();
	}
}

void Swords::show() {
	Contraption::show();
}