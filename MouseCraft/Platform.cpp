#include "Platform.h"

Platform::Platform()
{
}


Platform::~Platform()
{
}

bool Platform::use() {
	Contraption::use();

	fieldEntity->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->transform.setLocalPosition(GetEntity()->transform.getWorldPosition());
	fieldEntity->SetEnabled(true);

	std::cout << "PLATFORM is being used" << std::endl;
	PhysicsComponent* pc = GetEntity()->GetComponent<PhysicsComponent>();
	std::vector<PhysObjectType::PhysObjectType> stuff;

	if (pc->type = PhysObjectType::CONTRAPTION_DOWN) {
		stuff.push_back(PhysObjectType::MOUSE_DOWN);
	}

	auto p1 = fieldEntity->transform;
	auto pos = p1.getWorldPosition();
	auto bl = pos - glm::vec3(-1, 0, -1);
	auto tr = pos - glm::vec3(1, 0, 1);
	//if (pc->areaCheck(stuff, new Vector2D(bl.x, bl.z), new Vector2D(tr.x, tr.z), true)) {
	//	// Get mice and updateHeight

	//}

	return true;
}

void Platform::show() {
	Contraption::show();
}