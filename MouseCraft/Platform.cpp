#include "Platform.h"

Platform::Platform() : HandleOnCollision(this, &Platform::OnCollision)
{
}


Platform::~Platform()
{
}

bool Platform::use() {
	Contraption::use();

	std::cout << "PLATFORM is being used" << std::endl;

	GetEntity()->transform.setLocalPosition(GetEntity()->transform.getWorldPosition());
	GetEntity()->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->SetEnabled(true);

	_isPlaced = true;

	PhysicsComponent* pc = GetEntity()->GetComponent<PhysicsComponent>();
	checkFor.insert(PhysObjectType::MOUSE_DOWN);

	return true;
	
}

void Platform::show() {
	Contraption::show();
}

void Platform::OnCollision(PhysicsComponent * other)
{

}

void Platform::OnInitialized() {
	Contraption::OnInitialized();
}

void Platform::Update(float dt) {
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