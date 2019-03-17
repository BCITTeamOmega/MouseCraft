#include "Swords.h"

#include "Mice.h"

Swords::Swords()
{
}


Swords::~Swords()
{
}

bool Swords::use() {
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
	auto target = pc->areaCheck(stuff, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z));

	if (pc->type = PhysObjectType::CONTRAPTION_DOWN) {
		auto t1 = std::find_if(target.begin(), target.end(), [&stuff](const PhysicsComponent* p) {
			return p->type == PhysObjectType::CAT_DOWN; });

		auto t2 = std::find_if(target.begin(), target.end(), [&stuff](const PhysicsComponent* p) {
			return p->type == PhysObjectType::OBSTACLE_DOWN; });

		if (t1 != target.end()) {
			(*t1)->GetEntity()->GetParent()->GetComponent<HealthComponent>()->Damage(50);
			mice->dropItem();
			this->GetEntity()->Destroy();
		}
	}

	if (pc->type = PhysObjectType::CONTRAPTION_UP) {
		auto t1 = std::find_if(target.begin(), target.end(), [&stuff](const PhysicsComponent* p) {
			return p->type == PhysObjectType::CAT_UP; });

		auto t2 = std::find_if(target.begin(), target.end(), [&stuff](const PhysicsComponent* p) {
			return p->type == PhysObjectType::OBSTACLE_UP; });

		if (t1 != target.end()) {
			(*t1)->GetEntity()->GetParent()->GetComponent<HealthComponent>()->Damage(50);
			mice->dropItem();
			this->GetEntity()->Destroy();
		}
	}

	return false;
}

void Swords::show() {
	Contraption::show();
}