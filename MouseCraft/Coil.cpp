#include "Coil.h"

#include "Cat.h"
#include "PlayerComponent.h"
#include "Physics/PhysicsManager.h"

Coil::Coil() :
	HandleOnCollision(this, &Coil::OnCollision)
{
}


Coil::~Coil()
{
}

bool Coil::use() {
	Contraption::use();
	
	std::cout << "COIL is being used" << std::endl;

	// drop self
	GetEntity()->transform.setLocalPosition(GetEntity()->transform.getWorldPosition());
	GetEntity()->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->SetEnabled(true);

	_isPlaced = true;

	// determine which layer to check for 
	auto micePhys = GetEntity()->GetParent()->GetComponent<PhysicsComponent>();
	checkFor.insert(micePhys->isUp ? PhysObjectType::CAT_UP : PhysObjectType::CAT_DOWN);
	
	// create physics at the correct location 
	// however we don't actually need physics body for coil so we skip it

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