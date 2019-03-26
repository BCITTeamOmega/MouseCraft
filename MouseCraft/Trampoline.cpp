#include "Trampoline.h"

Trampoline::Trampoline() : HandleOnCollision(this, &Trampoline::OnCollision)
{
}


Trampoline::~Trampoline()
{
}

bool Trampoline::use(Mouse* m) {
	std::cout << "Trampoline is being used" << std::endl;

	auto pos = GetEntity()->transform.getWorldPosition();
	auto up = GetEntity()->GetParent()->GetComponent<PhysicsComponent>()->isUp;

	//physics
	auto ptype = up ? PhysObjectType::CONTRAPTION_UP : PhysObjectType::CONTRAPTION_DOWN;
	auto c_physics = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, ptype);

	GetEntity()->transform.setLocalPosition(pos);
	GetEntity()->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->SetEnabled(true);

	_isPlaced = true;

	PhysicsComponent* pc = GetEntity()->GetComponent<PhysicsComponent>();
	checkFor.insert(PhysObjectType::MOUSE_DOWN);

	GetEntity()->AddComponent(c_physics);

	return true;
	
}

void Trampoline::show() {
	Contraption::show();
}

void Trampoline::OnCollision(PhysicsComponent * other)
{

}

void Trampoline::OnInitialized() {
	Contraption::OnInitialized();
}

void Trampoline::Update(float dt) {
	if (!_isPlaced) return;

	auto pos = GetEntity()->transform.getWorldPosition();
	auto bl = glm::vec2(pos.x, pos.z) + glm::vec2(-1, -1);
	auto tr = glm::vec2(pos.x, pos.z) + glm::vec2(1, 1);

	auto hits = PhysicsManager::instance()->areaCheck(nullptr, checkFor, new Vector2D(bl), new Vector2D(tr));
	bool hitMice = hits.size() > 0;

	if (!_collidedMice && hitMice)
	{
		std::cout << "Mouse touched platform" << std::endl;
		_collidedMice = hits[0]->GetEntity()->GetComponent<PlayerComponent>();
		// Insert jump code
	}
	else if (_collidedMice && !hitMice)
		_collidedMice = nullptr;
}