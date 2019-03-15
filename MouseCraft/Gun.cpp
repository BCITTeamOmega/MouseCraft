#include "Gun.h"

#include "Mice.h"

Gun::Gun()
{
}


Gun::~Gun()
{
}

void Gun::use() {
	Contraption::use();

	std::cout << "GUN is being used" << std::endl;
	PhysicsComponent* pc = GetEntity()->GetComponent<PhysicsComponent>();
	std::vector<PhysObjectType::PhysObjectType> stuff;

	// this must change depending on if the contraption is up or down
	stuff.push_back(PhysObjectType::WALL);

	if (pc->type = PhysObjectType::CONTRAPTION_DOWN) {
		stuff.push_back(PhysObjectType::CAT_DOWN);
		stuff.push_back(PhysObjectType::OBSTACLE_DOWN);
		stuff.push_back(PhysObjectType::PLATFORM);
	}

	if (pc->type = PhysObjectType::CONTRAPTION_UP) {
		stuff.push_back(PhysObjectType::CAT_UP);
		stuff.push_back(PhysObjectType::OBSTACLE_UP);
	}

	auto p1 = GetEntity()->transform;
	auto pos = p1.getWorldPosition();
	auto end = pos + p1.getWorldForward() * range;
	pc->rayCheck(stuff, new Vector2D(pos.x, pos.z), new Vector2D(end.x, end.z), true);
	mice->dropItem();
	this->GetEntity()->Destroy();
}

void Gun::show() {
	Contraption::show();
}