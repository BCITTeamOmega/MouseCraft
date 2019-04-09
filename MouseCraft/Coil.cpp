#include "Coil.h"

#include "Cat.h"
#include "PlayerComponent.h"
#include "Physics/PhysicsManager.h"

Coil::Coil() :
	HandleOnCollision(this, &Coil::OnCollision)
{
	Contraption::type = CONTRAPTIONS::COIL;
}


Coil::~Coil()
{
}

bool Coil::use(Mouse* m) {
	std::cout << "COIL is being used" << std::endl;

	auto pos = GetEntity()->transform.getWorldPosition();
	auto up = GetEntity()->GetParent()->GetComponent<PhysicsComponent>()->isUp;

	//physics
	auto ptype = up ? PhysObjectType::CONTRAPTION_UP : PhysObjectType::CONTRAPTION_DOWN;
	auto c_physics = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, ptype);

	// drop self
	GetEntity()->transform.setLocalPosition(pos);
	GetEntity()->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->SetEnabled(true);

	_isPlaced = true;

	// determine which layer to check for 
	auto mousePhys = GetEntity()->GetParent()->GetComponent<PhysicsComponent>();
	checkFor.insert(mousePhys->isUp ? PhysObjectType::CAT_UP : PhysObjectType::CAT_DOWN);

	GetEntity()->AddComponent(c_physics);

	return true;
}

void Coil::show() {
	Contraption::show();
}

void Coil::OnCollision(PhysicsComponent * other)
{

}

void Coil::OnInitialized()
{
	Contraption::OnInitialized();
	// GetEntity()->GetComponent<PhysicsComponent>()->onCollide.Attach(HandleOnCollision);
}

void Coil::Update(float dt)
{	
	if (!_isPlaced) return;

	auto pos = GetEntity()->transform.getWorldPosition();
	auto bl = glm::vec2(pos.x, pos.z) + glm::vec2(-1, -1) * (FIELD_RANGE / 2);
	auto tr = glm::vec2(pos.x, pos.z) + glm::vec2(1, 1) * (FIELD_RANGE / 2);

	auto hits = PhysicsManager::instance()->areaCheck(nullptr, checkFor, new Vector2D(bl), new Vector2D(tr));
	bool hitCat = hits.size() > 0;

	if (!_collidedCat && hitCat)
	{
		std::cout << "coil hit cat" << std::endl;
		_collidedCat = hits[0]->GetEntity()->GetComponent<PlayerComponent>();
		_catSpeed = _collidedCat->GetSpeed();
		_collidedCat->SetSpeed(_catSpeed * SLOW_MULTIPLIER);
	}
	else if (_collidedCat && !hitCat)
	{
		_collidedCat->SetSpeed(_catSpeed);
		_collidedCat = nullptr;
	}
}