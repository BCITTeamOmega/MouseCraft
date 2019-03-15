#include "Coil.h"

#include "Cat.h"
#include "PlayerComponent.h"
#include "Physics/PhysicsManager.h"

Coil::Coil() :
	HandleOnCollision(this, &Coil::OnCollision)
{
	// todo: Manually subscribing to component update 
	EventManager::Subscribe(EventName::COMPONENT_UPDATE, this);
}


Coil::~Coil()
{
	EventManager::Unsubscribe(EventName::COMPONENT_UPDATE, this);
}

bool Coil::use() {
	Contraption::use();
	
	// drop self
	GetEntity()->SetParent(OmegaEngine::Instance().GetRoot());
	GetEntity()->transform.setLocalPosition(GetEntity()->transform.getWorldPosition());
	fieldEntity->SetEnabled(true);

	std::cout << "COIL is being used" << std::endl;
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
	GetEntity()->GetComponent<PhysicsComponent>()->onCollide.Attach(HandleOnCollision);
}

void Coil::Notify(EventName eventName, Param * params)
{
	if (eventName == EventName::COMPONENT_UPDATE)
	{
		auto dt = static_cast<TypeParam<float>*>(params)->Param;
		update(dt);
	}
}

void Coil::update(float dt)
{
	auto pos = GetEntity()->transform.getWorldPosition();
	auto bl = glm::vec2(pos.x, pos.z) + glm::vec2(-1, -1) * (FIELD_RANGE / 2);
	auto tr = glm::vec2(pos.x, pos.z) + glm::vec2(1, 1) * (FIELD_RANGE / 2);

	std::set<PhysObjectType::PhysObjectType> checkFor{ PhysObjectType::CAT_DOWN, PhysObjectType::CAT_UP };
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
